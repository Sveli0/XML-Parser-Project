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
    bool fileOpen = false;

    while(menu)
    {
        std::cout << "> ";
        std::getline(std::cin, input);
        std::stringstream ss(input);
        std::string command;
        ss >> command;

        if (!fileOpen) //Тази част с по-кратко меню се пуска само ако не е отворен файл.
        {
            if (command == "open")
            {
                std::string path;
                ss >> path;
                if (path.empty())
                {
                    std::cout << "Path is empty." << std::endl;
                    continue;
                }
                open(path);
                fileOpen = true;
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
        else //Тази част се пуска след като е отворен някакъв файл. Разделям менюто на две, преди и след отваряне на файл.
        {
            if (command == "close")
            {
                close();
                fileOpen = false;
            }
            else if (command == "save")
            {
                save();
            }
            else if (command == "saveas")
            {
                std::string path;
                ss >> path;
                if (path.empty())
                {
                    std::cout << "Path is empty.";
                    break;
                }
                saveAs(path);
            }
            else if (command == "help")
            {
                helpExtended();
            }
            else std::cout << "Invalid command. For command menu, use 'help'." << std::endl;
        }
    }
}

void Engine::open(const std::string& filePath)
{
    clearTree();
    this -> filePath = filePath;
    root = XMLParser::parseFile(filePath);

    if (root == nullptr)
    {
        std::cout << "File " << filePath << " not found. Creating new file." << std::endl;
        root = new XMLElement("");
    }
    else std::cout << "Successfully opened " << filePath << std::endl;
}

void Engine::close()
{
    std::cout << "Successfully closed " << filePath << std::endl;
    clearTree();
    filePath = "";
}

void Engine::save()
{
    if (XMLParser::saveToFile(filePath, root))
    {
        std::cout << "Successfully saved " << filePath << std::endl;
    }
    else std::cout << "Something went wrong. File couldn't be saved." << std::endl;
}

void Engine::saveAs(const std::string& savePath)
{
    if (XMLParser::saveToFile(savePath, root))
    {
        std::cout << "Successfully saved " << filePath << std::endl;
    }
    else std::cout << "Something went wrong. File couldn't be saved." << std::endl;
}

void Engine::help()
{
    std::cout << "The following commands are supported:" << std::endl;
    std::cout << "open <file>\t\topens <file>" << std::endl;
    std::cout << "help\t\t\tprints this information" << std::endl;
    std::cout << "exit\t\t\texists the program" << std::endl;
}

void Engine::helpExtended()
{
    std::cout << "The following commands are supported:" << std::endl;
    std::cout << "close\t\t\tcloses currently opened file" << std::endl;
    std::cout << "save\t\t\tsaves the currently open file" << std::endl;
    std::cout << "saveas <file>\t\tsaves the currently open file in <file>" << std::endl;
    std::cout << "help\t\t\tprints this information" << std::endl;
}