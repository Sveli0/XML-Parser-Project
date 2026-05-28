#include "XMLParser.h"
#include <fstream>
#include <iostream>

XMLElement* XMLParser::parseFile(const std::string& filePath)
{
    std::ifstream file(filePath);

    if (!file)
    {
        return nullptr;
    }
    
    //TODO:
    XMLElement* parsedRoot = nullptr;

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