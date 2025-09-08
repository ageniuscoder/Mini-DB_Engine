#ifndef KEY_CONTAINER_H
#define KEY_CONTAINER_H

#include <iostream>
#include <cstdint>

template <typename key_type>
class key_container{
    // 1.Member Variable
    key_type main_key;
    uint64_t key_offset;

    key_container(key_type value,uint64_t _key_offset){
        main_key=value;
        key_offset=_key_offset;
    }
    
    //Comparison Operators (for key_container objects)
    bool operator==(const key_container<key_type> &rhs){
        return this->main_key==rhs->main_key;
    }

    bool operator!=(const key_container<key_type> &rhs){
        return this->main_key!=rhs->main_key;
    }

    bool operator<(const key_container<key_type> &rhs){
        return this->main_key<rhs->main_key;
    }

    bool operator<=(const key_container<key_type>& rhs)
    {
        return this->main_key <= rhs.main_key;
    }

    bool operator>(const key_container<key_type>& rhs)
    {
        return this->main_key > rhs.main_key;
    }

    bool operator>=(const key_container<key_type>& rhs)
    {
        return this->main_key >= rhs.main_key;
    }

    //Comparison Operators (for raw key_type values) //matches object main_key with search key

    bool operator==(const key_type &rhs){
        return this->main_key==rhs;
    }

    bool operator!=(const key_type& rhs)
    {
        return this->main_key != rhs;
    }

    bool operator<(const key_type& rhs)
    {
        return this->main_key < rhs;
    }

    bool operator<=(const key_type& rhs)
    {
        return this->main_key <= rhs;
    }

    bool operator>(const key_type& rhs)
    {
        return this->main_key > rhs;
    }

    bool operator>=(const key_type& rhs)
    {
        return this->main_key >= rhs;
    }

    //Friend Stream Insertion Operator
    template<typename ostream_key_type>
    friend std::ostream& operator << (std::ostream &os,const key_container<overloadT> &operand);

};

template<typename overloadT>
 std::ostream& operator << (std::ostream &os,const key_container<overloadT> &operand){
    os << operand.main_key << "(" <<operand.key_offset << ")";
    return os;
 }

#endif // !KEY_CONTAINER_H