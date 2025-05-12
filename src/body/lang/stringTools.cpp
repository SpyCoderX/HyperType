#include <string>
#include <vector>
#include <any>
#include <unordered_map>
#include <memory>
#include <iostream>
#include "../../head/lang/stringTools.h"
#include "../../head/lang/Processor.h"

struct AnyHash {
    std::size_t operator()(const std::any& expr) const {
        // Implement a hash function for Nodes::Expression
        return std::hash<std::string>()(toStr(expr));
    }
};

struct AnyEqual {
    bool operator()(const std::any& lhs, const std::any& rhs) const {
        // Implement equality comparison for Nodes::Expression
        return toStr(lhs) == toStr(rhs);
    }
};
using AnyDictionary = std::unordered_map<std::any,std::any, AnyHash, AnyEqual>;


std::string vecToStr(const std::vector<std::string>& vec) {
    std::string result;
    result += "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        result += vec[i] + ((i + 1 < vec.size())?", ":"");
    }
    result += "]";
    return result;
}
std::string mapToStr(const AnyDictionary& map) {
    std::string result;
    result += "{";
    for (auto it = map.begin(); it != map.end(); ++it) {
        result += "" + toStrFormatted(it->first) + ": " + toStrFormatted(it->second) + "";
        if (std::next(it) != map.end()) {
            result += ", ";
        }
    }
    result += "}";
    return result;
}

std::string toStr(const std::any& obj) {
    if (obj.type() == typeid(int)) 
    {
        return std::to_string(std::any_cast<int>(obj));
    }
    else if (obj.type() == typeid(double))
    {
        return std::to_string(std::any_cast<double>(obj));
    }
    else if (obj.type() == typeid(bool)) {
        return std::any_cast<bool>(obj) ? "true" : "false";
    }
    else if (obj.type() == typeid(std::string)) 
    {
        return std::any_cast<std::string>(obj);
    }
    else if (obj.type() == typeid(const char*)) 
    {
        return std::string(std::any_cast<const char*>(obj));
    }
    else if (obj.type() == typeid(char))
    {
        return std::string(1, std::any_cast<char>(obj));
    }
    else if (obj.type() == typeid(std::vector<std::string>)) 
    {
        return vecToStr(std::any_cast<std::vector<std::string>>(obj));
    }
    else if (obj.type() == typeid(std::vector<std::any>))
    {
        std::vector<std::string> vec;
        for (std::any& item : std::any_cast<std::vector<std::any>>(obj)) {
            vec.push_back(toStr(item));
        }
        return vecToStr(vec);
    }
    else if (obj.type() == typeid(DataTypes::ArrayList))
    {
        std::vector<std::string> vec;
        for (const auto& item : std::any_cast<DataTypes::ArrayList>(obj)) {
            vec.push_back(toStr(item.data.toJSON()));
        }
        return vecToStr(vec);

    }
    else if (obj.type() == typeid(DataTypes::Dictionary)) 
    {
        AnyDictionary map;
        for (const auto& pair : std::any_cast<DataTypes::Dictionary>(obj)) {
            map[pair.first.value] = pair.second.data.value;
        }
        return mapToStr(map);

    }
    else if (obj.type() == typeid(DataTypes::Data)) {
        return toStr(std::any_cast<DataTypes::Data>(obj).toJSON());
    }
    else if (obj.type() == typeid(DataTypes::Var)) {
        return toStr(std::any_cast<DataTypes::Var>(obj).data.toJSON());
    }
    else if (obj.type() == typeid(DataTypes::Class)) {
        return toStr(std::any_cast<DataTypes::Class>(obj).toJSON());
    }
    else if (obj.type() == typeid(JsonObject)) 
    {
        return std::any_cast<JsonObject>(obj).toString();
    }
    else if (obj.type() == typeid(JsonArray)) 
    {
        return std::any_cast<JsonArray>(obj).toString();
    }
    else if (obj.type() == typeid(Nodes::Base)) {
        return toStr(std::any_cast<Nodes::Base>(obj).toJSON());
    }
    else if (obj.type() == typeid(NullObject)) {
        return "null";
    }
    
    return "Unknown Type: " + std::string(obj.type().name()); // Handle unknown types
}

std::string toStrFormatted(const std::any& obj) {
    std::string result = toStr(obj);
    if (obj.type() == typeid(std::string)) {
        result = "\""+result+"\"";
    }
    return result;
}


JsonObject& JsonObject::add(const std::string label, const std::any& value) {
    data.push_back(std::make_pair(label, value));
    return (*this);
}


std::string JsonObject::toString() const {
    std::string result;
    std::string second;
    result += "{";
    if (!data.empty()) {
        result += "\n";
    }
    for (auto it = data.begin(); it != data.end(); ++it) {
        second = toStrFormatted(it->second);
        // Indent and process newlines
        second = indentNewlines(second);
        // Append to result
        result += "   \""+it->first + "\": " + second + (std::next(it)!=data.end()?",\n":"\n");
    }
    result += "}";
    return result;
}

JsonArray& JsonArray::append(const std::any& value) {
    data.push_back(value);
    return (*this);
}
std::string JsonArray::toString() const {
    std::string result;
    std::string value;
    result += "[\n";
    for (auto it = data.begin(); it != data.end(); ++it) {
        value = toStrFormatted(*it);
        // Indent and process newlines
        value = indentNewlines(value);
        // Append to result
        result += "   "+value;
        if (std::next(it) != data.end()) {
            result += ",\n";
        }
    }
    result += "]";
    return result;
}

std::string indentNewlines(const std::string& str){
    std::string result = "";
    for (auto x = str.begin(); x != str.end(); x++) {
        result += *x;
        if (*x == '\n') {
            result += "   ";
        }
    }
    return result;
}