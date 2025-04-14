#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <unordered_map>
#include <any>
// Smart pointers
#include <memory>
#include <cassert>
#include <cctype> // for isspace, isalpha, isdigit
#include "../../head/lang/Processor.h"
#include "../../head/lang/stringTools.h"
#include "../../head/color/consoleColors.h"
#include "../../head/lang/operatorTools.h"


namespace ProcessorTests {
    // Test cases for the Tokenizer class
    void testTokenizer() {
        printf("Testing Tokenizer...\n");
        std::string input = "int a = 5; // This is a comment\nfloat b = 10.5; /* Multiline comment */";
        std::vector<std::string> expectedOutput = {"int", "a", "=", "5", ";", "float", "b", "=", "10",".","5", ";"};
        std::vector<std::string> output = Tokenizer::process(input);
        assertEqual(expectedOutput,output);
    }
    template<typename T>
    void assertEqual(const T& expected, const T& actual) {
        if (expected != actual) {

            ConsoleColors::PrintError("Test FAILED:\n - exp: " + toStr(expected) + "\n - got: " + toStr(actual) +"\n");
        } else {
            ConsoleColors::PrintSuccess("Test passed!\n");
        }
    }
    // Expression tests
    void testIfStatement() {
        printf("- IfStatement...\n");
        std::shared_ptr<Nodes::Body> body = std::make_shared<Nodes::Body>();
        body->addVar("a", std::any(5));
        // Add tests for IfStatement and its derived classes
        std::vector<std::string> tokens = {"if", "(","a", "==", "true", "+", "5", ".", "43", ")", "{", "return", "a;", "}"};
        auto start = tokens.begin();
        auto end = tokens.end();
        body->process(start, end); // Assuming process is a method that interprets the tokens and populates the body
        std::cout << toStr(body->toJSON()) << std::endl;
        ConsoleColors::PrintSuccess("  - IfStatement processed successfully.\n");
    }
    void testElseStatement() {
        printf("- ElseStatement...\n");
        // Add tests for ElseStatement and its derived classes
        ConsoleColors::PrintSuccess("  - ElseStatement processed successfully.\n");
    }
    void testExpressions() {
        printf("Testing Expressions...\n");
        // Add tests for Expression and its derived classes
        
    }
    void testStatements() {
        printf("Testing Statements...\n");
        testIfStatement();
        testElseStatement();
        // Add tests for Statement and its derived classes
        
    }
    void testBlocks() {
        printf("Testing Blocks...\n");
        // Add tests for Block and its methods
        
    }
    void runTests() {
        try {
        testTokenizer();
        testExpressions();
        testStatements();
        testBlocks();
        ConsoleColors::PrintSuccess("All tests completed.\n");
        } catch (const std::exception& e) {
            ConsoleColors::PrintError("Test failed: " + std::string(e.what()));
        }
        // Add more tests for other classes and methods as needed
    }

}
std::vector<std::vector<std::string>> operatorLevels = {
    {"||", "&&"},
    {"==", "!=", "<", ">", "<=", ">="},
    {"+", "-"},
    {"*", "/"}
};
namespace Nodes {
    std::shared_ptr<Nodes::Expression> parse(std::vector<std::string>::iterator& start, std::vector<std::string>::iterator end,uint32_t level = 0);
    std::shared_ptr<Nodes::Expression> parseFactor(std::vector<std::string>::iterator& start, std::vector<std::string>::iterator end);

    std::string Base::toString() {
        return name;
    }
    JsonObject Base::toJSON() {
        return JsonObject().add("type",toString()).add("parent",(!parent.expired())?parent.lock()->toString():"null");
    }
    void Base::process(std::vector<std::string>::iterator& start, std::vector<std::string>::iterator end) {
        // Default implementation does nothing
        // Derived classes can override this method to provide specific processing
    }
    std::any Expression::evaluate() {
        // Default implementation does nothing
        // Derived classes can override this method to provide specific evaluation
        return NullObject();
    }
    std::any& Base::getVar(const std::string& label) {
        if (parent.expired()) {
            static std::any empty = NullObject();
            return empty;
        }
        return parent.lock()->getVar(label);
    }

    JsonObject Statement::toJSON() {
        JsonObject json = Expression::toJSON();
        if (expression) {
            json.add("expression", expression->toJSON());
        }
        return json;
    }

