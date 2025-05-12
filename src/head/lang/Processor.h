#ifndef PROCESSOR_DEF
#define PROCESSOR_DEF
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <any>
#include "stringTools.h"

// Syntax
// class <name> { <body> }
// class <name> : <parent> { <body> }
// <type> <name>(<args>) { <body> }
// <variable> = <value>
// <variable>.<method>(<args>)
// <variable>.<property> is a <variable>
// <variable>[<index>] is a <variable>


namespace ProcessorTests {
    void testTokenizer();
    template<typename T>
    void assertEqual(const T& expected, const T& actual);

    void testIfStatement();
    void testElseStatement();

    void testExpressions();

    void testStatements();

    void testBlocks();

    void runTests();
}

namespace DataTypes {
    class Data;
    class Var;
    class Primitive;
    class Null;
    class String;
    class Bool;
    class Int;
    class Float;
    class Double;
    class Dict;
    class Array;
    class Class;
    class ClassInstance;
    class Function;
    // Variables are objects which contain data. The data can be anything. They are used to store values and can be passed around in the program.
    class Var {
        public:
            Data data;

            // Constructors using Lvalue ref. Used in Var(data_object_reference)
            Var(Data& _data) : data(_data) {}
            Var(const Data& _data) : data(_data) {}

            // Constructors using Rvalue ref. Used in Var(new_data_object)
            Var(const Data&& _data) : data(_data) {}

            Var(const Var& other) : data(other.data) {}
            Var(Var&& other) : data(std::move(other.data)) {}
            Var& operator=(const Var& other) {
                if (this != &other) {
                    data = other.data;
                }
                return *this;
            }

            Var getProperty(const Primitive& label) {
                // Dicts, Arrays and Classes are the only types that can have properties
                if (data.type == "class_instance") {
                    auto classInstance = std::any_cast<ClassInstance>(data.value);
                    return classInstance.getProperty(label);
                }
                if (data.type == "dict") {
                    auto dict = std::any_cast<Dict>(data.value);
                    return dict.getProperty(label);
                }
                if (data.type == "array") {
                    auto array = std::any_cast<Array>(data.value);
                    return array.getProperty(label);
                }
                throw std::runtime_error("Property not found in variable.");
            }

    };
    class Data {
        public:
            std::any value; 
            std::string type;
            Data (std::string _type,std::any _value) : type(_type), value(_value) {}
            Data (const Data& other) = default;
            Data& operator=(const Data& other) = default;
            Data (Data&& other) = default;
            Data(const Var& var) {
                const Data& data = var.data;
                type = data.type;
                value = data.value;
            }
            virtual ~Data() {}
            virtual const std::string toString() const {
                return type;
            }
            virtual const JsonObject toJSON() const {
                return JsonObject().add("type", type).add("value", value);
            }

            
            virtual Data&& operator+(const Data& other) const {return Null();}
            virtual Data&& operator-(const Data& other) const {return Null();}
            virtual Data&& operator/(const Data& other) const {return Null();}
            virtual Data&& operator*(const Data& other) const {return Null();}
            virtual Data&& operator%(const Data& other) const {return Null();}
            virtual Bool&& operator==(const Data& other) const {return Null();} 
            virtual Bool&& operator!=(const Data& other) const {return Null();}
            virtual Bool&& operator<(const Data& other) const {return Null();}
            virtual Bool&& operator>(const Data& other) const {return Null();}
            virtual Bool&& operator>=(const Data& other) const {return Null();}
            virtual Bool&& operator<=(const Data& other) const {return Null();}
            virtual Bool&& operator&&(const Data& other) const {return Null();}
            virtual Bool&& operator||(const Data& other) const {return Null();}
            virtual Data&& operator~() const {return Null();}
            virtual Data&& operator!() const {return Null();}
            virtual Data&& operator++() const {return Null();}
            virtual Data&& operator--() const {return Null();}
            virtual Data&& operator-() const {return Null();}

