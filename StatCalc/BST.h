#ifndef BST_H
#define BST_H
#include <vector>

struct Node {
    double data;
    Node *left, *right;
    Node(double val) : data(val), left(nullptr), right(nullptr) {}
};

class BST {
private:
    Node* root;
    void insert(Node*& node, double val) {
        if (!node) node = new Node(val);
        else if (val < node->data) insert(node->left, val);
        else insert(node->right, val);
    }
    void inOrder(Node* node, std::vector<double>& v) {
        if (!node) return;
        inOrder(node->left, v);
        v.push_back(node->data);
        inOrder(node->right, v);
    }
    void deleteTree(Node* node) {
        if (!node) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

public:
    BST() : root(nullptr) {}
    ~BST() { deleteTree(root); }
    
    void add(double val) { insert(root, val); }
    
    void clear() { 
        deleteTree(root);
        root = nullptr; 
    }

    std::vector<double> getSorted() {
        std::vector<double> v;
        inOrder(root, v);
        return v;
    }
};
#endif