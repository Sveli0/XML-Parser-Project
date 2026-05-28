#include "../XMLElement/XMLElement.h"
#include <string>

class XMLParser
{
    static XMLElement* parseFile(const std::string& filePath);
    static bool saveToFile(const std::string& path, XMLElement* root);
};