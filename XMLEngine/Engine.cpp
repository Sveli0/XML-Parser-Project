#include "Engine.h"
#include "XMLParser.h"
#include <iostream>
#include <sstream>
#include <fstream>

Engine::Engine()
{
    this -> filePath = "";
    this -> root = nullptr;
};

Engine::~Engine()
{
    clearTree();
};

void Engine::clearTree()
{
    delete root;
}

void Engine::start()
{
    std::cout << "XML Parser has started. For command menu, use 'help'.";

    std::string input = "";
    bool menu = true;

    while(menu)
    {
        std::cin >> input;

        if (input == "open")
        {
            /* code */
        }
        else if (input == "close")
        {
            /* code */
        }
        else if (input == "save")
        {
            /* code */
        }
        else if (input == "saveas")
        {
            /* code */
        }
        else if (input == "help")
        {
            /* code */
        }
        else if (input == "exit")
        {
            /* code */
        }
        else std::cout << "Invalid command. For command menu, use 'help'.";
    }
}