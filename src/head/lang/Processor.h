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
    class Data {
        public:
            std::string type;
            Data (std::string _type) : type(_type) {}
            Data (const Data& other) = default;
            virtual ~Data() {}
            virtual const std::string toString() const {
                return type;
            }
            virtual const JsonObject toJSON() const {
                return JsonObject().add("type", type);
            }
    };
    class Null : public Data {
        public:
            Null() : Data("null") {}
    };
    class Primitive : public Data {
        public:
            std::any value;
            Primitive(std::any v,std::string _type) : value(v), Data("primitive: "+_type) {}
            const std::string toString() const override {
                return toStr(value);
            }
            const JsonObject toJSON() const override {
                JsonObject data = Data::toJSON();
                data.add("value", value);
                return data;
            }
    };
    class String : public Primitive {
        public:
            String(std::string v) : Primitive(v,"string") {}
    };
    class Bool : public Primitive {
        public:
            Bool(bool v) : Primitive(v,"bool") {}
    };
    class Int : public Primitive {
        public:
            Int(int v) : Primitive(v,"int") {}
    };
    class Float : public Primitive {
        public:
            Float(float v) : Primitive(v,"float") {}
    };
    
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
            virtual const std::any& getVar(const std::string& label) const;
            virtual void execute() {}
    };

    class Expression : public Base {
        public:
            Expression(std::weak_ptr<Base> parentPointer, std::string n) 
                : Base(parentPointer, n) {}
            virtual std::any evaluate(); // Default evaluate method
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
            std::unordered_map<std::string, std::any> variables;

            Block(std::weak_ptr<Base> parentPointer, std::string n) 
                : Base(parentPointer, n) {}

            template<typename T>
            void addVar(const std::string& label, T value);

            const std::any& getVar(const std::string& label) const override;
            void process(std::vector<std::string>::iterator& start, std::vector<std::string>::iterator end);

            const JsonObject toJSON() const override;
    };

    namespace Expressions {
        class StatementCondition;
        class BinaryExpression;
        class UnaryExpression;

        class Value;

        class Variable;
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
            const std::any& getVar(const std::string& label) const override;
    };
}

class Tokenizer {
    public:
        static std::vector<std::string> process(const std::string input);
};

struct ExpressionHash {
    std::size_t operator()(const std::shared_ptr<Nodes::Expression>& expr) const {
        // Implement a hash function for Nodes::Expression
        return std::hash<std::string>()(toStr(expr->evaluate()));
    }
};

struct ExpressionEqual {
    bool operator()(const std::shared_ptr<Nodes::Expression>& lhs, const std::shared_ptr<Nodes::Expression>& rhs) const {
        // Implement equality comparison for Nodes::Expression
        return toStr(lhs->evaluate()) == toStr(rhs->evaluate());
    }
};
using Dictionary = std::unordered_map<std::shared_ptr<Nodes::Expression>,std::shared_ptr<Nodes::Expressions::Variable>, ExpressionHash, ExpressionEqual>;
using Array = std::vector<std::shared_ptr<Nodes::Expressions::Variable >>;
#endif // PROCESSOR_DEF