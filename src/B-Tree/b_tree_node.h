#ifndef __B_TREE_NODE_H
#define __B_TREE_NODE_H

#include <bits/stdc++.h>
using namespace std;

template <typename tree_type, int t = 3>
class BTreeNode
{
public:
    vector<tree_type> keys;
    vector<BTreeNode *> children;
    bool leaf;
    int n;

    BTreeNode(bool isLeaf = true)
    {
        leaf = isLeaf;
        n = 0;
        keys.reserve(2 * t - 1);
        children.reserve(2 * t);
    }

    int findKey(tree_type k)
    {
        int idx = 0;
        while (idx < n && keys[idx] < k)
            ++idx;
        return idx;
    }

    void splitChild(int i, BTreeNode *y)
    {
        BTreeNode *z = new BTreeNode(y->leaf);
        z->n = t - 1;

        for (int j = 0; j < t - 1; j++)
            z->keys.push_back(y->keys[j + t]);

        if (!y->leaf)
        {
            for (int j = 0; j < t; j++)
                z->children.push_back(y->children[j + t]);
        }

        y->n = t - 1;

        for (int j = n; j >= i + 1; j--)
            children[j + 1] = children[j];

        children[i + 1] = z;

        for (int j = n - 1; j >= i; j--)
            keys[j + 1] = keys[j];

        keys[i] = y->keys[t - 1];
        n = n + 1;
    }

    void insertNonFull(tree_type k)
    {
        int i = n - 1;
        if (leaf)
        {
            keys.resize(n + 1);
            while (i >= 0 && keys[i] > k)
            {
                keys[i + 1] = keys[i];
                i--;
            }
            keys[i + 1] = k;
            n = n + 1;
        }
        else
        {
            while (i >= 0 && keys[i] > k)
                i--;
            i++;
            if (children[i]->n == 2 * t - 1)
            {
                splitChild(i, children[i]);
                if (keys[i] < k)
                    i++;
            }
            children[i]->insertNonFull(k);
        }
    }

    void remove(tree_type k);
    void removeFromLeaf(int idx);
    void removeFromNonLeaf(int idx);
    tree_type getPred(int idx);
    tree_type getSucc(int idx);
    void fill(int idx);
    void borrowFromPrev(int idx);
    void borrowFromNext(int idx);
    void merge(int idx);
};

// Definitions for remaining functions (remove, fill, merge, etc.) can stay here or be inlined.
// You can copy them directly from your current code.

#endif
