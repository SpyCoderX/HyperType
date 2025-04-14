#ifndef PROCESSOR_DEF
#define PROCESSOR_DEF
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <any>
#include "stringTools.h"

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

namespace Nodes {
    class Base : public std::enable_shared_from_this<Base> {
        public:
            std::weak_ptr<Base> parent; // Parent pointer as weak_ptr
            std::string name;

            Base(std::weak_ptr<Base> parentPointer, std::string n) 
                : parent(parentPointer), name(n) {}

            virtual ~Base() {
            } // Add a virtual destructor to make the class polymorphic

            std::string toString();
            virtual JsonObject toJSON();
            virtual void process(std::vector<std::string>::iterator& start, std::vector<std::string>::iterator end);
            virtual std::any& getVar(const std::string& label);
            virtual void execute() {}
    };

    class Expression : public Base {
        public:
            Expression(std::weak_ptr<Base> parentPointer, std::string n) 
                : Base(parentPointer, n) {}
    };

    class Statement : public Expression {
        public:
            std::shared_ptr<Expression> expression; // Child pointer as shared_ptr
            Statement(std::weak_ptr<Base> parentPointer, std::string n, std::shared_ptr<Expression> expr = nullptr) 
                : Expression(parentPointer, n), expression(expr) {}
            JsonObject toJSON() override;
    };

    class Block : public Base {
        public:
            std::vector<std::shared_ptr<Statement>> stmts; // Child pointers as shared_ptr
            std::unordered_map<std::string, std::any> variables;

            Block(std::weak_ptr<Base> parentPointer, std::string n) 
                : Base(parentPointer, n) {}

            template<typename T>
            void addVar(const std::string& label, T value);

            std::any& getVar(const std::string& label) override;
            void process(std::vector<std::string>::iterator& start, std::vector<std::string>::iterator end);

            JsonObject toJSON() override;
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

                JsonObject toJSON() override;
        };

        class ElseStatement : public Statement {
            public:
                std::shared_ptr<Block> body; // Child pointer as shared_ptr
    
                ElseStatement(std::weak_ptr<Base> parentPointer, std::string n)
                    : Statement(parentPointer, n) {
                }
        };

        class WhileStatement;
        class ForStatement;
        class ReturnStatement;
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
            std::any& getVar(const std::string& label) override;
    };
}

class Tokenizer {
    public:
        static std::vector<std::string> process(const std::string input);
};

#endif