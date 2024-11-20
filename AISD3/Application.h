#ifndef APPLICATION_H
#define APPLICATION_H

#include "BinaryTree.h"
#include "RedBlackTree.h"
#include <iostream>
#include <limits>
#include <fstream>

typedef double number;

class Application {
public:
    Application();
    ~Application();

    void exec(BinaryTree<number>& binaryTree, RedBlackTree<number>& redBlackTree);

private:
    std::string pathToBracketTree = "C:\\LETI\\AISD\\AISD3\\AISD3\\AISD3\\bracketTree.txt";
    bool isValidTree(const std::string& str) const;
};

Application::Application() {}

Application::~Application() {}

// (8 (9 (5)) (1))
// (8 (3 (1) (6 (4) (7))) (10 (14 (13))))
// (9 (6 (3 (1 (2)) (4 (5))) (8 (7))) (17 (16 (12 (11 (10)) (14 (13) (15)))) (20 (19 (18)) (21))))
bool Application::isValidTree(const std::string& str) const
{
    std::stack<int> countInLevels;
    int unclosedBrackets = 0;
    bool isPrevUnclosedBracket = false;

    for (size_t i = 0; i < str.size(); i++) {
        char ch = str[i];
        if (std::isspace(ch)) {
            continue;
        }

        if (ch == '(') {
            if (!isPrevUnclosedBracket) {
                unclosedBrackets++;
                countInLevels.push(0);
                isPrevUnclosedBracket = true;
            }
            else {
                std::cout << "Ошибка: Две откр. скобки подряд\n";
                return false;
            }
        }

        else if (ch == ')') {
            if (unclosedBrackets <= 0) {
                std::cout << "Ошибка: Лишняя закрывающая скобка\n";
                return false;
            }
            if (countInLevels.top() == 0) {
                std::cout << "Ошибка: В скобках не содержится узел\n";
                return false;
            }
            unclosedBrackets--;
            countInLevels.pop();
            if (!countInLevels.empty()) countInLevels.top()++;
            isPrevUnclosedBracket = false;

            if (!countInLevels.empty() && countInLevels.top() >= 4) {
                std::cout << "Ошибка: У узла более 2 потомков\n";
                return false;
            }
        }

        else if (std::isdigit(ch) || (ch == '-' && (i + 1) < str.size() && std::isdigit(str[i + 1]))) {
            
            if (isPrevUnclosedBracket) {
                countInLevels.top()++;
                while (i + 1 < str.size() && std::isdigit(str[i + 1])) {
                    i++;
                }
                isPrevUnclosedBracket = false;
            }
            else {
                std::cout << "Ошибка: Число вне скобок\n";
                return false;
            }
        }

        else {
            std::cout << "Ошибка: некорректный символ '" << ch << "'\n";
            return false;
        }
    }

    if (unclosedBrackets != 0) {
        std::cout << "Ошибка: незакрытые скобки\n";
        return false;
    }

    return true;
}


