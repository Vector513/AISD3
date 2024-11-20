#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

#include "BinaryTree.h"

template <typename T>
class RedBlackTree {
private:
    enum Color { RED, BLACK };

    struct TreeNode {
        T value;
        TreeNode* left;
        TreeNode* right;
        TreeNode* parent;
        Color color;

        TreeNode(const T& value);
    };

    TreeNode* root;

    void rotateLeft(TreeNode* x);
    void rotateRight(TreeNode* x);
    void fixInsert(TreeNode* TreeNode);
    void printSecond(TreeNode* root, int level = 0, bool isRight = false) const;

public:
    RedBlackTree();
    RedBlackTree(const std::vector<T>& data);
    ~RedBlackTree();

    bool empty() const;
    void deleteTree(const TreeNode* node) const;
    void buildTree(const std::vector<T>& data);
    void insert(const T& value);
    std::vector<T> inOrder() const;
    std::vector<T> preOrder() const;
    std::vector<T> postOrder() const;
    std::vector<T> breadthFirstTraversal() const;
    TreeNode* search(const T& value) const;
    bool deleteNode(const T& value);
    void transplant(TreeNode* u, TreeNode* v);
    void fixDelete(TreeNode* x);
    int getHeight(const TreeNode* root) const;
    std::vector<T> countNodesAtEachLevel(TreeNode* root) const;
    void print() const;
    void printSecond();
};

template <typename T>
RedBlackTree<T>::TreeNode::TreeNode(const T& value)
    : value(value), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}

template <typename T>
RedBlackTree<T>::RedBlackTree() : root(nullptr) {}

template <typename T>
RedBlackTree<T>::RedBlackTree(const std::vector<T>& data) 
{
    for (auto it = data.rbegin(); it != data.rend(); ++it) {
        insert(*it);
    }
}

template<typename T>
inline RedBlackTree<T>::~RedBlackTree()
{
    deleteTree(root);
}

template <typename T>
bool RedBlackTree<T>::empty() const
{
    return root == nullptr;
}