            virtual operator bool() const {
                return false;
            }
        };
    class Primitive : public Data {
        public:
            Primitive() : Data("primitive", std::any()) {}
            Primitive(std::string t, std::any v) : Data(t,v) {}
            Primitive(const Primitive& other) : Data(other.type, other.value) {} 
            Primitive& operator=(const Primitive& other) {
                if (this != &other) {
                    type = other.type;
                    value = other.value;
                }
                return *this;
            }
            Primitive(Primitive&& other) : Data(std::move(other)) {}
            Primitive(const Var& var) : Data(var) {}
            virtual std::size_t hash() const { // Default Hash function for the primitive type
                return -1;
            }
            operator bool() const override {
                return false;
            }
            virtual operator int() const {
                return -1;
            }
            virtual operator float() const {
                return -1.0f;
            }
            virtual operator double() const {
                return -1.0;
            }
            virtual operator std::string() const {
                return "abstract object";
            }
            Bool&& operator==(const Data& other) const override {
                return false; 
            }
    };
    // Numeric is a base class for all numeric types. It is used to define the common interface for all numeric types.
    template<typename T>
    class Numeric : public Primitive {
        public:
            Numeric(std::string t, std::any v) : Primitive(t,v) {}
            Numeric(const Numeric& other) : Primitive(other.type, other.value) {}
            Numeric& operator=(const Numeric& other) {
                if (this != &other) {
                    type = other.type;
                    value = other.value;
                }
                return *this;
            }
            Numeric(Numeric&& other) : Primitive(std::move(other)) {}
            virtual std::size_t hash() const = 0;
            operator bool() const override {
                return std::any_cast<T>(value) != 0;
            }
            operator int() const override {
                return std::any_cast<T>(value);
            }
            operator float() const override {
                return std::any_cast<T>(value);
            }
            operator double() const override {
                return std::any_cast<T>(value);
            }
            operator std::string() const override {
                return std::to_string(std::any_cast<T>(value));
            }
    };
    class Null : public Primitive {
        public:
            Null() : Primitive("null",std::any()) {}
            std::size_t hash() const override {
                return 0;
            }
            operator bool() const override {
                return false;
            }
            operator int() const override {
                return 0;
            }
            operator float() const override {
                return 0.0f;
            }
            operator double() const override {
                return 0.0;
            }
            operator std::string() const override {
                return "null";
            }



    };
    class String : public Primitive {
        public:
            String(std::string v) : Primitive("string",v) {}
            std::size_t hash() const override {
                return std::hash<std::string>()(std::any_cast<std::string>(value));
            }
            Bool&& operator==(const Data& other) const override {
                if (other.type != "string") return Bool(false);
                return Bool(std::any_cast<std::string>(value) == std::any_cast<std::string>(other.value));
            }
            operator std::string() const override {
                return std::any_cast<std::string>(value); 
            }
            operator bool() const override {
                return !std::any_cast<std::string>(value).empty();
            }
    };
    class Bool : public Numeric<bool> {
        public:
            Bool(bool v) : Numeric("bool", v) {}
            Bool(const Data&& other) : Bool(bool(other)) {}
            std::size_t hash() const override {
                return std::hash<bool>()(std::any_cast<bool>(value));
            }
            Bool&& operator==(const Data& other) const override {
                if (other.type != "bool") return Bool(false);
                return Bool(std::any_cast<bool>(value) == std::any_cast<bool>(other.value));
            }
    };

    class Int : public Numeric<int> {
        public:
            Int(int v) : Numeric("int", v) {}
            std::size_t hash() const override {
                return std::hash<int>()(std::any_cast<int>(value));
            }
            Bool&& operator==(const Data& other) const override {
                if (other.type != "int") return Bool(false);
                return std::any_cast<int>(value) == std::any_cast<int>(other.value);
            }
    };

    class Float : public Numeric<float> {
        public:
            Float(float v) : Numeric("float", v) {}
            std::size_t hash() const override {
                return std::hash<float>()(std::any_cast<float>(value));
            }
            Bool&& operator==(const Data& other) const override {
                if (other.type != "float") return Bool(false);
                return std::any_cast<float>(value) == std::any_cast<float>(other.value);
            }
    };

