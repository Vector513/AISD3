#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <queue>

template <typename T>
class BinaryTree {
private:
    
    struct TreeNode {
        T value;
        TreeNode* left;
        TreeNode* right;

        TreeNode(const T& val);
    };
    
    TreeNode* root;

    void postOrderTraversal(TreeNode* node, std::vector<T>& res) const;
    void printSecond(TreeNode* root, int level = 0, bool isRight = false) const;

public:
    BinaryTree();
    ~BinaryTree();

    bool empty() const;
    void deleteTree(const TreeNode* node) const;
    int getHeight(const TreeNode* root) const;
    void build(const std::string& str);
    std::vector<T> postOrder() const;
    std::vector<T> countNodesAtEachLevel(TreeNode* root) const;
    void print() const;
    void printSecond();
};

template <typename T>
BinaryTree<T>::TreeNode::TreeNode(const T& val)
    : value(val), left(nullptr), right(nullptr) {}

template <typename T>
BinaryTree<T>::BinaryTree() : root(nullptr) {}

template <typename T>
BinaryTree<T>::~BinaryTree() 
{
    deleteTree(root);
}

template <typename T>
bool BinaryTree<T>::empty() const 
{
    return root == nullptr;
}

template <typename T>
void BinaryTree<T>::deleteTree(const TreeNode* node) const {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

template <typename T>
int BinaryTree<T>::getHeight(const TreeNode* root) const {
    if (root == nullptr) {
        return 0;
    }

    return 1 + max(getHeight(root->left), getHeight(root->right));
}

template <typename T>
void BinaryTree<T>::build(const std::string& str) 
{
    deleteTree(root);
    std::stack<TreeNode*> nodeStack;
    
    for (size_t i = 0; i < str.size(); i++) {
        char ch = str[i];
        if (std::isspace(ch)) {
            continue;
        }

        if (ch == '(') {
            continue;
        }

        if (ch == ')') {
            if (!nodeStack.empty()) {
                nodeStack.pop();
            }
        }
        
        else if (std::isdigit(str[i]) || (str[i] == '-' && (i + 1) < str.size() && std::isdigit(str[i + 1]))) {
            int start = i;
            if (str[i] == '-') {
                i++;
            }
            while (i < str.size() && std::isdigit(str[i])) {
                i++;
            }
            
            T value = std::stoi(str.substr(start, i - start));
            i--;
            TreeNode* newNode = new TreeNode(value);

            if (nodeStack.empty()) {
                root = newNode;
            }
            else {
                TreeNode* parent = nodeStack.top();
                if (!parent->left) {
                    parent->left = newNode;
                }
                else if (!parent->right) {
                    parent->right = newNode;
                }
            }
            nodeStack.push(newNode);
        }
    }
}

template <typename T>
void BinaryTree<T>::postOrderTraversal(TreeNode* root, std::vector<T>& res) const {
    if (!root) return ;
    postOrderTraversal(root->left, res);
    postOrderTraversal(root->right, res);
    res.push_back(root->value);
}

template <typename T>
std::vector<T> BinaryTree<T>::postOrder() const {
    std::vector<T> res;
    postOrderTraversal(root, res);
    return res;
}

template <typename T>
std::vector<T> BinaryTree<T>::countNodesAtEachLevel(TreeNode* root) const 
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
void BinaryTree<T>::print() const {
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
void BinaryTree<T>::printSecond(TreeNode* root, int level, bool isRight) const
{
    if (root == NULL) return;
    printSecond(root->right, level + 1, true);

    if (!level) std::cout << "-->"; // марафет для корня дерева
    else std::cout << "   ";

    for (int i = 0; i < level; i++) {
        if (i + 1 == level) isRight ? std::cout << ".-->" : std::cout << "`-->";
        else std::cout << "    ";
    }

    std::cout << root->value << "\n";

    printSecond(root->left, level + 1);
}

template <typename T>
void BinaryTree<T>::printSecond()
{
    printSecond(root, 0, false);
}

#endif // BINARYTREE_H
