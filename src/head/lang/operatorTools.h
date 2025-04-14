#pragma once
#include <any>
#include <stdexcept>
#include <string>

namespace OperatorTools {

    // Addition
    std::any add(const std::any& a, const std::any& b) {
        if (a.type() == typeid(int) && b.type() == typeid(int)) {
            return std::any_cast<int>(a) + std::any_cast<int>(b);
        }
        if (a.type() == typeid(double) && b.type() == typeid(double)) {
            return std::any_cast<double>(a) + std::any_cast<double>(b);
        }
        if (a.type() == typeid(float) && b.type() == typeid(float)) {
            return std::any_cast<float>(a) + std::any_cast<float>(b);
        }
        if (a.type() == typeid(std::string) && b.type() == typeid(std::string)) {
            return std::any_cast<std::string>(a) + std::any_cast<std::string>(b);
        }
        throw std::invalid_argument("Unsupported types for addition");
    }

    // Subtraction
    std::any subtract(const std::any& a, const std::any& b) {
        if (a.type() == typeid(int) && b.type() == typeid(int)) {
            return std::any_cast<int>(a) - std::any_cast<int>(b);
        }
        if (a.type() == typeid(double) && b.type() == typeid(double)) {
            return std::any_cast<double>(a) - std::any_cast<double>(b);
        }
        if (a.type() == typeid(float) && b.type() == typeid(float)) {
            return std::any_cast<float>(a) - std::any_cast<float>(b);
        }
        throw std::invalid_argument("Unsupported types for subtraction");
    }

    // Multiplication
    std::any multiply(const std::any& a, const std::any& b) {
        if (a.type() == typeid(int) && b.type() == typeid(int)) {
            return std::any_cast<int>(a) * std::any_cast<int>(b);
        }
        if (a.type() == typeid(double) && b.type() == typeid(double)) {
            return std::any_cast<double>(a) * std::any_cast<double>(b);
        }
        if (a.type() == typeid(float) && b.type() == typeid(float)) {
            return std::any_cast<float>(a) * std::any_cast<float>(b);
        }
        throw std::invalid_argument("Unsupported types for multiplication");
    }

    // Division
    std::any divide(const std::any& a, const std::any& b) {
        if (a.type() == typeid(int) && b.type() == typeid(int)) {
            if (std::any_cast<int>(b) == 0) throw std::invalid_argument("Division by zero");
            return std::any_cast<int>(a) / std::any_cast<int>(b);
        }
        if (a.type() == typeid(double) && b.type() == typeid(double)) {
            if (std::any_cast<double>(b) == 0.0) throw std::invalid_argument("Division by zero");
            return std::any_cast<double>(a) / std::any_cast<double>(b);
        }
        if (a.type() == typeid(float) && b.type() == typeid(float)) {
            if (std::any_cast<float>(b) == 0.0f) throw std::invalid_argument("Division by zero");
            return std::any_cast<float>(a) / std::any_cast<float>(b);
        }
        throw std::invalid_argument("Unsupported types for division");
    }

    // Modulus
    std::any modulus(const std::any& a, const std::any& b) {
        if (a.type() == typeid(int) && b.type() == typeid(int)) {
            return std::any_cast<int>(a) % std::any_cast<int>(b);
        }
        throw std::invalid_argument("Unsupported types for modulus");
    }

    // Logical NOT
    std::any logicalNot(const std::any& a) {
        if (a.type() == typeid(bool)) {
            return !std::any_cast<bool>(a);
        }
        throw std::invalid_argument("Unsupported type for logical NOT");
    }

    // Negation
    std::any negate(const std::any& a) {
        if (a.type() == typeid(int)) {
            return -std::any_cast<int>(a);
        }
        if (a.type() == typeid(double)) {
            return -std::any_cast<double>(a);
        }
        if (a.type() == typeid(float)) {
            return -std::any_cast<float>(a);
        }
        throw std::invalid_argument("Unsupported type for negation");
    }

