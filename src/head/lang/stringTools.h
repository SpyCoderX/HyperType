#ifndef STR_TOOLS
#define STR_TOOLS
#include <string>
#include <vector>
#include <any>
#include <unordered_map>
#include <list>
#include <iostream>

class NullObject {};

class JsonObject {
    public:
        std::vector<std::pair<std::string, std::any>> data;
        JsonObject() : data() {}

        JsonObject& add(const std::string label, const std::any& value);

        std::string toString() const;
};
class JsonArray {
    public:
        std::list<std::any> data;
        JsonArray() : data() {}
        JsonArray& append(const std::any& value);
        std::string toString() const;
};
std::string vecToStr(const std::vector<std::string>& vec);
std::string toStr(const std::any& obj);
std::string toStrFormatted(const std::any& obj);
std::string indentNewlines(const std::string &str);


#endif