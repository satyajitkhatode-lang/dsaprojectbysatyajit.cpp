#include <iostream>
#include <algorithm>
using namespace std;

struct Node {
    int key;
    Node* left;
    Node* right;
    int height;

    Node(int value) {
        key = value;
        left = NULL;
        right = NULL;
        height = 1;
    }
};

int height(Node* node) {
    if (node == NULL)
        return 0;
    return node->height;
}

int getBalance(Node* node) {
    if (node == NULL)
        return 0;
    return height(node->left) - height(node->right);
}

void updateHeight(Node* node) {
    node->height = 1 + max(height(node->left),
                           height(node->right));
}

Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* temp = x->right;

    x->right = y;
    y->left = temp;

    updateHeight(y);
    updateHeight(x);

    return x;
}

Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* temp = y->left;

    y->left = x;
    x->right = temp;

    updateHeight(x);
    updateHeight(y);

    return y;
}

Node* rebalance(Node* root) {
    if (root == NULL)
        return root;

    updateHeight(root);
    int balance = getBalance(root);

    if (balance > 1) {
        if (getBalance(root->left) < 0)
            root->left = leftRotate(root->left);

        return rightRotate(root);
    }

    if (balance < -1) {
        if (getBalance(root->right) > 0)
            root->right = rightRotate(root->right);

        return leftRotate(root);
    }

    return root;
}

Node* insert(Node* root, int key) {
    if (root == NULL)
        return new Node(key);

    if (key < root->key)
        root->left = insert(root->left, key);
    else if (key > root->key)
        root->right = insert(root->right, key);
    else
        return root;

    return rebalance(root);
}

Node* findMin(Node* root) {
    while (root->left != NULL)
        root = root->left;

    return root;
}

Node* findMax(Node* root) {
    while (root->right != NULL)
        root = root->right;

    return root;
}

// method = 1: inorder successor
// method = 2: inorder predecessor
Node* deleteNode(Node* root, int key, int method) {
    if (root == NULL)
        return NULL;

    if (key < root->key) {
        root->left = deleteNode(root->left, key, method);
    }
    else if (key > root->key) {
        root->right = deleteNode(root->right, key, method);
    }
    else {
        if (root->left == NULL || root->right == NULL) {
            Node* child;

            if (root->left != NULL)
                child = root->left;
            else
                child = root->right;

            delete root;
            return child;
        }

        Node* replacement;

        if (method == 1) {
            replacement = findMin(root->right);
            root->key = replacement->key;

            root->right = deleteNode(
                root->right, replacement->key, method);
        }
        else {
            replacement = findMax(root->left);
            root->key = replacement->key;

            root->left = deleteNode(
                root->left, replacement->key, method);
        }
    }

    return rebalance(root);
}

void inorder(Node* root) {
    if (root == NULL)
        return;

    inorder(root->left);
    cout << root->key << " ";
    inorder(root->right);
}

void display(Node* root) {
    cout << "Inorder traversal: ";
    inorder(root);
    cout << '\n';
}

void freeTree(Node* root) {
    if (root == NULL)
        return;

    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

int main() {
    Node* root = NULL;
    int n, value, key, method;

    cout << "Enter number of nodes: ";
    cin >> n;

    if (n < 0) {
        cout << "Invalid number of nodes.\n";
        return 0;
    }

    cout << "Enter node values:\n";

    for (int i = 0; i < n; i++) {
        cin >> value;
        root = insert(root, value);
    }

    cout << "\nAVL tree before deletion:\n";
    display(root);

    cout << "\nEnter key to delete: ";
    cin >> key;

    cout << "Choose replacement method:\n";
    cout << "1. Inorder successor\n";
    cout << "2. Inorder predecessor\n";
    cout << "Enter choice: ";
    cin >> method;

    if (method != 1 && method != 2) {
        cout << "Invalid choice.\n";
        freeTree(root);
        return 0;
    }

    root = deleteNode(root, key, method);

    cout << "\nAVL tree after deletion:\n";
    display(root);

    freeTree(root);
    return 0;
}