    JsonObject Block::toJSON() {
        JsonObject json = Base::toJSON();
        JsonArray stmtsArray;
        for (const std::shared_ptr<Statement>& stmt : stmts) { // Updated to use shared_ptr
            stmtsArray.append(stmt->toJSON());
        }
        json.add("statements", stmtsArray);
        return json;
    }
    

    namespace Expressions {
        class StatementCondition : public Expression {
            public:
                std::shared_ptr<Expression> condition; // Updated to shared_ptr

                StatementCondition(std::weak_ptr<Base> parentPointer)
                    : Expression(parentPointer, "Statement Condition") {
                    }

                void process(std::vector<std::string>::iterator& start, std::vector<std::string>::iterator end) override {
                    if (start == end || *start != "(") {
                        throw std::runtime_error("Expected '(' in condition.");
                    }
                    // Move past '('
                    start++;

                    std::vector<std::string>::iterator expressionEnd = start;
                    int depth = 1;
                    while (expressionEnd != end && depth > 0) {
                        std::string token = *expressionEnd;
                        if (token == "(") {
                            depth++;
                        } else if (token == ")") {
                            if (--depth == 0) {
                                break;
                            }
                        }
                        
                        expressionEnd++;
                    }
                    if (depth > 0) {
                        throw std::runtime_error("Unmatched '(' in condition.");
                    }
                    condition = parse(start, expressionEnd);
                    condition->parent = std::weak_ptr<Base>(shared_from_this());
                    start = ++expressionEnd;
                }
                JsonObject toJSON() {
                    JsonObject base = Expression::toJSON();
                    base.add("condition",condition ? condition->toJSON() : JsonObject());
                    return base;
                }
                std::any evaluate() override {
                    // Evaluate the condition expression
                    return condition->evaluate();
                }
        };
        class BinaryExpression : public Expression {
            public:
                std::shared_ptr<Expression> left;
                std::shared_ptr<Expression> right;
                std::string op; // Operator like +, -, *, /, etc.

                BinaryExpression(std::weak_ptr<Base> parentPointer,  std::shared_ptr<Expression> leftExpr, std::string oper)
                    : Expression(parentPointer, "Operator(Bi): "+oper), left(leftExpr), right(nullptr), op(oper) {
                    }
                JsonObject toJSON() override {
                    JsonObject json = Expression::toJSON();
                    json.add("left", left ? left->toJSON() : JsonObject());
                    json.add("right", right ? right->toJSON() : JsonObject());
                    return json;
                }
                std::any evaluate() {
                    // Evaluate the left and right expressions
                    std::any leftValue = left->evaluate();
                    std::any rightValue = right->evaluate();

                    // Perform the operation based on the operator
                    if (op == "+") {
                        return OperatorTools::add(leftValue, rightValue);
                    } else if (op == "-") {
                        return OperatorTools::subtract(leftValue, rightValue);
                    } else if (op == "*") {
                        return OperatorTools::multiply(leftValue, rightValue);
                    } else if (op == "/") {
                        return OperatorTools::divide(leftValue, rightValue);
                    } else if (op == "%") {
                        return OperatorTools::modulus(leftValue, rightValue);
                    } else if (op == "==") {
                        return OperatorTools::equals(leftValue, rightValue);
                    } else if (op == "!=") {
                        return OperatorTools::notEquals(leftValue, rightValue);
                    } else if (op == "<") {
                        return OperatorTools::lessThan(leftValue, rightValue);
                    } else if (op == ">") {
                        return OperatorTools::greaterThan(leftValue, rightValue);
                    } else if (op == "<=") {
                        return OperatorTools::lessThanOrEqual(leftValue, rightValue);
                    } else if (op == ">=") {
                        return OperatorTools::greaterThanOrEqual(leftValue, rightValue);
                    } else if (op == "&&") {
                        return OperatorTools::logicalAnd(leftValue, rightValue);
                    } else if (op == "||") {
                        return OperatorTools::logicalOr(leftValue, rightValue);
                    }
                    return NullObject();
                }
        };
        class UnaryExpression : public Expression {
            public:
            std::string op; // Operator like -, !, etc.
            std::shared_ptr<Expression> expr;