    class Double : public Numeric<double> {
        public:
            Double(double v) : Numeric("double", v) {}
            std::size_t hash() const override {
                return std::hash<double>()(std::any_cast<double>(value));
            }
            Bool&& operator==(const Data& other) const override {
                if (other.type != "double") return Bool(false);
                return std::any_cast<double>(value) == std::any_cast<double>(other.value);
            }
    };
    struct PrimitiveHash {
        std::size_t operator()(const Primitive& p) const {
            return p.hash();
        }
    };
    struct PrimitiveEqual {
        bool operator()(const Primitive& p1, const Primitive& p2) const {
            return p1 == p2;
        }
    };
    using Dictionary = std::unordered_map<Primitive, Var, PrimitiveHash, PrimitiveEqual>;
    class Dict : public Data {
        public:
            Dict() : Data("dict",Dictionary()) {}
            Dict(Dictionary d) : Data("dict",d) {}
            Dict(const Dict& other) : Data(other.type, other.value) {}
            Dict& operator=(const Dict& other) {
                if (this != &other) {
                    type = other.type;
                    value = other.value;
                }
                return *this;
            }
            Dict(Dict&& other) : Data(std::move(other)) {}
            Var getProperty(const Primitive& label) {
                auto dict = std::any_cast<Dictionary>(value);
                auto it = dict.find(label);
                if (it != dict.end()) {
                    return it->second;
                }
                throw std::runtime_error("Property not found in dictionary.");
            }
            const JsonObject toJSON() const override {
                JsonObject json;
                for (const auto& [key, value] : std::any_cast<Dictionary>(this->value)) {
                    json.add(key.toString(), value.data.toJSON());
                }
                return json;
            }
    };
    using ArrayList = std::vector<Var>;
    class Array : public Data {
        public:
            Array() : Data("array",ArrayList()) {}
            Array(ArrayList a) : Data("array",a) {}
            Array(const Array& other) : Data(other.type, other.value) {}
            Array& operator=(const Array& other) {
                if (this != &other) {
                    type = other.type;
                    value = other.value;
                }
                return *this;
            }
            Array(Array&& other) : Data(std::move(other)) {}
            Var getProperty(const Primitive& label) {
                if (label.type != "int") {
                    throw std::runtime_error("Index must be an integer.");
                }
                ArrayList array = std::any_cast<ArrayList>(value);
                int index = std::any_cast<int>(label.value);
                if (index < 0 || index >= array.size()) {
                    throw std::runtime_error("Index out of bounds.");
                }
                return array[index];
            }
            const JsonObject toJSON() const override {
                JsonArray jsonArray;
                for (const auto& item : std::any_cast<ArrayList>(this->value)) {
                    jsonArray.append(item.data.toJSON());
                }
                return JsonObject().add("array", jsonArray);
            }

    };
    class Class {
        public:
            std::string name;
            // Methods are functions that can be called on in the class instances. This means methods have a constant reference to their body
            std::vector<Function> methods;
            // Properties are variables that are defined with their initial values in the class. In the class instance, they are instantiated as actual variables.
            std::unordered_map<std::string, Data> properties;
            std::weak_ptr<Class> parent; // Parent pointer as weak_ptr
            Class(std::string n, std::unordered_map<std::string, Data> props = {}, std::weak_ptr<Class> parent_ref = std::weak_ptr<Class>()) : name(n), parent(parent_ref), properties(props) {}
            Class(const Class& other) : name(other.name), methods(other.methods), properties(other.properties) {}
            Class& operator=(const Class& other) {
                if (this != &other) {
                    name = other.name;
                    methods = other.methods;
                    properties = other.properties;
                    parent = other.parent;
                }
                return *this;
            }
            JsonObject toJSON() const {
                JsonObject json;
                json.add("name", name);
                JsonArray methodsArray;
                for (const auto& method : methods) {
                    methodsArray.append(method.toJSON());
                }
                json.add("methods", methodsArray);
                JsonObject propertiesObject;
                for (const auto& [key, value] : properties) {
                    propertiesObject.add(key, value.toJSON());
                }
                json.add("properties", propertiesObject);
                if (!parent.expired()) {
                    json.add("parent", parent.lock()->toJSON());
                }
                else {
                    json.add("parent", "null");
                }
                return json;
            }
            void addMethod(const Function& method) {
                methods.push_back(method);
            }
            void addProperty(const std::string& name, const Data& initialValue) {
                properties[name] = initialValue;
            }
            Var getProperty(const Primitive& label) {
                if (label.type != "string") {
                    throw std::runtime_error("Property name must be a string.");
                }
                // Check if the property exists in the class (properties and methods)
                // If it does, return the value of the property
                // If it doesn't, throw an error
                auto it = properties.find(std::any_cast<std::string>(label.value));
                if (it != properties.end()) {
                    return Var(it->second);
                }
                for (const auto& method : methods) {
                    if (method.name == std::any_cast<std::string>(label.value)) {
                        return Var(method);
                    }
                }
                // Check if the property exists in the parent class (if any)
                if (!parent.expired()) {
                    auto parentClass = parent.lock();
                    return parentClass->getProperty(label);
                }
                // If the property is not found in the class, throw an error
                throw std::runtime_error("Property not found in class (or superclass).");
            }

