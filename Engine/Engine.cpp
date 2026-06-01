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
                help();
            else if (command == "exit")
            {
                std::cout << "Exiting the program...";
                menu = false;
            }
            else std::cout << "Invalid command. For command menu, use 'help'." << std::endl;
        }
        else //Тази част се пуска след като е отворен някакъв файл. Разделям менюто на две, преди и след отваряне на файл.
        {
            //TODO: rest of commands
            //TODO: да опростя полетата
            //TODO: да добавя описания
            //TODO: да добавя съобщения за успех и неуспех при избор на команда
            if (command == "print")
            {
                print(root, 0);
            }
            else if (command == "select")
            {
                //TODO: no id and/or no key should fail in all functions below
                std::string id, key;
                ss >> id >> key;
                selectAttribute(id, key);
            }
            else if (command == "set")
            {
                std::string id, key, value;
                ss >> id >> key >> value;
                setAttribute(id, key, value);
            }
            else if (command == "children")
            {
                std::string id;
                ss >> id;
                printChildren(id);
            }
            else if (command == "child")
            {
                std::string id;
                int n;
                ss >> id >> n;
                printChild(id, n);
            }
            else if (command == "text")
            {
                std::string id;
                ss >> id;
                showText(id);
            }
            else if (command == "delete")
            {
                std::string id, key;
                ss >> id >> key;
                deleteAttribute(id, key);
            }
            else if (command == "newchild")
            {
                std::string id, tagName, newChildId;
                ss >> id >> tagName >> newChildId;
                addNewChild(id, tagName, newChildId);
            }
            else if (command == "xpath")
            {
                //TODO:
            }
            else if (command == "close")
            {
                close();
                fileOpen = false;
            }
            else if (command == "save")
                save();
            else if (command == "saveas")
            {
                std::string path;
                ss >> path;
                if (path.empty())
                {
                    std::cout << "Path is empty." << std::endl;
                    continue;;
                }
                saveAs(path);
            }
            else if (command == "help")
                helpExtended();

            else std::cout << "Invalid command. For command menu, use 'help'." << std::endl;
        }
    }
}


//Basic Commands Below
void Engine::open(const std::string& filePath)
{
    this -> filePath = filePath;
    root = XMLParser::parseFile(filePath);

    if (root == nullptr)
    {
        std::cout << "File " << filePath << " not found. Creating new file." << std::endl;
        root = new XMLElement("root");
        root -> setId("rootID");
        root -> setAttribute("id", "rootID");
    }
    else std::cout << "Successfully opened \"" << filePath << "\"" << std::endl;
}

void Engine::close()
{
    clearTree();
    std::cout << "Successfully closed \"" << filePath << "\"" << std::endl;
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
        std::cout << "Successfully saved " << savePath << std::endl;
        this -> filePath = savePath;
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
    //TODO: add description to special commands
    std::cout << "The following commands are supported:" << std::endl;
    std::cout << "print" << std::endl;
    std::cout << "select <id> <key>" << std::endl;
    std::cout << "set <id> <key> <value>"<< std::endl;
    std::cout << "children <id>" << std::endl;
    std::cout << "child <id> <n>" << std::endl;
    std::cout << "text <id>" << std::endl;
    std::cout << "delete <id> <key>" << std::endl;
    std::cout << "newchild <id> <newTag> <newId>" << std::endl;
    std::cout << "xpath <XPath>" << std::endl;
    std::cout << "close\t\t\tcloses currently opened file" << std::endl;
    std::cout << "save\t\t\tsaves the currently open file" << std::endl;
    std::cout << "saveas <file>\t\tsaves the currently open file in <file>" << std::endl;
    std::cout << "help\t\t\tprints this information" << std::endl;
}


//XML Unique Commands Below
void Engine::print(const XMLElement* element, const int depth)
{
    std::string tabulation(depth * 4, ' ');
    std::cout << tabulation;
    std::cout << "<" + element -> getTagName();

    for (auto attribute : element -> getAttributes())
        std::cout << " " + attribute.first + "=\"" + attribute.second + "\"";
    std::cout << ">";

    if (element -> getChildren().size() > 0)
    {
        std::cout << std::endl;

        for (XMLElement* child : element -> getChildren())
            print(child, depth + 1);

        std::cout << tabulation;
    }
    else
        std::cout << element -> getText();

    std::cout << "</" + element -> getTagName() + ">" << std::endl;
}