void Application::exec(BinaryTree<number>& binaryTree, RedBlackTree<number>& redBlackTree)
{
    const char separator[] = "------------------------------------------------------------------------------------------------------------------------";
    const char commands[] =
        "1) Двоичное дерево\n"
        "2) КЧ-дерево\n"
        "c) Вывести список комманд\n"
        "e) Выход из программы\n";

    std::string command = "c";

    do {
        if (command == "e") {
            std::cout << "Программа завершена пользователем\n";
            break;
        }
        else if (command == "c") {
            std::cout << commands;
        }
        else if (command == "1") {
            const std::string bTreeCommands = 
                "1) Ввести скобочную запись в консоль\n"
                "2) Прочитать скобочную запись из файла\n"
                "3) Обход дерева в глубину(post-order)\n"
                "r) Изменить путь файла со скобочной записью\n"
                "h) Вывести путь к файлу по умолчанию\n"
                "s) Вывести бинарное дерево\n"
                "c) Вывести список комманд\n"
                "<) Вернуться в главное меню\n";

            command = "c";

            do {
                if (command == "c") {
                    std::cout << bTreeCommands;
                }
                else if (command == "<") {
                    std::cout << '\n';
                    std::cout << commands;
                    break;
                }
                else if (command == "s") {
                    std::cout << "Binary Tree structure:" << std::endl;
                    //binaryTree.print();
                    binaryTree.printSecond();
                }
                else if (command == "h") {
                    std::cout << "Путь к файлу по умолчанию: " << pathToBracketTree << '\n';
                }
                else if (command == "r") {
                    std::cout << "Введите абсолютный или относительный путь до файла: ";
                    std::getline(std::cin, pathToBracketTree);
                    if (!std::cin.fail()) {
                        std::cout << "Путь к файлу был изменён на: " << pathToBracketTree << '\n';
                    }
                    else {
                        std::cerr << "Путь не был перезаписан\n";
                    }
                }
                else if (command == "1") {
                    std::cout << "Введите скобочную последовательность: ";
                    std::string bracketTree;
                    std::getline(std::cin, bracketTree);
                    std::cin.ignore(1000000, '\n');
                    if (!std::cin.fail()) {
                        if (isValidTree(bracketTree)) {
                            // (8 (9 (5)) (1))
                            // (9 (6 (3 (1 (2)) (4 (5))) (8 (7))) (17 (16 (12 (11 (10)) (14 (13) (15)))) (20 (19 (18)) (21))))
                            std::cout << "Форма корректна\n";
                            binaryTree.build(bracketTree);
                            std::cout << "Дерево было построено\n";
                        }
                    }
                    else {
                        std::cerr << "Последовательность не была прочтена\n";
                    }
                }
                else if (command == "2") {
                    std::ifstream inputBracketFile(pathToBracketTree);
                    if (inputBracketFile) { 
                        std::string bracketTree;
                        std::getline(inputBracketFile, bracketTree);
                        
                        if (!inputBracketFile.fail()) {
                            if (isValidTree(bracketTree)) {
                                std::cout << "С файла была прочтена форма, она корректна\n";
                                binaryTree.build(bracketTree);
                                std::cout << "Дерево было построено\n";
                            }
                            else {
                                std::cout << "С файла была прочтена форма, она некорректна\n";
                            }
                        }
                        else {
                            std::cerr << "Ошибка: Строка не была прочтена из файла\n";
                        }

                        inputBracketFile.close();
                    }
                    else {
                        std::cerr << "Ошибка при открытии файла!\n";
                    }
                }
                else if (command == "3") {
                    if (!binaryTree.empty()) {
                        std::cout << "Обход post-order: ";
                        std::vector<number> postOrder = binaryTree.postOrder();
                        for (size_t i = 0; i < postOrder.size(); i++) {
                            std::cout << postOrder[i] << ' ';
                        }
                        std::cout << '\n';
                    }
                    else {
                        std::cout << "Дерево не содержит элементов\n";
                    }
                }

                else {
                    std::cout << "Некорректная команда. Попробуйте снова.\n";
                }

                std::cout << separator << '\n';
                std::cout << "Введите команду: ";
                std::getline(std::cin, command);

                if (std::cin.fail()) {
                    std::cin.clear();
                    //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Некорректный ввод! Попробуйте снова.\n";
                    command = "c";
                }

                std::cout << '\n';

            } while (true);
        }
        else if (command == "2") {
            const std::string RBTreeCommands =
                "1) Создать КЧ-дерево, на основе уже имеющегося бинарного дерева\n"
                "2) Обход дерева в глубину(pre-order)\n"
                "3) Обход дерева в глубину(in-order)\n"
                "4) Обход дерева в глубину(post-order)\n"
                "5) Обход дерева в ширину\n"
                "6) Поиск элемента в дереве\n"
                "7) Удалить элемент по значению\n"
                "s) Вывести бинарное дерево\n"
                "c) Вывести список комманд\n"
                "<) Вернуться в главное меню\n";

            command = "c";

            do {
                if (command == "c") {
                    std::cout << RBTreeCommands;
                }
                else if (command == "<") {
                    std::cout << '\n';
                    std::cout << commands;
                    break;
                }
                else if (command == "s") {
                    std::cout << "Red-Black Tree structure:" << std::endl;
                    //redBlackTree.print();
                    redBlackTree.printSecond();
                }
                else if (command == "1") {
                    if (!binaryTree.empty()) {
                        redBlackTree.buildTree(binaryTree.postOrder());
                        if (!redBlackTree.empty()) {
                            std::cout << "КЧ-дерево было успешно заполнено\n";
                        }
                        else {
                            std::cout << "КЧ-дерево не было заполнено\n";
                        }
                    }
                    else {
                        std::cout << "Ошибка: Нельзя создать КЧ-дерево, бинарное дерево не содержит элементов\n";
                    }
                   
                }
                else if (command == "2") {
                    if (!redBlackTree.empty()) {
                        std::cout << "Обход pre-order: ";
                        std::vector<number> preOrder = redBlackTree.preOrder();
                        for (size_t i = 0; i < preOrder.size(); i++) {
                            std::cout << preOrder[i] << ' ';
                        }
                        std::cout << '\n';
                    }
                    else {
                        std::cout << "Дерево не содержит элементов\n";
                    }
                }
                else if (command == "3") {
                    if (!redBlackTree.empty()) {
                        std::cout << "Обход in-order: ";
                        std::vector<number> inOrder = redBlackTree.inOrder();
                        for (size_t i = 0; i < inOrder.size(); i++) {
                            std::cout << inOrder[i] << ' ';
                        }
                        std::cout << '\n';
                    }
                    else {
                        std::cout << "Дерево не содержит элементов\n";
                    }
                }
                else if (command == "4") {
                    if (!redBlackTree.empty()) {
                        std::cout << "Обход post-order: ";
                        std::vector<number> postOrder = redBlackTree.postOrder();
                        for (size_t i = 0; i < postOrder.size(); i++) {
                            std::cout << postOrder[i] << ' ';
                        }
                        std::cout << '\n';
                    }
                    else {
                        std::cout << "Дерево не содержит элементов\n";
                    }
                }
                else if (command == "5") {
                    if (!redBlackTree.empty()) {
                        std::cout << "Обход в ширину: ";
                        std::vector<number> breadthFirst = redBlackTree.breadthFirstTraversal();
                        for (size_t i = 0; i < breadthFirst.size(); i++) {
                            std::cout << breadthFirst[i] << ' ';
                        }
                        std::cout << '\n';
                    }
                    else {
                        std::cout << "Дерево не содержит элементов\n";
                    }
                }
                else if (command == "6") {
                    std::cout << "Введите значение элемента: ";
                    number value;
                    std::cin >> value;
                    std::cin.ignore(1000000, '\n');
                    if (!std::cin.fail()) {
                        bool isThereElem = redBlackTree.search(value);
                        if (isThereElem) {
                            std::cout << "Элемент был успешно найден\n";
                        }
                        else {
                            std::cout << "Элемент не был найден\n";
                        }
                    }
                    else {
                        std::cerr << "Значение элемента не было прочтено\n";
                    }
                }
                else if (command == "7") {
                    std::cout << "Введите значение элемента: ";
                    number value;
                    std::cin >> value;
                    std::cin.ignore(1000000, '\n');
                    if (!std::cin.fail()) {
                        bool wasRemoved = redBlackTree.deleteNode(value);
                        if (wasRemoved) {
                            std::cout << "Элемент был успешно удалён\n";
                        }
                        else {
                            std::cout << "Элемент не был удален, так как такого элемента нет в дереве\n";
                        }
                    }
                    else {
                        std::cerr << "Значение элемента не было прочтено\n";
                    }
                }
                else {
                    std::cout << "Некорректная команда. Попробуйте снова.\n";
                }
       

                std::cout << separator << '\n';
                std::cout << "Введите команду: ";
                std::getline(std::cin, command);

                if (std::cin.fail()) {
                    std::cin.clear();
                    //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Некорректный ввод! Попробуйте снова.\n";
                    command = "c";
                }

                std::cout << '\n';

            } while (true);
        }
        else {
            std::cout << "Некорректная команда. Попробуйте снова.\n";
        }

        std::cout << separator << '\n';
        std::cout << "Введите команду: ";
        std::getline(std::cin, command);

        if (std::cin.fail()) {
            std::cin.clear();
            //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Некорректный ввод! Попробуйте снова.\n";
            command = "c";
        }

        std::cout << '\n';

    } while (true);
}

#endif // APPLICATION_H
