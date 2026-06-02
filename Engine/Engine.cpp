#include "Engine.h"
#include "../XMLParser/XMLParser.h"
#include <iostream>
#include <sstream>

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
                    std::cout << "Path cannot be empty!" << std::endl;
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
            std::string id, key;
            if (command == "print")
            {
                print(root, 0);
            }
            else if (command == "select")
            {
                ss >> id >> key;
                if (id.empty() || key.empty())
                    std::cout << "'ID' and/or 'key' fields cannot be empty!" << std::endl;
                else selectAttribute(id, key);
            }
            else if (command == "set")
            {
                std::string value;
                ss >> id >> key >> value;
                if (id.empty() || key.empty() || value.empty())
                    std::cout << "'ID', 'key' and/or 'value' fields cannot be empty!" << std::endl;
                else setAttribute(id, key, value);
            }
            else if (command == "children")
            {
                ss >> id;
                if (id.empty())
                    std::cout << "'ID' cannot be empty!" << std::endl;
                else printChildren(id);
            }
            else if (command == "child")
            {
                int n = -1;
                ss >> id >> n;
                if (id.empty() || n < 1)
                    std::cout << "'ID' cannot be empty and/or <n> cannot be empty or negative!" << std::endl;
                else printChild(id, n);
            }
            else if (command == "text")
            {
                ss >> id;
                if (id.empty())
                    std::cout << "'ID' cannot be empty!" << std::endl;
                else showText(id);
            }
            else if (command == "delete")
            {
                ss >> id >> key;
                if (id.empty() || key.empty())
                    std::cout << "'ID' and/or 'key' fields cannot be empty!" << std::endl;
                else deleteAttribute(id, key);
            }
            else if (command == "newchild")
            {
                std::string tagName, newChildId;
                ss >> id >> tagName >> newChildId;
                if (id.empty() || tagName.empty() || newChildId.empty())
                    std::cout << "'ID', 'newTag' and/or 'newId' fields cannot be empty!" << std::endl;
                else addNewChild(id, tagName, newChildId);
            }
            else if (command == "xpath")
            {
                std::string query;
                ss >> query;
                if (query.empty())
                    std::cout << "Query cannot be empty!" << std::endl;
                else executeXPathQuery(root, query);
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
                    std::cout << "Path cannot be empty!" << std::endl;
                else saveAs(path);
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
    std::cout << "The following commands are supported:" << std::endl;
    std::cout << "print\t\t\t\t\tprints the file" << std::endl;
    std::cout << "select <id> <key>\t\t\tprints attribute <key> of element with <id>" << std::endl;
    std::cout << "set <id> <key> <value>\t\t\tsets the value of attribute <key> for element with <id>"<< std::endl;
    std::cout << "children <id>\t\t\t\tprints childeren of element with <id>" << std::endl;
    std::cout << "child <id> <n>\t\t\t\tprints ancestor <n> of element with <id>" << std::endl;
    std::cout << "text <id>\t\t\t\tshows text of element with <id>" << std::endl;
    std::cout << "delete <id> <key>\t\t\tdeletes attribute <key> of element with <id>" << std::endl;
    std::cout << "newchild <id> <newTag> <newId>\t\tcreates a new child for parent with <id>" << std::endl;
    std::cout << "xpath <XPath>\t\t\t\textracts an xpath query" << std::endl;
    std::cout << "close\t\t\t\t\tcloses currently opened file" << std::endl;
    std::cout << "save\t\t\t\t\tsaves the currently open file" << std::endl;
    std::cout << "saveas <file>\t\t\t\tsaves the currently open file in <file>" << std::endl;
    std::cout << "help\t\t\t\t\tprints this information" << std::endl;
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
    XMLElement* element = findElementById(root, id);
    
    if (element != nullptr)
    {
        if(key == "id")
        {
            XMLElement* duplicate = findElementById(root, value);
            if (duplicate != nullptr)
            {
                std::cout << "Cannot set attribute <id> to " << value << " because it isn't unique!" << std::endl;
                return;
            }
            element -> setId(value);
        }
        element -> setAttribute(key, value);
        std::cout << "Attribute successfully set!" << std::endl;
    }
    else std::cout << "Element with ID: " << id << " not found." << std::endl;
}

void Engine::printChildren(const std::string& id)
{
    XMLElement* element = findElementById(root, id);

    if (element != nullptr)
        print(element, 0);
    else std::cout << "Element with ID: " << id << " not found." << std::endl;
}