                UnaryExpression(std::weak_ptr<Base> parentPointer, std::string oper, std::shared_ptr<Expression> operand)
                    : Expression(parentPointer, "Operator(Uni): "+oper), op(oper), expr(operand) {}
                JsonObject toJSON() override {
                    JsonObject json = Expression::toJSON();
                    json.add("operand", expr ? expr->toJSON() : JsonObject());
                    return json;
                }
                std::any evaluate() {
                    // Evaluate the operand expression
                    std::any operandValue = expr->evaluate();

                    // Perform the operation based on the operator
                    if (op == "-") {
                        return OperatorTools::negate(operandValue);
                    } else if (op == "!") {
                        return OperatorTools::logicalNot(operandValue);
                    }
                    return NullObject();
                }
        };
        class ParenthesisExpression : public Expression {
            public:
                std::shared_ptr<Expression> expr;

                ParenthesisExpression(std::weak_ptr<Base> parentPointer, std::string n)
                    : Expression(parentPointer, n) {}
                
                JsonObject toJSON() override {
                    JsonObject json = Expression::toJSON();
                    json.add("expression", expr ? expr->toJSON() : JsonObject());
                    return json;
                }
                std::any evaluate() {
                    // Evaluate the expression inside the parentheses
                    return expr->evaluate();
                }
        };

        class Value : public Expression {
            public:
                std::any value;
                Value(std::weak_ptr<Base> parentPointer, std::string n, std::any val) 
                    : Expression(parentPointer, n), value(std::move(val)) {}

                virtual std::any get() {
                    return value;
                }
                JsonObject toJSON() override {
                    JsonObject json = Expression::toJSON();
                    json.add("value", get());
                    return json;
                }
                std::any evaluate() override {
                    return get();
                }
        };

        class Variable : public Value {
            public:
                std::string varLabel;

                Variable(std::weak_ptr<Base> parentPointer, std::string label) 
                    : Value(parentPointer, "Variable", std::any(2)), varLabel(label) {
                    }

                std::any get() override {
                    return Value::getVar(varLabel);
                }
                JsonObject toJSON() override {
                    JsonObject json = Value::toJSON();
                    json.add("label", varLabel);
                    return json;
                }
        };
        
    } // namespace Expressions
    
    namespace Statements {
       void IfStatement::process(std::vector<std::string>::iterator& start, std::vector<std::string>::iterator end) {
            
            if (start == end || *start != "if") {
                throw std::runtime_error("Expected 'if' keyword.");
            }
            start++;

            expression = std::make_shared<Expressions::StatementCondition>(shared_from_this()); // Updated to shared_ptr
            expression->process(start, end); // Process the condition
            body = std::make_shared<Blocks::StatementBlock>(shared_from_this()); // Updated to shared_ptr
            body->process(start, end); // Process the body block

        }
        JsonObject IfStatement::toJSON() {
            JsonObject json = Statement::toJSON();
            json.add("body", body->toJSON());
            return json;
        }
    }

    
    template<typename T>
    void Block::addVar(const std::string& label, T value) {
        // Check if the variable already exists
        if (variables.find(label) != variables.end()) {
            throw std::runtime_error("Variable '" + label + "' already exists in this scope.");
        }
        // Store the value in the unordered_map
        variables[label] = value;
    }


    std::any& Block::getVar(const std::string& label) {
        auto it = variables.find(label);
        if (it == variables.end()) {
            return Base::getVar(label);
        }
        return it->second;
    }
    void Block::process(std::vector<std::string>::iterator& start, std::vector<std::string>::iterator end) {
        // This method should parse the tokens and create statements
        // FUll implementation.
        // Start with looping the tokens.
        // Example code:
        // if (a == b) {
        //     // do something
        // } else {
        //     // do something else
        // }
        while (start != end) {
            std::string token = *start;
            if (token == "if") {
                // Handle IfStatement
                std::shared_ptr<Nodes::Statements::IfStatement> ifStmt = std::make_shared<Nodes::Statements::IfStatement>(shared_from_this());
                stmts.push_back(ifStmt);
                ifStmt->process(start, end);
            }
            start++;
        }
    }

    namespace Blocks {
        void StatementBlock::process(std::vector<std::string>::iterator& start, std::vector<std::string>::iterator end)  {
            // Process the block of statements
            int depth = 1; // To handle nested blocks, we need a depth counter.
            // Make sure we start with a '{'
            if (*start != "{") {
                throw std::runtime_error("Expected '{' in block.");
            }
            // Move past the '{'
            start++;

            std::vector<std::string>::iterator blockEnd = start;
            // These loops are just to find the end of the block. Processing the block comes after.
            while (depth > 0 && blockEnd != end) {
                std::string token = *blockEnd;
                if (token == "}") {
                    if (--depth == 0) {
                        break;
                    }
                }
                else if (token == "{") {
                    depth++; // Nested block
                }
                blockEnd++;
            }
            if (depth > 0) {
                throw std::runtime_error("Unmatched '{' in block.");
            }
            std::vector<std::string> blockTokens(start, blockEnd);
            auto it = blockTokens.begin();
            Block::process(it, blockTokens.end()); // Process the block tokens
        }
    }