void Engine::selectAttribute(const std::string& id, const std::string& key)
{
    XMLElement* el = findElementById(root, id);
    if (el != nullptr)
    {
        std::string val = el -> getAttribute(key);
        if (val != "")
            std::cout << key << ": " << val << std::endl;
        else std::cout << "Attribute " << key << " does not exist for element with ID: " << id << std::endl;
    }
    else std::cout << "Element with ID: " << id << " not found." << std::endl;
}

void Engine::setAttribute(const std::string& id, const std::string& key, const std::string& value)
{
    XMLElement* el = findElementById(root, id);
    if (el != nullptr)
    {
        el -> setAttribute(key, value);
        if(key == "id")
            el -> setId(value);
    }
    else std::cout << "Element with ID: " << id << " not found." << std::endl;
}

void Engine::printChildren(const std::string& id)
{
    XMLElement* element = findElementById(root, id);

    print(element, 0);
}

void Engine::printChild(const std::string& id, const int n)
{
    XMLElement* element = findElementById(root, id);
    XMLElement* child = element -> getChildren()[n - 1];

    print(child, 0);
}

void Engine::showText(const std::string& id)
{
    XMLElement* el = findElementById(root, id);
    if (el != nullptr)
    {
        std::cout << el -> getTagName() << ": " << el -> getText() << std::endl;
    }
    else std::cout << "Element with ID: " << id << " not found." << std::endl;
}

void Engine::deleteAttribute(const std::string& id, const std::string& key)
{
    XMLElement* el = findElementById(root, id);
    if (el != nullptr)
    {
        el -> deleteAttribute(key);
    }
    else std::cout << "Element with ID: " << id << " not found." << std::endl;
}

void Engine::addNewChild(const std::string& id, const std::string& tagName, const std::string& newChildId)
{
    XMLElement* element = findElementById(root, id);
    XMLElement* child = new XMLElement(tagName);
    child -> setId(newChildId);
    child -> setAttribute("id", newChildId);
    element -> addChild(child);
}

void Engine::executeXPathQuery(const XMLElement* root, std::string& xPathQuery)
{
    if (root == nullptr || xPathQuery.empty())
        return;
    
    int linePos = xPathQuery.find("/");

    std::string currentStep;
    std::string nextStep;

    if (linePos == std::string::npos)
    {
        currentStep = xPathQuery;
        nextStep = "";
    }
    else
    {
        currentStep = xPathQuery.substr(0, linePos);
        nextStep = xPathQuery.substr(linePos + 1);
    }

    std::string tagName;
    int bracketStart = currentStep.find("[");

    if (bracketStart != std::string::npos)
        tagName = currentStep.substr(0, bracketStart);
    else
        tagName = currentStep;

    if (nextStep.empty())
    {
        int atPos = currentStep.find("@");

        if (atPos != std::string::npos)
        {
            std::string attribute = currentStep.substr(atPos + 1);

            for (XMLElement* element : root -> getChildren())
            {
                if (element -> getAttributes().count(attribute) > 0)
                {
                    std::cout << element -> getAttribute(attribute) << std::endl;
                }
            }
        }
        else if (bracketStart != std::string::npos)
        {
            int bracketEnd = currentStep.find("]");
            std::string indexStr = currentStep.substr(bracketStart + 1, bracketEnd - bracketStart - 1);
            int index = std::stoi(indexStr);

            int counter = 0;
            for (XMLElement* element : root -> getChildren())
            {
                if (element -> getTagName() == tagName)
                {
                    if (counter == index)
                    {
                        std::cout << element -> getText() << std::endl;
                        break;
                    }
                    counter++;
                }
            }
        }
        else
        {
            for (XMLElement* element : root -> getChildren())
            {
                if (element -> getTagName() == tagName)
                    std::cout << element -> getText() << std::endl;
            }
        }
        
        
        
    }
    
    
}

XMLElement* Engine::findElementById(XMLElement* current, const std::string& id)
{
    if (current == nullptr)
        return nullptr;
    if (current -> getId() == id)
        return current;
    for (XMLElement* child : current -> getChildren())
    {
        XMLElement* el = findElementById(child, id);
        if (el != nullptr)
            return el;
    }
    return nullptr;
}