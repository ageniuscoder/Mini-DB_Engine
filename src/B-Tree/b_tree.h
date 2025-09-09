#ifndef __B_TREE_H
#define __B_TREE_H

#include <iostream>
#include <queue>
#include <vector>
#include <fstream>
#include <cstdint>

#include "b_tree_node.h"
#include "key_container.h"

template <typename tree_type, typename inner_type, int t = 3>
class BTree {
private:
    uint64_t tree_key_count;
    uint16_t key_size;

    void deleteTree(BTreeNode<tree_type, t>* node)
    {
        if (node == nullptr) return;
        if (!node->leaf) {
            for (size_t i = 0; i <= node->n; i++) {
                deleteTree(node->children[i]);
            }
        }
        delete node;
    }

public:
    BTreeNode<tree_type, t>* root;

    BTree()
    {
        root = nullptr;
        tree_key_count = 0;
        key_size = sizeof(int);
    }

    BTree(uint16_t _key_size)
    {
        root = nullptr;
        tree_key_count = 0;
        key_size = _key_size;
    }

    template <typename type>
    void write_key_containter(std::ofstream& b_tree_write_stream, key_container<type>& current_key)
    {
        b_tree_write_stream.write(reinterpret_cast<char*>(&current_key.main_key), this->key_size);
        b_tree_write_stream.write(reinterpret_cast<char*>(&current_key.key_offset), sizeof(uint64_t));
    }

    void disk_serialize(std::string file_name)
    {
        std::ofstream b_tree_write_stream(file_name, std::ios::binary);

        b_tree_write_stream.write(reinterpret_cast<char*>(&tree_key_count), sizeof(uint64_t));
        b_tree_write_stream.write(reinterpret_cast<char*>(&this->key_size), sizeof(uint16_t));

        std::queue<BTreeNode<tree_type, t>*> q;
        if (root)
            q.push(root);

        while (!q.empty()) {
            BTreeNode<tree_type, t>* current = q.front();
            q.pop();

            for (int j = 0; j < current->n; j++)
                write_key_containter(b_tree_write_stream, current->keys[j]);

            if (!current->leaf) {
                for (int j = 0; j <= current->n; j++) {
                    if (current->children[j])
                        q.push(current->children[j]);
                }
            }
        }
        b_tree_write_stream.close();
    }

    void insert(tree_type k)
    {
        tree_key_count++;
        if (root == nullptr)
        {
            root = new BTreeNode<tree_type, t>();
            root->keys.push_back(k);
            root->n = 1;
        }
        else {
            if (root->n == 2 * t - 1) {
                BTreeNode<tree_type, t>* s = new BTreeNode<tree_type, t>(false);
                s->children.push_back(root);
                s->splitChild(0, root);
                int i = 0;
                if (s->keys[0] < k)
                    i++;
                s->children[i]->insertNonFull(k);
                root = s;
            }
            else {
                root->insertNonFull(k);
            }
        }
    }

    void load(std::string index_file_name)
    {
        std::ifstream tree_read_stream(index_file_name, std::ios::binary);

        uint16_t read_key_size;
        uint64_t read_key_count;

        tree_read_stream.read(reinterpret_cast<char*>(&read_key_count), sizeof(uint64_t));
        tree_read_stream.read(reinterpret_cast<char*>(&read_key_size), sizeof(uint16_t));

        while (read_key_count--)
        {
            inner_type main_key;
            uint64_t key_offset;

            tree_read_stream.read(reinterpret_cast<char*>(&main_key), read_key_size);
            tree_read_stream.read(reinterpret_cast<char*>(&key_offset), sizeof(uint64_t));

            this->insert(key_container<inner_type>(main_key, key_offset));
        }
        tree_read_stream.close();
    }

    tree_type search(BTreeNode<tree_type, t>*& current, tree_type value)
    {
        if (current == nullptr)
        {
            key_container<inner_type> not_found(-1, -1);
            return not_found;
        }

        int i = 0;
        while (i < current->n && value > current->keys[i])
        {
            i++;
        }

        if (i < current->n && value == current->keys[i])
            return current->keys[i];

        if (current->leaf)
        {
            key_container<inner_type> not_found(-1, -1);
            return not_found;
        }

        return search(current->children[i], value);
    }

    void remove(tree_type k)
    {
        if (!root)
        {
            std::cout << "The tree is empty\n";
            return;
        }
        root->remove(k);
        if (root->n == 0) {
            BTreeNode<tree_type, t>* tmp = root;
            if (root->leaf)
                root = nullptr;
            else
                root = root->children[0];
            delete tmp;
        }
    }

    void levelOrderTraversal()
    {
        if (root == nullptr) {
            std::cout << "Tree is empty\n";
            return;
        }

        std::queue<BTreeNode<tree_type, t>*> q;
        q.push(root);

        while (!q.empty()) {
            int size = q.size();
            for (int i = 0; i < size; i++) {
                BTreeNode<tree_type, t>* current = q.front();
                q.pop();

                for (int j = 0; j < current->n; j++) {
                    std::cout << current->keys[j] << " ";
                }
                std::cout << "\t\t";

                if (!current->leaf) {
                    for (int j = 0; j <= current->n; j++) {
                        if (current->children[j])
                            q.push(current->children[j]);
                    }
                }
            }
            std::cout << "\n";
        }
    }

    ~BTree()
    {
        if (root != nullptr) {
            deleteTree(root);
        }
    }
};

#endif // __B_TREE_H