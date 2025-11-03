#ifndef __KEY_CONTAINER_H
#define __KEY_CONTAINER_H

#include <bits/stdc++.h>
using namespace std;

template <typename key_type>
class key_container
{
public:
    key_type main_key;
    uint64_t key_offset;

    key_container(key_type value = key_type(), uint64_t _key_offset = 0)
    {
        main_key = value;
        key_offset = _key_offset;
    }

    bool operator==(const key_container<key_type> &rhs) { return this->main_key == rhs.main_key; }
    bool operator!=(const key_container<key_type> &rhs) { return this->main_key != rhs.main_key; }
    bool operator<(const key_container<key_type> &rhs) { return this->main_key < rhs.main_key; }
    bool operator<=(const key_container<key_type> &rhs) { return this->main_key <= rhs.main_key; }
    bool operator>(const key_container<key_type> &rhs) { return this->main_key > rhs.main_key; }
    bool operator>=(const key_container<key_type> &rhs) { return this->main_key >= rhs.main_key; }

    bool operator==(const key_type &rhs) { return this->main_key == rhs; }
    bool operator!=(const key_type &rhs) { return this->main_key != rhs; }
    bool operator<(const key_type &rhs) { return this->main_key < rhs; }
    bool operator<=(const key_type &rhs) { return this->main_key <= rhs; }
    bool operator>(const key_type &rhs) { return this->main_key > rhs; }
    bool operator>=(const key_type &rhs) { return this->main_key >= rhs; }

    template <typename ostream_key_type>
    friend ostream &operator<<(ostream &os, const key_container<ostream_key_type> &operand)
    {
        os << operand.main_key << "(" << operand.key_offset << ")";
        return os;
    }
};

#endif
