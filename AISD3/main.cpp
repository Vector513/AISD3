#include <Windows.h>
#include "Application.h"

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    
    Application app;
    BinaryTree<number> binaryTree;
    RedBlackTree<number> redBlackTree;
    app.exec(binaryTree, redBlackTree);

    return 0;
}
