#include "../XMLElement/XMLElement.h"
#include <string>

class Engine
{
    private:
        std::string filePath;
        XMLElement* root;

        void clearTree();
        void printElement(XMLElement* element, int depth);
        XMLElement* findElementById(XMLElement* current, const std::string& id);

        void open(const std::string& filePath);
        void close();
        void save();
        void saveAs(const std::string& savePath);
        void help();
        void helpExtended();

        void print(const XMLElement*, const int depth);
        void selectAttribute(const std::string& id, const std::string& key);
        void setAttribute(const std::string& id, const std::string& key, const std::string& value);
        void deleteAttribute(const std::string& id, const std::string& key);
        void showText(const std::string& id);
        void printChildren(const std::string& id);
        void printChild(const std::string& id, const int n);
        void addNewChild(const std::string& id, const std::string& tagName, const std::string& newChildId);
        void executeXPathQuery(const XMLElement* root, std::string& xPathQuery);

    public:
        Engine();
        ~Engine();

        void start();
};