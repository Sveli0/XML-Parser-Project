#include "XMLParser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <vector>

void setInitialID(XMLElement* element, std::vector<std::string>& ids, int& autoIdCounter);
XMLElement* parseParent(std::string& line, std::vector<std::string>& ids, int& autoIdCounter);
void parseAttributes(XMLElement* element, std::stringstream& ss);

XMLElement* XMLParser::parseFile(const std::string& filePath)
{
    std::ifstream file(filePath);

    if (!file)
        return nullptr;

    XMLElement* parsedRoot;
    std::string line;
    std::string tagName;
    std::vector<std::string> ids;
    int autoIdCounter = 1;

    if (std::getline(file, line))
    {
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        parsedRoot = parseParent(line, ids, autoIdCounter);
    }

    std::stack <XMLElement*> parentsStack;
    parentsStack.push(parsedRoot);

    while (std::getline(file, line))
    {
        line.erase(0, line.find_first_not_of(" \t\r\n"));

        if (line.empty())
            continue;
        else if (line.substr(0, 2) == "</")
        {
            if (!parentsStack.empty())
                parentsStack.pop();
        }
        else if (line.substr(0, 1) == "<" && line.find("</") != std::string::npos)
        {
            int firstCloseBracket = line.find('>');
            std::string openingTag = line.substr(1, firstCloseBracket - 1);
            std::stringstream openingTagStream(openingTag);
                
            std::string tagName;
            openingTagStream >> tagName;
            XMLElement* child = new XMLElement(tagName);

            int startOfClosingTag = line.find("</");

            if (startOfClosingTag - firstCloseBracket > 1)
            {
                int textStart = firstCloseBracket + 1;
                int textLength = startOfClosingTag - textStart;
                    
                std::string text = line.substr(textStart, textLength);
                child -> setText(text);
            }
            
            parseAttributes(child, openingTagStream);
            setInitialID(child, ids, autoIdCounter);

            if (!parentsStack.empty())
                parentsStack.top() -> addChild(child);
        }
        else if(line.substr(0, 1) == "<")
        {
            XMLElement* child = parseParent(line, ids, autoIdCounter);

            if (!parentsStack.empty())
                parentsStack.top() -> addChild(child);

            parentsStack.push(child);
        }
        //else throw exception
    }

    file.close();
    return parsedRoot;
}

bool XMLParser::saveToFile(const std::string& path, XMLElement* root)
{
    std::ofstream file(path);

    if (!file)
        return false;
    
    if (root != nullptr)
    {
        //TODO: Запиши таговете от дървото във файла
    }
    
    file.close();
    return true;
}

void setInitialID(XMLElement* element, std::vector<std::string>& ids, int& autoIdCounter)
{
    std::string elId = element -> getAttribute("id");

    if (elId == "")
    {
        elId = "auto_" + std::to_string(autoIdCounter++);
        element -> setId(elId);
        element -> setAttribute("id", elId);
    }
    else
    {
        int duplicateCounter = 0;
        for (const std::string& id : ids)
            if (id == elId)
                duplicateCounter++;
        if (duplicateCounter == 0)
            element -> setId(elId);
        else
        {
            elId += "_" + std::to_string(duplicateCounter);
            element -> setId(elId);
        }
        ids.push_back(elId);
    }
}

XMLElement* parseParent(std::string& line, std::vector<std::string>& ids, int& autoIdCounter)
{
    std::stringstream ss(line);
    std::string tagName;
    ss >> tagName;
    tagName = tagName.substr(1);

    if (tagName.back() == '>')
        tagName.pop_back();

    XMLElement* element = new XMLElement(tagName);

    parseAttributes(element, ss);
    setInitialID(element, ids, autoIdCounter);

    return element;
}

void parseAttributes(XMLElement* element, std::stringstream& ss)
{
    std::string word;
    while (ss >> word)
    {
        if (word.back() == '>')
            word.pop_back();

        std::string key, value;
        key = word.substr(0, word.find('='));
        value = word.substr(word.find('=') + 2);
        value.pop_back();

        element -> setAttribute(key, value);
    }
}