    std::any& Body::getVar(const std::string& label) {
        // Body should the top level scope, so throw an error if the variable is not found.
        auto it = variables.find(label);
        if (it == variables.end()) {
            throw std::runtime_error("Variable '" + label + "' not found in Body scope.");
        }
        return it->second;
    }

    
    std::shared_ptr<Nodes::Expression> parse(std::vector<std::string>::iterator& start, std::vector<std::string>::iterator end, uint32_t level) {
        if (level >= operatorLevels.size()) {
            return parseFactor(start, end);
        }
        // Parse the first term
        auto left = parse(start, end, level + 1);
    
        // Handle binary operators (+, -)
        while (start != end && (std::find(operatorLevels[level].begin(), operatorLevels[level].end(), *start) != operatorLevels[level].end())) {
            std::string op = *start;
            start++; // Move past the operator
    
            // Parse the right-hand side term
            auto right = parse(start, end, level + 1);
    
            // Create a BinaryExpression node
            auto binaryExpr = std::make_shared<Nodes::Expressions::BinaryExpression>(std::weak_ptr<Nodes::Base>(), left, op);
            if (!binaryExpr) {
                throw std::runtime_error("Expected BinaryExpression while assigning right operand.");
            }
            binaryExpr->right = right;
            binaryExpr->left->parent = std::weak_ptr<Nodes::Base>(binaryExpr);
            binaryExpr->right->parent = std::weak_ptr<Nodes::Base>(binaryExpr);
            left = binaryExpr;
        }
    
        return left;
    }
    std::shared_ptr<Nodes::Expression> parseFactor(std::vector<std::string>::iterator& start, std::vector<std::string>::iterator end) {
        if (start == end) {
            throw std::runtime_error("Unexpected end of tokens while parsing factor.");
        }
    
        std::string token = *start;
    
        // Handle parentheses
        if (token == "(") {
            start++; // Move past '('
            auto expr = parse(start, end, 0);
            if (start == end || *start != ")") {
                throw std::runtime_error("Expected ')' after expression.");
            }
            start++; // Move past ')'
            return expr;
        }
    
        // Handle unary operators (-, !)
        if (token == "-" || token == "!") {
            start++; // Move past the operator
            auto operand = parseFactor(start, end);
            auto uni_expr = std::make_shared<Nodes::Expressions::UnaryExpression>(std::weak_ptr<Nodes::Base>(), token, operand);
            uni_expr->expr->parent = std::weak_ptr<Nodes::Base>(uni_expr);
            return uni_expr;
        }

        if (token == "true" || token == "false") {
            start++; // Move past the boolean
            return std::make_shared<Nodes::Expressions::Value>(std::weak_ptr<Nodes::Base>(), "Boolean", token == "true");
        }

        // Handle dictionaries (also called maps)
        if (token == "{") {
            start++; // Move past the opening brace
            std::vector<std::pair<Nodes::Expression,Nodes::Expression>> dictExpressions;
            
            if (start == end || *start != "}") {
                throw std::runtime_error("Expected '}' to close dictionary.");
            }
            start++; // Move past the closing brace
            return std::make_shared<Nodes::Expressions::Value>(std::weak_ptr<Nodes::Base>(), "Dictionary", dictExpressions);
        }

        // Handle arrays
        if (token == "[") {
            start++; // Move past the opening bracket
            std::vector<Nodes::Expression> arrayExpressions;
            while (start != end && *start != "]") {
                auto expr = parse(start, end);
                arrayExpressions.push_back(*expr);
                if (start != end && *start == ",") {
                    start++; // Move past the comma
                }
            }
            if (start == end || *start != "]") {
                throw std::runtime_error("Expected ']' to close array.");
            }
            start++; // Move past the closing bracket
            return std::make_shared<Nodes::Expressions::Value>(std::weak_ptr<Nodes::Base>(), "Array", arrayExpressions);
        }
    
        // Handle numbers
        if (std::isdigit(token[0])) {
            start++; // Move past the number
            if (*start == ".") {
                start++;
                token += "."+(*start);
                start++;
            }
            return std::make_shared<Nodes::Expressions::Value>(std::weak_ptr<Nodes::Base>(), "Number", std::stoi(token));
        }
    
        // Handle variables
        if (std::isalpha(token[0])) {
            start++; // Move past the variable
            return std::make_shared<Nodes::Expressions::Variable>(std::weak_ptr<Nodes::Base>(), token);
        }

        // Handle string literals
        if (token == "\"") {
            // Move past the opening quote
            start++;

            // Get the string value
            std::string strValue;
            if (*start == "\"") {
                strValue = "";
            } else {
                strValue = *start++;
            }
            // Move past the closing quote value
            start++;
            return std::make_shared<Nodes::Expressions::Value>(std::weak_ptr<Nodes::Base>(), "String", strValue);
        }
    
        throw std::runtime_error("Unexpected token: " + token);
    }
}

