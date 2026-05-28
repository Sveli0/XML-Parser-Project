#include "Engine.h"
#include "../XMLParser/XMLParser.h"
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

    std::string input;
    bool menu = true;

    while(menu)
    {
        std::cout << "> ";
        std::getline(std::cin, input);
        std::stringstream ss(input);
        std::string command;
        ss >> command;

        if (command == "open")
        {
            std::string path;
            ss >> path;
            if (path.empty())
            {
                std::cout << "Path is empty.";
                break;
            }
            open(path);
        }
        else if (command == "help")
        {
            help();
        }
        else if (command == "exit")
        {
            std::cout << "Exiting the program...";
            menu = false;
        }
        else std::cout << "Invalid command. For command menu, use 'help'.";
    }
}

void Engine::open(const std::string& filePath)
{
    //TODO: add more commands, fix menu
    std::string input = "";
    if (input == "close")
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
        
}

void Engine::close()
{

}

void Engine::save()
{

}

void Engine::saveAs(const std::string& savePath)
{

}

void Engine::help()
{

}