#ifndef BST_H
#define BST_H

#include <vector>
struct Node {
    float data;
    Node* left;
    Node* right;
};

Node* newNode(float key) {
    Node* temp = new Node;
    temp->data = key;
    temp->left = NULL;
    temp->right = NULL;
    return temp;
}

class BST {
private:
    Node* root;
    void insertNode(Node* current, float key) {
        if (current == NULL) {
            current = newNode(key);
            return;
        }
        if (key < current->data) {
            insertNode(current->left, key);
        } else {
            insertNode(current->right, key);
        }
    }

    // in-order traversal to get sorted values
    void inOrderTraversal(Node* current, std::vector<float>& result) {
        if (current == NULL) return;
        inOrderTraversal(current->left, result);
        result.push_back(current->data);
        inOrderTraversal(current->right, result);
    }

    //  delete all nodes (post-order)
    void deleteTree(Node* current) {
        if (current == NULL) return;
        deleteTree(current->left);
        deleteTree(current->right);
        delete current;
    }

public:
    // Constructor
    BST() {
        root = NULL;
    }

    // Destructor
    ~BST() {
        deleteTree(root);
    }

    // Add a value to BST
    void add(float key) {
        insertNode(root, key);
    }

    // Clear the entire tree
    void clear() {
        deleteTree(root);
        root = NULL;
    }

    // Return sorted values
    std::vector<float> getSorted() {
        std::vector<float> sortedValues;
        inOrderTraversal(root, sortedValues);
        return sortedValues;
    }
};

#endif
