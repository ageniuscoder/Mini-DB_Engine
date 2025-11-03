#ifndef __PAGER_H
#define __PAGER_H

#include <bits/stdc++.h>
#include "../frontend/evaluationWrapper.h"
#include "../b-tree/b_tree.h"

#define PAGE_SIZE 4096
#define PRIMARY_INDEX_DEGREE 100
#define TREE_KEY_UPPER_BOUND 2 * PRIMARY_INDEX_DEGREE - 1

using namespace std;

struct search_constraint
{
    uint64_t attribute_offset;
    uint64_t read_size;
    TOKEN_SET data_type;
    NODE_SET relational_operation;
    string value;
};

struct update_constraint
{
    uint64_t attribute_offset;
    uint64_t operation_size;
    TOKEN_SET data_type;
    string new_value;
};

enum
{
    EXEC_SUCCESS,
    ERROR_TABLE_NOT_FOUND,
    ERROR_ATTRIBUTE_MISMATCH,
    ERROR_ATTRIBUTE_UNDERFLOW,
    ERROR_ATTRIBUTE_OVERFLOW,
} EXEC_CODE;

extern unordered_map<NODE_SET, char> schema_type_conversion;
extern unordered_map<TOKEN_SET, uint64_t> token_set_size_converter;
extern unordered_map<char, uint64_t> size_conversion;

uint64_t get_attribute_count(string &table_schema);

template <typename datatype>
bool compare_raw_values(datatype &lhs, datatype &rhs, NODE_SET OP_CODE);

struct page_header
{
    uint64_t page_record_count;
};

struct HeapFile_Metadata
{
    uint64_t total_offset;
    uint64_t page_count;
    uint64_t record_size;
    uint64_t record_count;
    uint64_t write_page_id;
    uint64_t attribute_count;
    uint64_t schema_offset;
    uint64_t primary_key_string_offset;
    string schema;
    string primary_key_string;

    HeapFile_Metadata();
    HeapFile_Metadata(string _schema, uint64_t _record_size, string _primary_key_string);
};

class Pager
{
private:
    bool heap_file_exists(string *table_name);
    string construct_schema(vector<AST_NODE *> &table_attribute);
    int extract_string_size(string table_schema, int offset);
    uint64_t get_size(string &table_schema);

public:
    unordered_map<string, BTree<key_container<int>, int, PRIMARY_INDEX_DEGREE> *> index_cache;

    Pager();
    void load_index_files();
    vector<string> split_schema(string &table_schema);
    void *serialize(vector<AST_NODE *> &record, uint64_t record_size, vector<string> &schema_chunks);
    void write_heapfile_metadata(ofstream &heap_write_stream, HeapFile_Metadata &required_headers);
    void update_heapfile_metadata(fstream &heap_write_stream, HeapFile_Metadata &required_headers);
    void update_page_to_disk(fstream &heap_write_stream, void *new_page_block, uint64_t record_count, uint64_t page_number, uint64_t total_offset);
    uint64_t get_page_record_count(void *page_block);
    int get_primary_index_operation_size(vector<string> schema_chunks, string primary_key_string);
    bool add_to_heap(AST_NODE *&action_node);

    template <typename stream_type>
    bool create_new_page(stream_type &heap_write_stream, HeapFile_Metadata &current_meta);

    string construct_primary_key_string(string table_schema, AST_NODE *&action_node);
    string get_primary_index_stream_name(string pre_padding, string primary_key_string, vector<string> schema_chunks);
    string get_primary_index_name(string primary_key_string, vector<string> schema_chunks);
    bool create_new_heap(AST_NODE *&action_node);

    template <typename stream_type>
    HeapFile_Metadata deserialize_heapfile_metadata(stream_type &heap_read_stream);

