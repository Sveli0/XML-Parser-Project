#include "XMLElement.h"

XMLElement::XMLElement(const std::string& name)
{
    this -> tagName = name;
    this -> id = "";
    this -> text = "";
}

XMLElement::~XMLElement()
{
    for (XMLElement* child : children)
        delete child;
}

std::string XMLElement::getTagName() const
{
    return tagName;
}

std::string XMLElement::getId() const
{
    return id;
}

void XMLElement::setId(const std::string& newId)
{
    this -> id = id;
}

std::string XMLElement::getText() const
{
    return text;
}

void XMLElement::setText(const std::string& newText)
{
    this -> text = newText;
}

const std::map<std::string, std::string> XMLElement::getAttributes() const
{
    return attributes;   
}

std::string XMLElement::getAttribute(const std::string& key) const
{
    auto it = attributes.find(key);

    if (it != attributes.end())
        return it -> second;

    return "";
}

void XMLElement::setAttribute(const std::string& key, const std::string& value)
{
    attributes[key] = value;
}

void XMLElement::deleteAttribute(const std::string& key)
{
    attributes.erase(key);
}

void XMLElement::addChild(XMLElement* child)
{
    children.push_back(child);
}

const std::vector<XMLElement*>& XMLElement::getChildren() const
{
    return children;
}