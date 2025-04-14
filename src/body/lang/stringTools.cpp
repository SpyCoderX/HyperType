#include <string>
#include <vector>
#include <any>
#include <unordered_map>
#include <memory>
#include <iostream>
#include "../../head/lang/stringTools.h"
#include "../../head/lang/Processor.h"
std::string vecToStr(const std::vector<std::string>& vec) {
    std::string result;
    result += "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        result += vec[i] + ((i + 1 < vec.size())?", ":"");
    }
    result += "]";
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
    
    return "Unknown Type";
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
        result += "   "+it->first + ": " + second +"\n";
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