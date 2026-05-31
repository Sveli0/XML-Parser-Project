#include "XMLParser.h"
#include <iostream>
#include <fstream>
#include <sstream>

XMLElement* XMLParser::parseFile(const std::string& filePath)
{
    std::ifstream file(filePath);

    if (!file)
    {
        return nullptr;
    }
    
    XMLElement* parsedRoot;
    std::string line;
    std::string word;

    if (std::getline(file, line))
    {
        std::stringstream ss(line);
        ss >> word;
        word = word.substr(1);

        if (word.back() == '>') word.pop_back();

        parsedRoot = new XMLElement(word);

        while (ss >> word)
        {
            std::string key, value;
            if (word.back() == '>') word.pop_back();
            key = word.substr(0, word.find('='));
            value = word.substr(word.find('=') + 2);
            value.pop_back();
            parsedRoot -> setAttribute(key, value);
        }

        std::unordered_set<std::string> ids;
        int autoIdCounter = 1;

        setInitialID(parsedRoot, ids, autoIdCounter);
    }
    

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
    }
    
    

    file.close();
    return parsedRoot;
}

bool XMLParser::saveToFile(const std::string& path, XMLElement* root)
{
    std::ofstream file(path);

    if (!file)
    {
        return false;
    }
    
    if (root != nullptr)
    {
        //TODO: Запиши таговете от дървото във файла
    }
    
    file.close();
    return true;
}

void setInitialID(XMLElement* element, std::unordered_set<std::string>& ids, int& autoIdCounter)
{
    std::string elId = element -> getAttribute("id");
    bool duplicateId;

    if (elId == "") element -> setId("auto_" + std::to_string(autoIdCounter++));
    else
    {
        if (ids.find(elId) == ids.end())
        {
            element -> setId(elId);
        }
        else
        {
            elId = elId + "_";
        }
        ids.insert(elId);
    }
}