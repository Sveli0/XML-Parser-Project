#ifndef XML_ELEMENT_H
#define XML_ELEMENT_H

#include <string>
#include <vector>
#include <map>

class XMLElement
{
    private:
        std::string tagName;
        std::string id;
        std::string text;
        std::map<std::string, std::string> attributes;
        std::vector<XMLElement*> children;

    public:
        XMLElement(const std::string& name);
        ~XMLElement();

        //Капсулация на полетата на елемента
        std::string getTagName() const;

        std::string getId() const;
        void setId(const std::string& newId);

        std::string getText() const;
        void setText(const std::string& newText);

        //Управление на атрибутите на елемента
        const std::map<std::string, std::string> getAttributes() const;
        std::string getAttribute(const std::string& key) const;
        void setAttribute(const std::string& key, const std::string& value);
        void deleteAttribute(const std::string& key);

        //Подтагове
        void addChild(XMLElement* child);
        const std::vector<XMLElement*>& getChildren() const;
};

#endif