    uint64_t get_attribute_offset(string *attribute, vector<string> &schema_chunks);
    int get_string_size_from_chunk(string chunk);
    string get_string_name_from_chunk(string chunk);
    int get_read_size(TOKEN_SET token_type, AST_NODE *&current_condition_node, vector<string> &schema_chunks);
    string get_search_string(AST_NODE *&condition_node, vector<string> &schema_chunks);
    vector<search_constraint> get_search_constraints(AST_NODE *&condition_node, vector<string> &schema_chunks);
    vector<update_constraint> get_update_constraints(vector<AST_NODE *> &update_vector_node, vector<string> &schema_chunks);
    bool match_search_constraints(vector<search_constraint> &data_constraints, void *page_block, uint64_t record_count, uint64_t record_size);
    void export_deserialization(string table_name, ofstream &mysql_write_stream);
    bool get_heap(AST_NODE *&action_node);
    void update_page_record(void *page_block, uint64_t record_count, uint64_t record_size, vector<update_constraint> &update_values);
    void delete_page_record(void *page_block, uint64_t record_number, uint64_t record_size, uint64_t total_record);
    bool delete_from_heap(AST_NODE *&action_node);
    bool update_heap(AST_NODE *&action_node);
    void deserialize(void *page_block, uint64_t read_offset, vector<string> &schema_chunks, bool exporting = false, ofstream *export_stream = nullptr);
};

// Template implementations must be in header
template <typename datatype>
bool compare_raw_values(datatype &lhs, datatype &rhs, NODE_SET OP_CODE)
{
    switch (OP_CODE)
    {
    case NODE_CONDITION_EQUALS:
        return lhs == rhs;
    case NODE_CONDITION_GREATER_THAN:
        return lhs > rhs;
    case NODE_CONDITION_GREATER_THAN_EQUALS:
        return lhs >= rhs;
    case NODE_CONDITION_LESS_THAN:
        return lhs < rhs;
    case NODE_CONDITION_LESS_THAN_EQUALS:
        return lhs <= rhs;
    case NODE_CONDITION_NOT_EQUALS:
        return lhs != rhs;
    }
    return false;
}

template <typename stream_type>
bool Pager::create_new_page(stream_type &heap_write_stream, HeapFile_Metadata &current_meta)
{
    void *new_page_block = malloc(PAGE_SIZE);
    uint64_t start_record_count = 0;
    memcpy(new_page_block, &start_record_count, sizeof(uint64_t));
    heap_write_stream.write(reinterpret_cast<char *>(new_page_block), PAGE_SIZE);
    current_meta.page_count++;
    free(new_page_block);
    return true;
}

template <typename stream_type>
HeapFile_Metadata Pager::deserialize_heapfile_metadata(stream_type &heap_read_stream)
{
    HeapFile_Metadata heapfile_headers;
    heap_read_stream.read(reinterpret_cast<char *>(&heapfile_headers.total_offset), sizeof(uint64_t));
    heap_read_stream.read(reinterpret_cast<char *>(&heapfile_headers.page_count), sizeof(uint64_t));
    heap_read_stream.read(reinterpret_cast<char *>(&heapfile_headers.record_count), sizeof(uint64_t));
    heap_read_stream.read(reinterpret_cast<char *>(&heapfile_headers.record_size), sizeof(uint64_t));
    heap_read_stream.read(reinterpret_cast<char *>(&heapfile_headers.write_page_id), sizeof(uint64_t));
    heap_read_stream.read(reinterpret_cast<char *>(&heapfile_headers.attribute_count), sizeof(uint64_t));
    heap_read_stream.read(reinterpret_cast<char *>(&heapfile_headers.schema_offset), sizeof(uint64_t));

    char *schema_buffer_pointer = (char *)malloc(heapfile_headers.schema_offset + 1);
    heap_read_stream.read(schema_buffer_pointer, heapfile_headers.schema_offset);
    schema_buffer_pointer[heapfile_headers.schema_offset] = '\0';

    heapfile_headers.schema.assign(schema_buffer_pointer);
    free(schema_buffer_pointer);

    heap_read_stream.read(reinterpret_cast<char *>(&heapfile_headers.primary_key_string_offset), sizeof(uint64_t));

    char *primary_key_string_buffer_pointer = (char *)malloc(heapfile_headers.primary_key_string_offset + 1);
    heap_read_stream.read(primary_key_string_buffer_pointer, heapfile_headers.primary_key_string_offset);
    primary_key_string_buffer_pointer[heapfile_headers.primary_key_string_offset] = '\0';

    heapfile_headers.primary_key_string.assign(primary_key_string_buffer_pointer);
    free(primary_key_string_buffer_pointer);

    return heapfile_headers;
}

#endif