            const virtual Data& instantiate() {
                ClassInstance instance(*this);
                return instance;
            }
    };
    // A Class Instance is a variable that is an instance of a class.
    // It has a reference to the class and can access its methods and properties.
    class ClassInstance : public Data {
        public:
            std::unordered_map<std::string, Var> properties;
            ClassInstance(Class& c) : Data("class_instance",c) {
                for (const auto& [key, value] : c.properties) {
                    properties[key] = Var(value);
                }
            }
            ClassInstance(const ClassInstance& other) : Data(other.type, other.value), properties(other.properties) {}
            ClassInstance& operator=(const ClassInstance& other) {
                if (this != &other) {
                    type = other.type;
                    value = other.value;
                    properties = other.properties;
                }
                return *this;
            }
            Var getProperty(const Primitive& label) {
                if (label.type != "string") {
                    throw std::runtime_error("Property name must be a string.");
                }
                // Check if the property exists in the class instance
                // If it does, return the value of the property
                // If it doesn't, throw an error
                auto it = properties.find(std::any_cast<std::string>(label.value));
                if (it != properties.end()) {
                    return it->second;
                } 
                // Check if the property exists in the class (just methods)
                // If it does, return the value of the property
                // If it doesn't, throw an error
                for (const auto& method : std::any_cast<Class&>(value).methods) {
                    if (method.name == std::any_cast<std::string>(label.value)) {
                        return Var(method);
                    }
                }
                // If the property is not found in the class instance, throw an error
                throw std::runtime_error("Property not found in class instance.");
            }
            const JsonObject toJSON() const override {
                JsonObject json;
                json.add("class", std::any_cast<Class&>(value).toJSON());
                JsonObject propertiesObject;
                for (const auto& [key, value] : properties) {
                    propertiesObject.add(key, value.data.toJSON());
                }
                json.add("properties", propertiesObject);
                return json;
            }
    };
    class Function : public Data {
        public:
            Function(std::string n, std::vector<std::string> a, std::shared_ptr<Nodes::Block> b) : Data("function",b), name(n), args(a) {}
            Function(const Function& other) : Data(other.type, other.value), name(other.name), args(other.args) {}
            Function& operator=(const Function& other) {
                if (this != &other) {
                    type = other.type;
                    value = other.value;
                    name = other.name;
                    args = other.args;
                }
                return *this;
            }
            Function(Function&& other) : Data(std::move(other)), name(std::move(other.name)), args(std::move(other.args)) {}
            std::string name;
            std::vector<std::string> args;
            // Body is stored in the the superclass's "value" field.
    };

    class IntClassType : public Class {
        public:
            IntClassType() : Class("int") {
            }
            const Data& instantiate() override {
                Int instance(0);
                return instance;
            }
    };

    class StringClassType : public Class {
        public:
            StringClassType() : Class("string") {
            }
            const Data& instantiate() override {
                String instance("");
                return instance;
            }
    };
    class BoolClassType : public Class {
        public:
            BoolClassType() : Class("bool") {
            }
            const Data& instantiate() override {
                Bool instance(false);
                return instance;
            }
    };
    class FloatClassType : public Class {
        public:
            FloatClassType() : Class("float") {
            }
            const Data& instantiate() override {
                Float instance(0.0f);
                return instance;
            }
    };
    class DoubleClassType : public Class {
        public:
            DoubleClassType() : Class("double") {
            }
            const  Data& instantiate() override {
                Double instance(0.0);
                return instance;
            }
    };
    class NullClassType : public Class {
        public:
            NullClassType() : Class("null") {
            }
            const Data& instantiate() override {
                Null instance;
                return instance;
            }
    };
    class ArrayClassType : public Class {
        public:
            ArrayClassType() : Class("array") {
            }
            const Data& instantiate() override {
                Array instance;
                return instance;
            }
    };
    class DictClassType : public Class {
        public:
            DictClassType() : Class("dict") {
            }
            const Data& instantiate() override {
                Dict instance;
                return instance;
            }
    };

    std::vector<Class> defaultClassTypes = {
        IntClassType(),
        StringClassType(),
        BoolClassType(),
        FloatClassType(),
        DoubleClassType(),
        NullClassType(),
        ArrayClassType(),
        DictClassType()
    };
    // The default class types are the primitive types. They are used to define the primitive types in the language.
}

namespace Nodes {
    class Base : public std::enable_shared_from_this<Base> {
        public:
            std::weak_ptr<Base> parent; // Parent pointer as weak_ptr
            std::string name;

            Base(std::weak_ptr<Base> parentPointer, std::string n) 
                : parent(parentPointer), name(n) {}

