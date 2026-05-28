#include "XMLElement.h"
#include <string>
#include <map>

class Engine
{
    private:
        std::string filePath;
        XMLElement* root;

        void clearTree();
        void printElement(XMLElement* element, int depth) const;
        XMLElement* findElementById(XMLElement* current, const std::string& targetId) const;

    public:
        Engine();
        ~Engine();

        void start();

        void open(const std::string& filePath);
        void close();
        void save();
        void saveAs(const std::string& savePath);
        void help();

        void print();
        void selectAttribute(const std::string& id, const std::string& key);
        void setAttribute(const std::string& id, const std::string& key, const std::string& value);
        void deleteAttribute(const std::string& id, const std::string& key);
        void showText(const std::string& id);
        void printChildren(const std::string& id);
        void printChild(const std::string& id, int n);
        void addNewChild(const std::string& id);
        void executeXPathQuery(const std::string& xPathQuery);
};