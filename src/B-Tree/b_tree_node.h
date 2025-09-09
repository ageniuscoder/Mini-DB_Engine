#ifndef B_TREE_NODE_H
#define B_TREE_NODE_H

#include <iostream>
#include <vector>

template <typename tree_type,int t=3>
class BTreeNode{
    public:
    std::vector<tree_type> keys;    //stores the keys within the node.
    std::vector<BTreeNode*> children;
    bool leaf;     //is leaf node or not
    int n;    //no of keys in node

    BTreeNode(bool isLeaf=true){
        leaf=isLeaf;
        n=0;
        keys.reserve(2*t-1);
        children.reserve(2*t);
    }

    //node operation
    int findKey(tree_type k){
        int idx=0;
        while(idx<n && keys[idx]<k){
            idx++;
        }
        return idx;
    } 

    void splitChild(int i, BTreeNode* y)
    {
        BTreeNode* z = new BTreeNode(y->leaf);
        z->n = t - 1;

        for (int j = 0; j < t - 1; j++)
            z->keys.push_back(y->keys[j + t]);

        if (!y->leaf) {
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
            keys.push_back(k);
            while (i >= 0 && keys[i] > k) {
                keys[i + 1] = keys[i];
                i--;
            }
            keys[i + 1] = k;
            n = n + 1;
        } else {
            while (i >= 0 && keys[i] > k)
                i--;
            i++;

            if (children[i]->n == 2 * t - 1) {
                splitChild(i, children[i]);
                if (keys[i] < k)
                    i++;
            }
            children[i]->insertNonFull(k);
        }
    }

    void remove(tree_type k)
    {
        int idx = findKey(k);

        if (idx < n && keys[idx] == k) {
            if (leaf)
                removeFromLeaf(idx);
            else
                removeFromNonLeaf(idx);
        } else {
            if (leaf) {
                std::cout << "The key " << k << " does not exist in the tree\n";
                return;
            }

            bool flag = (idx == n);

            if (children[idx]->n < t)
                fill(idx);

            if (flag && idx > n)
                children[idx - 1]->remove(k);
            else
                children[idx]->remove(k);
        }
    }

    void removeFromLeaf(int idx)
    {
        for (int i = idx + 1; i < n; ++i)
            keys[i - 1] = keys[i];
        n--;
    }

    void removeFromNonLeaf(int idx)
    {
        tree_type k = keys[idx];

        if (children[idx]->n >= t) {
            tree_type pred = getPred(idx);
            keys[idx] = pred;
            children[idx]->remove(pred);
        }
        else if (children[idx + 1]->n >= t) {
            tree_type succ = getSucc(idx);
            keys[idx] = succ;
            children[idx + 1]->remove(succ);
        }
        else {
            merge(idx);
            children[idx]->remove(k);
        }
    }

    tree_type getPred(int idx)
    {
        BTreeNode* cur = children[idx];
        while (!cur->leaf)
            cur = cur->children[cur->n];
        return cur->keys[cur->n - 1];
    }

    tree_type getSucc(int idx)
    {
        BTreeNode* cur = children[idx + 1];
        while (!cur->leaf)
            cur = cur->children[0];
        return cur->keys[0];
    }

    void fill(int idx)
    {
        if (idx != 0 && children[idx - 1]->n >= t)
            borrowFromPrev(idx);
        else if (idx != n && children[idx + 1]->n >= t)
            borrowFromNext(idx);
        else {
            if (idx != n)
                merge(idx);
            else
                merge(idx - 1);
        }
    }

    void borrowFromPrev(int idx)
    {
        BTreeNode* child = children[idx];
        BTreeNode* sibling = children[idx - 1];

        for (int i = child->n - 1; i >= 0; --i)
            child->keys[i + 1] = child->keys[i];

        if (!child->leaf) {
            for (int i = child->n; i >= 0; --i)
                child->children[i + 1] = child->children[i];
        }

        child->keys[0] = keys[idx - 1];

        if (!child->leaf)
            child->children[0] = sibling->children[sibling->n];

        keys[idx - 1] = sibling->keys[sibling->n - 1];

        child->n += 1;
        sibling->n -= 1;
    }

    void borrowFromNext(int idx)
    {
        BTreeNode* child = children[idx];
        BTreeNode* sibling = children[idx + 1];

        child->keys[child->n] = keys[idx];

        if (!child->leaf)
            child->children[child->n + 1] = sibling->children[0];

        keys[idx] = sibling->keys[0];

        for (int i = 1; i < sibling->n; ++i)
            sibling->keys[i - 1] = sibling->keys[i];

        if (!sibling->leaf) {
            for (int i = 1; i <= sibling->n; ++i)
                sibling->children[i - 1] = sibling->children[i];
        }

        child->n += 1;
        sibling->n -= 1;
    }

    void merge(int idx)
    {
        BTreeNode* child = children[idx];
        BTreeNode* sibling = children[idx + 1];

        child->keys[t - 1] = keys[idx];

        for (int i = 0; i < sibling->n; ++i)
            child->keys[i + t] = sibling->keys[i];

        if (!child->leaf) {
            for (int i = 0; i <= sibling->n; ++i)
                child->children[i + t] = sibling->children[i];
        }

        for (int i = idx + 1; i < n; ++i)
            keys[i - 1] = keys[i];

        for (int i = idx + 2; i <= n; ++i)
            children[i - 1] = children[i];

        child->n += sibling->n + 1;
        n--;

        delete sibling;
    }
};

#endif // !B_TREE_NODE_H