void Engine::printChild(const std::string& id, const int n)
{
    XMLElement* element = findElementById(root, id);
    XMLElement* child = nullptr;

    if (element != nullptr)
    {
        if (n <= element -> getChildren().size())
        {
            child = element -> getChildren()[n - 1];
                print(child, 0);
        }
        else std::cout << "Element with ID: " << id << " does not have a descendant number <n>." << std::endl;
    }
    else std::cout << "Element with ID: " << id << " not found." << std::endl;
}

void Engine::showText(const std::string& id)
{
    XMLElement* element = findElementById(root, id);
    if (element != nullptr)
    {
        std::cout << element -> getTagName() << ": " << element -> getText() << std::endl;
        std::cout << "Do you want to change the text? 'Y/N'" << std::endl << "> ";
        std::string input;
        while (true)
        {
            std::getline(std::cin, input);
            if (input == "Y")
            {
                std::cout << "Enter new text:" << std::endl << "> ";
                std::getline(std::cin, input);
                element -> setText(input);
                std::cout << "Successfully changed the text!" << std::endl;
                return;
            }
            else if (input == "N")
                return;
            else std::cout << "Wrong input!" << std::endl << "> ";
        }       
    }
    else std::cout << "Element with ID: " << id << " not found." << std::endl;
}

void Engine::deleteAttribute(const std::string& id, const std::string& key)
{
    XMLElement* el = findElementById(root, id);
    if (el != nullptr)
    {
        if (el -> getAttribute(key) != "")
        {
            el -> deleteAttribute(key);
            std::cout << "Successfully deleted attribute!" << std::endl;
        }
        else std::cout << "Attribute " << key << " does not exist for element with ID: " << id << std::endl;
    }
    else std::cout << "Element with ID: " << id << " not found." << std::endl;
}

void Engine::addNewChild(const std::string& id, const std::string& tagName, const std::string& newChildId)
{
    XMLElement* duplicate = findElementById(root, newChildId);
    if (duplicate != nullptr)
    {
        std::cout << "Cannot set <id> to " << newChildId << " because it isn't unique!" << std::endl;
        return;
    }

    XMLElement* element = findElementById(root, id);
    if (element != nullptr)
    {
        XMLElement* child = new XMLElement(tagName);
        child -> setId(newChildId);
        child -> setAttribute("id", newChildId);
        element -> addChild(child);
        std::cout << "Successfully added new child!" << std::endl;
    }
    else std::cout << "Element with ID: " << id << " not found." << std::endl;
}

void Engine::executeXPathQuery(XMLElement* root, std::string xPathQuery)
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
            if (attribute.back() == ']')
                attribute.pop_back();

            for (XMLElement* child : root -> getChildren())
            {
                if (child -> getAttributes().count(attribute) > 0)
                    std::cout << child -> getAttribute(attribute) << std::endl;
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
    else
    {
        int equalPos = currentStep.find("=");

        if (bracketStart != std::string::npos && equalPos != std::string::npos)
        {
            int bracketEnd = currentStep.find("]");
            std::string tag = currentStep.substr(bracketStart + 1, equalPos - bracketStart - 1);
            std::string value = currentStep.substr(equalPos + 1, bracketEnd - equalPos - 1);

            if (!value.empty() && value.front() == '"')
                value.erase(0, 1);
            if (!value.empty() && value.back() == '"')
                value.pop_back();

            bool isFilter = (tag.front() == '@');

            for(XMLElement* element : root -> getChildren())
            {
                if (element -> getTagName() == tagName)
                {
                    bool conditionMet = false;

                    if (isFilter)
                    {
                        std::string attributeName = tag.substr(1);

                        if (element -> getAttributes().count(attributeName) > 0 && element -> getAttribute(attributeName) == value)
                            conditionMet = true;
                    }
                    else
                    {
                        for(XMLElement* elChild : element -> getChildren())
                        {
                            if (elChild -> getTagName() == tag && elChild -> getText() == value)
                            {
                                conditionMet = true;
                                break;
                            }
                        }   
                    }

                    if (conditionMet)
                        executeXPathQuery(element, nextStep);
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
                        executeXPathQuery(element, nextStep);
                        break;
                    }
                    counter++;
                }
            }
        }
        else
        {
            int atPos = currentStep.find("@");

            bool hasFilter = (bracketStart != std::string::npos && atPos != std::string::npos);
            std::string filter = "";

            if (hasFilter)
            {
                filter = currentStep.substr(atPos + 1);
                filter.pop_back();
            }

            for (XMLElement* element : root -> getChildren())
            {
                if (element -> getTagName() == tagName)
                {
                    if (hasFilter && element -> getAttributes().count(filter) == 0)
                        continue;
                    
                    executeXPathQuery(element, nextStep);
                }
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