            virtual ~Base() {
            } // Add a virtual destructor to make the class polymorphic

            const std::string toString() const;
            virtual const JsonObject toJSON() const;
            virtual void process(std::vector<std::string>::iterator& start, std::vector<std::string>::iterator end);
            virtual const DataTypes::Var& getVar(const std::string& label) const;
            virtual const DataTypes::Class& getClass(const std::string& label) const;
            virtual void execute() {}
    };

    class Expression : public Base {
        public:
            Expression(std::weak_ptr<Base> parentPointer, std::string n) 
                : Base(parentPointer, n) {}
            virtual DataTypes::Data evaluate(); // Default evaluate method
    };

    class Statement : public Expression {
        public:
            std::shared_ptr<Expression> expression; // Child pointer as shared_ptr
            Statement(std::weak_ptr<Base> parentPointer, std::string n, std::shared_ptr<Expression> expr = nullptr) 
                : Expression(parentPointer, n), expression(expr) {}
            const JsonObject toJSON() const override;
    };

    class Block : public Base {
        public:
            std::vector<std::shared_ptr<Statement>> stmts; // Child pointers as shared_ptr
            std::unordered_map<std::string, DataTypes::Var> variables;
            std::unordered_map<std::string, DataTypes::Class&> classTypes; // Class types are the primitive types.

            Block(std::weak_ptr<Base> parentPointer, std::string n) 
                : Base(parentPointer, n) {
                for (const auto& classType : DataTypes::defaultClassTypes) {
                    classTypes[classType.name] = classType;
                }
            }

            void setVar(const std::string& label, DataTypes::Data value);
            void addClass(DataTypes::Class& classType);

            const DataTypes::Var& getVar(const std::string& label) const override;
            const DataTypes::Class& getClass(const std::string& label) const override;
            void process(std::vector<std::string>::iterator& start, std::vector<std::string>::iterator end);

            const JsonObject toJSON() const override;
    };

    namespace Expressions {
        class StatementCondition;
        class BinaryExpression;
        class UnaryExpression;

        class Value; // A constant value, must be a primitive type.
            class ArrayList; // A list of values, must be a primitive type.
            class MapDictionary; // A dictionary of values, must be a primitive type.


        class FunctionCall; // A function call with arguments and a variable.
        class VariableAccessor;
            class VariableDeclaration; // A variable that is a reference to a value.
            class VariableProperty; // A property of a variable, array, or dict which is itself a variable.
        
        // VariableProperty = "var.prop" or "var[numerical_index]" or "var[string_index]"
        // VariableDeclaration = "type name = value"
        // FunctionCall = "var_which_holds_a_function(args)"
        // VariableProperty is a subclass of VariableAccessor, which is a subclass of Expression.

        class ClassReference; // A reference to a class, which is a variable.
    }

    namespace Statements {
        class IfStatement : public Statement {
            public:
                std::shared_ptr<Block> body; // Child pointer as shared_ptr
    
                IfStatement(std::weak_ptr<Base> parentPointer)
                    : Statement(parentPointer, "IfStatement") {
                }
                void process(std::vector<std::string>::iterator& start, std::vector<std::string>::iterator end);

                const JsonObject toJSON() const override;
        };

        class ElseStatement : public Statement {
            public:
                std::shared_ptr<Block> body; // Child pointer as shared_ptr
    
                ElseStatement(std::weak_ptr<Base> parentPointer, std::string n)
                    : Statement(parentPointer, n) {
                }
        };
        class PrintStatement;
        class ImportStatement;

        class WhileStatement;
        class ForStatement;

        class BreakStatement;
        class ContinueStatement;

        class AssignmentStatement;

        class FunctionStatement;
        class ReturnStatement;
        
        class ClassStatement;
        
        class SwitchStatement;
        class CaseStatement;
        
    }

    namespace Blocks {
        class StatementBlock : public Block {
            public:
                StatementBlock(std::weak_ptr<Base> p) : Block(p, "Statement Block") {}
                void process(std::vector<std::string>::iterator& start, std::vector<std::string>::iterator end);
        };
    }

    class Body : public Block {
        public:
            Body() : Block(std::weak_ptr<Base>(), "Main Body") {}
            const DataTypes::Var& getVar(const std::string& label) const override;
            const DataTypes::Class& getClass(const std::string& label) const override;
    };
}

class Tokenizer {
    public:
        static std::vector<std::string> process(const std::string input);
};


#endif // PROCESSOR_DEF