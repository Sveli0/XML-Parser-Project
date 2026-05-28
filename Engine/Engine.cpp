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
    std::cout << "XML Parser has started. For command menu, use 'help'." << std::endl;

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
        else std::cout << "Invalid command. For command menu, use 'help'." << std::endl;
    }
}

void Engine::open(const std::string& filePath)
{
    std::string input;
    bool menu = true;

    while (menu)
    {
        //TODO: add rest of the commands
        std::cout << "> ";
        std::getline(std::cin, input);
        std::stringstream ss(input);
        std::string command;
        ss >> command;

        if (command == "close")
        {
            /* code */
        }
        else if (command == "save")
        {
            /* code */
        }
        else if (command == "saveas")
        {
            /* code */
        }
        else if (command == "help")
        {

        }
        else std::cout << "Invalid command. For command menu, use 'help'." << std::endl;
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
    std::cout << "The following commands are supported:" << std::endl;
    std::cout << "open <file>	opens <file>" << std::endl;
    std::cout << "close			closes currently opened file" << std::endl;
    std::cout << "save			saves the currently open file" << std::endl;
    std::cout << "saveas <file>	saves the currently open file in <file>" << std::endl;
    std::cout << "help			prints this information" << std::endl;
    std::cout << "exit			exists the program" << std::endl;
}