class Interpreter {
    public:
        void process(std::string in) {
            std::vector<std::string> tokens = Tokenizer::process(in);
            Nodes::Body body;
            auto it = tokens.begin();
            body.process(it, tokens.end()); // Assuming process is a method that interprets the tokens and populates the body
        }
};

std::vector<std::string> combinedSymbols = {"==", ">=", "<=", "!=", "+=", "-=", "*=", "/=", "%=", "&&", "||", "/*", "*/", "**", "//"};

std::vector<std::string> Tokenizer::process(std::string in) {
    std::vector<std::string> data;
    std::string word;
    bool isString = false;
    bool isMultilineComment = false;
    bool isLineComment = false;
    for (char c : in) {
        std::string str(1, c); // C as a string
        if (!isString) {
            if (!isLineComment) {
                if (word + str == "/*") {
                    isMultilineComment = true;
                    word = "";
                    continue;
                }
                if (word + str == "*/") { // need to add error checking for improper placement
                    isMultilineComment = false;
                    word = "";
                    continue;
                }
            }
            if (!isMultilineComment) {
                if (word + str == "//") {
                    isLineComment = true;
                    word = "";
                    continue;
                }
                if (str == "\n") {
                    if (isLineComment) {
                        isLineComment = false;
                    } else {
                        data.push_back(word);
                        word = "";
                    }
                    continue;
                }
            } else if (c == '*') {
                word = str;
                continue;
            } else {
                continue;
            }
            if (isspace(c)) {
                if (word.length()) {
                    data.push_back(word);
                    word = "";
                }
                continue;
            }
        }
        if (!isMultilineComment && !isLineComment) {
            if (c == '"') {
                isString = !isString;
            }
        }

        if (isalpha(c) || isdigit(c) || isspace(c)) {
            if (isMultilineComment || isLineComment) {
                continue;
            }
            if (word.length()) {
                if (isdigit(word[0]) && isalpha(c)) {
                    data.push_back(word);
                    word = str;
                    continue;
                } else {
                    word.append(str);
                    continue;
                }
            } else {
                word = str;
                continue;
            }
        } else {
            if (word.length()) {
                if (isalpha(word[0]) || isdigit(word[0])) {
                    data.push_back(word);
                    word = "";
                }
            }
            if (std::any_of(combinedSymbols.begin(), combinedSymbols.end(), [word, str](std::string t) { return t.rfind(word + str, 0) == 0; })) {
                // Combined symbols code
                if (std::any_of(combinedSymbols.begin(), combinedSymbols.end(), [word, str](std::string t) { return t == word + str; })) {
                    data.push_back(word + str);
                    word = "";
                    continue;
                } else {
                    word.append(str);
                    continue;
                }
            }
        }
        data.push_back(str);
    }
    if (word.length()) {
        data.push_back(word);
    }
    return data;
}



// int main() {
//     Tokenizer tokenizer;
//     std::string in;
//     std::cout << "Enter code:" << std::endl;
//     std::getline(std::cin, in);
//     std::vector<std::string> out_vec = tokenizer.process(in);
//     std::string out;
//     std::for_each(out_vec.begin(), out_vec.end() - 1, [&out](std::string t) { out.append(t + ", "); });
//     std::cout << "[" << out << out_vec[out_vec.size() - 1] << "]" << std::endl;
//     std::cin.ignore();
// }