    // Equality
    std::any equals(const std::any& a, const std::any& b) {
        if (a.type() == b.type()) {
            if (a.type() == typeid(int)) {
                return std::any_cast<int>(a) == std::any_cast<int>(b);
            }
            if (a.type() == typeid(double)) {
                return std::any_cast<double>(a) == std::any_cast<double>(b);
            }
            if (a.type() == typeid(float)) {
                return std::any_cast<float>(a) == std::any_cast<float>(b);
            }
            if (a.type() == typeid(bool)) {
                return std::any_cast<bool>(a) == std::any_cast<bool>(b);
            }
            if (a.type() == typeid(std::string)) {
                return std::any_cast<std::string>(a) == std::any_cast<std::string>(b);
            }
        }
        throw std::invalid_argument("Unsupported types for equality");
    }

    // Greater than or equal
    std::any greaterThanOrEqual(const std::any& a, const std::any& b) {
        if (a.type() == typeid(int) && b.type() == typeid(int)) {
            return std::any_cast<int>(a) >= std::any_cast<int>(b);
        }
        if (a.type() == typeid(double) && b.type() == typeid(double)) {
            return std::any_cast<double>(a) >= std::any_cast<double>(b);
        }
        if (a.type() == typeid(float) && b.type() == typeid(float)) {
            return std::any_cast<float>(a) >= std::any_cast<float>(b);
        }
        throw std::invalid_argument("Unsupported types for comparison");
    }

    // Less than or equal
    std::any lessThanOrEqual(const std::any& a, const std::any& b) {
        if (a.type() == typeid(int) && b.type() == typeid(int)) {
            return std::any_cast<int>(a) <= std::any_cast<int>(b);
        }
        if (a.type() == typeid(double) && b.type() == typeid(double)) {
            return std::any_cast<double>(a) <= std::any_cast<double>(b);
        }
        if (a.type() == typeid(float) && b.type() == typeid(float)) {
            return std::any_cast<float>(a) <= std::any_cast<float>(b);
        }
        throw std::invalid_argument("Unsupported types for comparison");
    }

    // Greater than
    std::any greaterThan(const std::any& a, const std::any& b) {
        if (a.type() == typeid(int) && b.type() == typeid(int)) {
            return std::any_cast<int>(a) > std::any_cast<int>(b);
        }
        if (a.type() == typeid(double) && b.type() == typeid(double)) {
            return std::any_cast<double>(a) > std::any_cast<double>(b);
        }
        if (a.type() == typeid(float) && b.type() == typeid(float)) {
            return std::any_cast<float>(a) > std::any_cast<float>(b);
        }
        throw std::invalid_argument("Unsupported types for comparison");
    }

    // Less than
    std::any lessThan(const std::any& a, const std::any& b) {
        if (a.type() == typeid(int) && b.type() == typeid(int)) {
            return std::any_cast<int>(a) < std::any_cast<int>(b);
        }
        if (a.type() == typeid(double) && b.type() == typeid(double)) {
            return std::any_cast<double>(a) < std::any_cast<double>(b);
        }
        if (a.type() == typeid(float) && b.type() == typeid(float)) {
            return std::any_cast<float>(a) < std::any_cast<float>(b);
        }
        throw std::invalid_argument("Unsupported types for comparison");
    }

    // Not equal
    std::any notEquals(const std::any& a, const std::any& b) {
        if (a.type() == b.type()) {
            if (a.type() == typeid(int)) {
                return std::any_cast<int>(a) != std::any_cast<int>(b);
            }
            if (a.type() == typeid(double)) {
                return std::any_cast<double>(a) != std::any_cast<double>(b);
            }
            if (a.type() == typeid(float)) {
                return std::any_cast<float>(a) != std::any_cast<float>(b);
            }
            if (a.type() == typeid(bool)) {
                return std::any_cast<bool>(a) != std::any_cast<bool>(b);
            }
            if (a.type() == typeid(std::string)) {
                return std::any_cast<std::string>(a) != std::any_cast<std::string>(b);
            }
        }
        throw std::invalid_argument("Unsupported types for inequality");
    }

    // Logical OR
    std::any logicalOr(const std::any& a, const std::any& b) {
        if (a.type() == typeid(bool) && b.type() == typeid(bool)) {
            return std::any_cast<bool>(a) || std::any_cast<bool>(b);
        }
        throw std::invalid_argument("Unsupported types for logical OR");
    }

    // Logical AND
    std::any logicalAnd(const std::any& a, const std::any& b) {
        if (a.type() == typeid(bool) && b.type() == typeid(bool)) {
            return std::any_cast<bool>(a) && std::any_cast<bool>(b);
        }
        throw std::invalid_argument("Unsupported types for logical AND");
    }

}