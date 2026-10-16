#include <iostream>
#include <algorithm>
using namespace std;

struct Node {
    int key;
    Node *left;
    Node *right;
    int height;

    Node(int value) {
        key = value;
        left = NULL;
        right = NULL;
        height = 1;
    }
};

int getHeight(Node* node) {
    if (node == NULL)
        return 0;
    return node->height;
}

int getBalance(Node* node) {
    if (node == NULL)
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}

void updateHeight(Node* node) {
    node->height = 1 + max(getHeight(node->left),
                           getHeight(node->right));
}

Node* rotateRight(Node* y) {
    cout << "Rotation: Right Rotation\n";

    Node* x = y->left;
    Node* temp = x->right;

    x->right = y;
    y->left = temp;

    updateHeight(y);
    updateHeight(x);

    return x;
}

Node* rotateLeft(Node* x) {
    cout << "Rotation: Left Rotation\n";

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

    // Left-heavy tree
    if (balance > 1) {
        if (getBalance(root->left) < 0) {
            cout << "LR case detected\n";
            root->left = rotateLeft(root->left);
        }
        return rotateRight(root);
    }

    // Right-heavy tree
    if (balance < -1) {
        if (getBalance(root->right) > 0) {
            cout << "RL case detected\n";
            root->right = rotateRight(root->right);
        }
        return rotateLeft(root);
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
    while (root != NULL && root->left != NULL)
        root = root->left;
    return root;
}

Node* findMax(Node* root) {
    while (root != NULL && root->right != NULL)
        root = root->right;
    return root;
}

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
        // Node has zero or one child
        if (root->left == NULL || root->right == NULL) {
            Node* child;

            if (root->left != NULL)
                child = root->left;
            else
                child = root->right;

            delete root;
            return child;
        }

        // Node has two children
        Node* replacement;

        if (method == 1) {
            replacement = findMin(root->right);
            cout << "Using inorder successor: "
                 << replacement->key << "\n";

            root->key = replacement->key;
            root->right = deleteNode(
                root->right, replacement->key, method);
        }
        else {
            replacement = findMax(root->left);
            cout << "Using inorder predecessor: "
                 << replacement->key << "\n";

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

bool checkSorted(Node* root, int& previous, bool& first) {
    if (root == NULL)
        return true;

    if (!checkSorted(root->left, previous, first))
        return false;

    if (!first && root->key <= previous)
        return false;

    previous = root->key;
    first = false;

    return checkSorted(root->right, previous, first);
}

bool checkBalance(Node* root) {
    if (root == NULL)
        return true;

    int leftHeight = getHeight(root->left);
    int rightHeight = getHeight(root->right);

    int actualHeight = 1 + max(leftHeight, rightHeight);

    if (root->height != actualHeight)
        return false;

    if (leftHeight - rightHeight > 1 ||
        rightHeight - leftHeight > 1)
        return false;

    return checkBalance(root->left) &&
           checkBalance(root->right);
}

void displayChecks(Node* root) {
    int previous = 0;
    bool first = true;

    cout << "Inorder traversal: ";
    inorder(root);
    cout << "\n";

    bool sorted = checkSorted(root, previous, first);
    bool balanced = checkBalance(root);

    cout << "Inorder sorted: "
         << (sorted ? "YES" : "NO") << "\n";

    cout << "AVL balance and heights valid: "
         << (balanced ? "YES" : "NO") << "\n";
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

    cout << "\nBefore deletion:\n";
    displayChecks(root);

    cout << "\nEnter key to delete: ";
    cin >> key;

    cout << "1. Inorder Successor\n";
    cout << "2. Inorder Predecessor\n";
    cout << "Enter choice: ";
    cin >> method;

    if (method != 1 && method != 2) {
        cout << "Invalid choice.\n";
        freeTree(root);
        return 0;
    }

    cout << "\nDeletion and rotation log:\n";
    root = deleteNode(root, key, method);

    cout << "\nAfter deletion:\n";
    displayChecks(root);

    freeTree(root);
    return 0;
}