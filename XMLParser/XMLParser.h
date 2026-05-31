#include "../XMLElement/XMLElement.h"
#include <string>
#include <unordered_set>

class XMLParser
{
    private:
        void setInitialID(XMLElement* element);
        std::unordered_set<std::string> ids;
        int autoIdCounter = 1;

    public:
        static XMLElement* parseFile(const std::string& filePath);
        static bool saveToFile(const std::string& path, XMLElement* root);

};