template <typename T>
void RedBlackTree<T>::deleteTree(const TreeNode* node) const {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

template <typename T>
void RedBlackTree<T>::buildTree(const std::vector<T>& data) 
{
    deleteTree(root);

    for (auto it = data.rbegin(); it != data.rend(); ++it) {
        insert(*it);
    }
}

template <typename T>
void RedBlackTree<T>::rotateLeft(TreeNode* pivotNode) 
{
    TreeNode* newParent = pivotNode->right;
    pivotNode->right = newParent->left;

    if (newParent->left) newParent->left->parent = pivotNode;

    newParent->parent = pivotNode->parent;

    if (!pivotNode->parent) // Если pivotNode был корнем
        root = newParent;   // Новый корень — это newParent
    else if (pivotNode == pivotNode->parent->left) // Если pivotNode был левым ребёнком
        pivotNode->parent->left = newParent; // Новый левый ребёнок — это newParent
    else
        pivotNode->parent->right = newParent; // Новый правый ребёнок — это newParent

    newParent->left = pivotNode; // pivotNode становится левым ребёнком newParent
    pivotNode->parent = newParent; // Обновляем родителя pivotNode на newParent
}


template <typename T>
void RedBlackTree<T>::rotateRight(TreeNode* pivotNode) {
    TreeNode* leftChild = pivotNode->left; // Левый потомок pivotNode
    pivotNode->left = leftChild->right;    // Переместить правое поддерево leftChild на место левого поддерева pivotNode

    if (leftChild->right) {
        leftChild->right->parent = pivotNode; // Обновить родителя правого поддерева leftChild
    }

    leftChild->parent = pivotNode->parent; // Установить нового родителя для leftChild

    // Если pivotNode является корнем дерева
    if (!pivotNode->parent) {
        root = leftChild;
    }
    // Если pivotNode был правым потомком своего родителя
    else if (pivotNode == pivotNode->parent->right) {
        pivotNode->parent->right = leftChild;
    }
    // Если pivotNode был левым потомком своего родителя
    else {
        pivotNode->parent->left = leftChild;
    }

    leftChild->right = pivotNode; // Установить pivotNode в качестве правого потомка leftChild
    pivotNode->parent = leftChild; // Обновить родителя pivotNode
}

template <typename T>
void RedBlackTree<T>::fixInsert(TreeNode* currentNode) 
{
    while (currentNode != root && currentNode->parent->color == RED) {
        TreeNode* parentNode = currentNode->parent;
        TreeNode* grandparentNode = parentNode->parent;

        if (parentNode == grandparentNode->left) {
            TreeNode* uncleNode = grandparentNode->right;

            if (uncleNode && uncleNode->color == RED) {
                parentNode->color = BLACK;
                uncleNode->color = BLACK;
                grandparentNode->color = RED;
                currentNode = grandparentNode;
            }
            else {
                if (currentNode == parentNode->right) {
                    currentNode = parentNode;
                    rotateLeft(currentNode);
                }

                parentNode->color = BLACK;
                grandparentNode->color = RED;
                rotateRight(grandparentNode);
            }
        }

        else {
            TreeNode* uncleNode = grandparentNode->left;

            if (uncleNode && uncleNode->color == RED) {
                parentNode->color = BLACK;
                uncleNode->color = BLACK;
                grandparentNode->color = RED;
                currentNode = grandparentNode;
            }
            else {
                if (currentNode == parentNode->left) {
                    currentNode = parentNode;
                    rotateRight(currentNode);
                }
                parentNode->color = BLACK;
                grandparentNode->color = RED;
                rotateLeft(grandparentNode);
            }
        }
    }

    root->color = BLACK;
}

template <typename T>
void RedBlackTree<T>::insert(const T& value) 
{
    TreeNode* newNode = new TreeNode(value);
    if (!root) {
        root = newNode;
        root->color = BLACK;
        return;
    }

    TreeNode* current = root;
    TreeNode* parent = nullptr;

    while (current) {
        parent = current;
        if (value < current->value)
            current = current->left;
        else
            current = current->right;
    }

    newNode->parent = parent;
    if (value < parent->value)
        parent->left = newNode;
    else
        parent->right = newNode;

    fixInsert(newNode);
}

template <typename T>
std::vector<T> RedBlackTree<T>::inOrder() const 
{
    std::vector<T> res;
    std::stack<TreeNode*> stack;
    TreeNode* current = root;

    while (current != nullptr || !stack.empty()) {
        while (current != nullptr) {
            stack.push(current);
            current = current->left;
        }

        current = stack.top();
        stack.pop();

        res.push_back(current->value);

        current = current->right;
    }

    return res;
}

template <typename T>
std::vector<T> RedBlackTree<T>::preOrder() const 
{
    std::vector<T> res;
    if (root == nullptr) return res;

    std::stack<TreeNode*> stack;
    stack.push(root);

    while (!stack.empty()) {
        TreeNode* current = stack.top();
        stack.pop();

        res.push_back(current->value);

        if (current->right != nullptr)
            stack.push(current->right);
        if (current->left != nullptr)
            stack.push(current->left);
    }
    return res;
}

template <typename T>
std::vector<T> RedBlackTree<T>::postOrder() const 
{
    std::vector<T> res;
    if (root == nullptr) return res;

    std::stack<TreeNode*> stack1, stack2;
    stack1.push(root);

    while (!stack1.empty()) {
        TreeNode* current = stack1.top();
        stack1.pop();
        stack2.push(current);

        if (current->left != nullptr)
            stack1.push(current->left);
        if (current->right != nullptr)
            stack1.push(current->right);
    }

    while (!stack2.empty()) {
        TreeNode* current = stack2.top();
        stack2.pop();
        res.push_back(current->value);
    }
    return res;
}

template <typename T>
std::vector<T> RedBlackTree<T>::breadthFirstTraversal() const 
{
    std::vector<T> res;
    if (!root) {
        return res;
    }

    std::queue<TreeNode*> nodeQueue;
    nodeQueue.push(root);

    while (!nodeQueue.empty()) {
        TreeNode* currentNode = nodeQueue.front();
        nodeQueue.pop();

        res.push_back(currentNode->value);

        if (currentNode->left) {
            nodeQueue.push(currentNode->left);
        }
        if (currentNode->right) {
            nodeQueue.push(currentNode->right);
        }
    }

    return res;
}

template <typename T>
typename RedBlackTree<T>::TreeNode* RedBlackTree<T>::search(const T& value) const
{
    TreeNode* node = root;
    while (node) {
        if (value == node->value) {
            return node;
        }
        if (value < node->value) {
            node = node->left;
        }
        else {
            node = node->right;
        }
    }
    return nullptr;
}

template <typename T>
bool RedBlackTree<T>::deleteNode(const T& value) {
    TreeNode* nodeToDelete = search(value);
    if (!nodeToDelete)
        return false;

    TreeNode* y = nodeToDelete;
    TreeNode* x = nullptr;
    TreeNode* xParent = nullptr;
    bool yOriginalColor = y->color;

    if (!nodeToDelete->left) {
        x = nodeToDelete->right;
        transplant(nodeToDelete, nodeToDelete->right);
    }
    else if (!nodeToDelete->right) {
        x = nodeToDelete->left;
        transplant(nodeToDelete, nodeToDelete->left);
    }
    else {
        TreeNode* successor = nodeToDelete->right;
        while (successor->left) {
            successor = successor->left;
        }

        y = successor;
        yOriginalColor = y->color;
        x = y->right;

        if (y->parent == nodeToDelete) {
            if (x) x->parent = y;
        }
        else {
            transplant(y, y->right);
            y->right = nodeToDelete->right;
            y->right->parent = y;
        }

        transplant(nodeToDelete, y);
        y->left = nodeToDelete->left;
        y->left->parent = y;
        y->color = nodeToDelete->color;
    }

    delete nodeToDelete;

    if (yOriginalColor == BLACK && x) {
        fixDelete(x);
    }

    return true;
}


template <typename T>
void RedBlackTree<T>::transplant(TreeNode* u, TreeNode* v) {
    if (!u->parent)
        root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;

    if (v)
        v->parent = u->parent;
}

template <typename T>
void RedBlackTree<T>::fixDelete(TreeNode* x) {
    while (x != root && x->color == BLACK) {
        if (x == x->parent->left) {
            TreeNode* sibling = x->parent->right;
            if (sibling->color == RED) {
                sibling->color = BLACK;
                x->parent->color = RED;
                rotateLeft(x->parent);
                sibling = x->parent->right;
            }

            if ((!sibling->left || sibling->left->color == BLACK) &&
                (!sibling->right || sibling->right->color == BLACK)) {
                sibling->color = RED;
                x = x->parent;
            }
            else {
                if (!sibling->right || sibling->right->color == BLACK) {
                    if (sibling->left) sibling->left->color = BLACK;
                    sibling->color = RED;
                    rotateRight(sibling);
                    sibling = x->parent->right;
                }
                sibling->color = x->parent->color;
                x->parent->color = BLACK;
                if (sibling->right) sibling->right->color = BLACK;
                rotateLeft(x->parent);
                x = root;
            }
        }
        else {
            TreeNode* sibling = x->parent->left;
            if (sibling->color == RED) {
                sibling->color = BLACK;
                x->parent->color = RED;
                rotateRight(x->parent);
                sibling = x->parent->left;
            }

            if ((!sibling->right || sibling->right->color == BLACK) &&
                (!sibling->left || sibling->left->color == BLACK)) {
                sibling->color = RED;
                x = x->parent;
            }
            else {
                if (!sibling->left || sibling->left->color == BLACK) {
                    if (sibling->right) sibling->right->color = BLACK;
                    sibling->color = RED;
                    rotateLeft(sibling);
                    sibling = x->parent->left;
                }
                sibling->color = x->parent->color;
                x->parent->color = BLACK;
                if (sibling->left) sibling->left->color = BLACK;
                rotateRight(x->parent);
                x = root;
            }
        }
    }

    x->color = BLACK;
}

template <typename T>
int RedBlackTree<T>::getHeight(const TreeNode* root) const {
    if (root == nullptr) {
        return 0;
    }

    return 1 + max(getHeight(root->left), getHeight(root->right));
}

template <typename T>
std::vector<T> RedBlackTree<T>::countNodesAtEachLevel(TreeNode* root) const
{
    std::vector<T> result;

    if (!root) return result;

    std::queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        int levelSize = q.size();
        result.push_back(levelSize);

        for (int i = 0; i < levelSize; ++i) {
            TreeNode* node = q.front();
            q.pop();

            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
    }

    return result;
}

template <typename T>
void RedBlackTree<T>::print() const {
    if (root == nullptr) {
        return;
    }

    int minSpace = 2;
    int countSpace = minSpace * std::pow(2, getHeight(root) - 2);

    std::string connector(countSpace, '-');
    std::string emptySpace(countSpace, ' ');

    std::vector<T> transferPoints = countNodesAtEachLevel(root);
    int transferIndex = 0;
    int currentIndex = 0;

    std::queue<TreeNode*> unprinted;
    std::queue<int> places{};
    places.push(1);

    int lastPos = 0;
    unprinted.push(root);
    while (!unprinted.empty())
    {
        TreeNode* current = unprinted.front();
        currentIndex++;
        unprinted.pop();

        std::string emptySpace(countSpace * (4 * (places.front() - lastPos) - 2) - (lastPos == 0 ? countSpace : 0), ' '); // 2 6 11 14-15 18-19
        std::string microSpace(countSpace, ' ');
        std::string connector(countSpace, '-');

        std::cout << emptySpace;
        if (current->left != nullptr) {
            std::cout << connector;
            unprinted.push(current->left);
            places.push(places.front() * 2 - 1);
        }
        else {
            std::cout << microSpace;
        }
        std::cout << current->value;
        if (current->right != nullptr) {
            std::cout << connector;
            unprinted.push(current->right);
            places.push(places.front() * 2);
        }
        else {
            std::cout << microSpace;
        }

        lastPos = places.front();
        places.pop();

        if (transferIndex < transferPoints.size() && currentIndex >= transferPoints[transferIndex]) {
            transferIndex++;
            currentIndex = 0;
            lastPos = 0;
            countSpace = max(countSpace / 2, 1);
            std::cout << '\n';
        }
    }
}

template <typename T>
void RedBlackTree<T>::printSecond(TreeNode* root, int level, bool isRight) const
{
    if (root == NULL) return;
    printSecond(root->right, level + 1, true);

    if (!level) std::cout << "-->";
    else std::cout << "   ";

    for (int i = 0; i < level; i++) {
        if (i + 1 == level) isRight ? std::cout << ".-->" : std::cout << "`-->";
        else std::cout << "    ";
    }

    std::cout << root->value << "\n";

    printSecond(root->left, level + 1);
}

template <typename T>
void RedBlackTree<T>::printSecond() 
{
    printSecond(root, 0, false);
}

#endif // BINARYTREE_H
