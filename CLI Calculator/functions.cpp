#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <stdexcept>
#include <vector>

#include "functions.hpp"

double add(double a, double b) {
    double result = a + b;
    // std::cout << "Result: " << result << std::endl;
    return result;
}

double subtract(double a, double b) {
    double result = a - b;
    // std::cout << "Result: " << result << std::endl;
    return result;
}
double multiply(double a, double b) {
    double result = a * b;
    // std::cout << "Result: " << result << std::endl;
    return result;
}
double divide(double a, double b) {
    if (b == 0) {
        // invalid operation
        throw std::invalid_argument("Division by zero");
    }
    double result = a / b;
    // std::cout << "Result: " << result << std::endl;
    return result;
}
double power(double base, double exponent) {
    if (base < 0 && exponent != static_cast<int>(exponent)) {
        throw std::invalid_argument("Negative base with non-integer exponent");
    }
    double result = std::pow(base, exponent);
    // std::cout << "Result: " << result << std::endl;
    return result;
}
double square_root(double value) {
    if (value < 0) {
        throw std::invalid_argument("Square root of negative number");
    }
    double result = std::sqrt(value);
    // std::cout << "Result: " << result << std::endl;
    return result;
}
double modulus(double a, double b) {
    if (b == 0) {
        throw std::invalid_argument("Division by zero");
    }
    double result = static_cast<int>(a) % static_cast<int>(b);
    // std::cout << "Result: " << result << std::endl;
    return result;
}

int precedence(const std::string& operation) {
    if (operation == "+" || operation == "-") {
        return 1;
    } else if (operation == "*" || operation == "/" || operation == "%") {
        return 2;
    } else if (operation == "^") {
        return 3;
    }
    else if (operation == "s") { // square root
        return 4; // Highest precedence for square root
    }
    return 0; // Default precedence for unknown operators
}

bool is_operator(const std::string& c) {
    return c == "+" || c == "-" || c == "*" || c == "/" || c == "^" || c == "%" || c == "s";
}

std::vector<std::string> tokenizer(const std::string& expression) {
    std::vector<std::string> tokens;
    tokens.reserve(expression.size() / 2); // Reserve space for tokens to avoid multiple reallocations
    

    // add logic to support unary operators (eg -5 or +3)
    auto is_unary_operator = [&](size_t index) {
        if (tokens.empty()) return true; // If it's the first token, it can be unary
        const auto &prev = tokens.back();
        return is_operator(prev) || prev == "(";
    };
    
    size_t i = 0;

    while (i < expression.size()) {
        char c = expression[i];

        if(std::isspace(c)) {
            i++;
            continue; // Skip whitespace
        }

        // unary operators
        if((c == '-' || c == '+') && i + 1 < expression.size() && std::isdigit(expression[i + 1]) && is_unary_operator(i)) {
            // treat leading + / - as part of next digit
            size_t start = i; // Start of the number
            bool is_decimal = false; // Track if we have seen a decimal point
            ++i; // Move past the unary operator
            // scan digits and decimal point
            while (i < expression.size() && (std::isdigit(expression[i]) || (!is_decimal && expression[i] == '.'))) {
                if (expression[i] == '.') is_decimal = true; // Allow only one decimal point
                ++i; // Move to the next character
            }
            tokens.push_back(expression.substr(start, i - start)); // Extract the number with unary operator
            continue; // Skip to the next iteration
        }
        
        // numbers: integers or decimals
        if(std::isdigit(c) || c == '.') { // Check if the character is a digit or a decimal point
            size_t start = i; // Start of the number
            bool is_decimal = (c == '.'); // Track if we have seen a decimal point
            while( i + 1 < expression.size() && (std::isdigit(expression[i + 1]) || (!is_decimal && expression[i + 1] == '.'))) { // Continue while next character is a digit or decimal point
                if (expression[i + 1] == '.') { // Check for decimal point
                    is_decimal = true; // Allow only one decimal point
                }
                ++i; // Move to the next character
            }
            tokens.push_back(expression.substr(start, i - start + 1)); // Extract the number
            ++i; // Move past the last character of the number
            continue;
        }

        // name: functions or variables
        if(std::isalpha(c)) {
            size_t start = i; // Start of the name
            while (i + 1 < expression.size() && std::isalpha(expression[i + 1])) { // Continue while next character is an alphabetic character
                ++i;
            }
            tokens.push_back(expression.substr(start, i - start + 1)); // Extract the name
            ++i; // Move past the last character of the name
            continue;
        }

        // single character operators or parentheses
        const std::string single{c}; // Define single character operators
        tokens.push_back(single);
        ++i;
    }

    return tokens;
}

// associativity helper function
bool is_left_associative(const std::string& operation) {
    return operation != "^"; // All operators except exponentiation are left associative
}

// Shunting Yard Algorithm
std::vector<std::string> shunting_yard(const std::vector<std::string>& tokens) {
    std::vector<std::string> output;
    std::vector<std::string> operators; // operators stack

    for (const auto& token : tokens) {
        if(std::isdigit(token[0]) || (token[0] == '.') || ((token[0] == '-' || token[0] == '+') && token.size() > 1 && token.size() > 1 && std::isdigit(token[1]))) {
            output.push_back(token); // If the token is a number, add it to the output
        }


        // operator
        else if (is_operator(token)) {
            while(!operators.empty() && is_operator(operators.back())) {
                int precedence1 = precedence(operators.back());
                int precedence2 = precedence(token);

                if((precedence1 > precedence2) || (precedence1 == precedence2 && is_left_associative(token))) {
                    output.push_back(operators.back()); // Pop the operator from the stack to the output
                    operators.pop_back(); // Remove the operator from the stack
                    continue; // Continue to check the next operator
                }
                break; // If the precedence is lower, break the loop
            }
            operators.push_back(token); // Push the current operator onto the stack
            
        } else if (token == "(") { // left parenthesis
            operators.push_back(token);
        } else if (token == ")") { // right parenthesis
            bool found_left_parenthesis = false;
            while (!operators.empty()) {
                if (operators.back() == "(") {
                    found_left_parenthesis = true; // Found the matching left parenthesis
                    operators.pop_back(); // Pop the left parenthesis from the stack
                    break; // Break the loop after popping
                }
                output.push_back(operators.back()); // Pop operators until we find a left parenthesis
                operators.pop_back();
            }
            if (!found_left_parenthesis) {
                throw std::invalid_argument("Mismatched parentheses in expression");
            }


        } else {
            if(token == "s") { // square root
                operators.push_back(token); // Push the square root operator onto the stack
            } else {
                throw std::invalid_argument("Unknown token: " + token); // Handle unknown tokens
            }
        }
    }
    // Pop all remaining operators from the stack to the output
    while (!operators.empty()) {
        if (operators.back() == "(" || operators.back() == ")") {
            throw std::invalid_argument("Mismatched parentheses in expression");
        }
        output.push_back(operators.back());
        operators.pop_back();
    }
    
    return output; // Return the output in Reverse Polish Notation (RPN)

}

void calculate(const std::vector<std::string>& tokens) {

    std::vector<double> stack;
    double result;
    double operand2;
    double operand1;
    for (const auto& token : tokens) {
        if (is_operator(token)) {
            if (stack.size() < 2) {
                operand1 = stack.back();
                stack.pop_back();
            } else {
                operand2 = stack.back();
                stack.pop_back();
                operand1 = stack.back();
                stack.pop_back();
            }

            if (token == "+") {
                result = add(operand1, operand2);
            } else if (token == "-") {
                result = subtract(operand1, operand2);
            } else if (token == "*") {
                result = multiply(operand1, operand2);
            } else if (token == "/") {
                result = divide(operand1, operand2);
                if (result == 1) {
                    std::cout << "Division by zero" << std::endl;
                    return;
                }
            } else if (token == "^") {
                result = power(operand1, operand2);
            } else if (token == "s") {
                result = square_root(operand1);
            } else if (token == "%") {
                result = modulus(operand1, operand2);
            } else {
                throw std::invalid_argument("Unknown operator: " + token);
            }
            stack.push_back(result); // Push the result back onto the stack
        } else {
            stack.push_back(static_cast<double>(std::stod(token)));
        }
    }
    if (stack.size() != 1) {
        throw std::invalid_argument("Invalid expression");
    }
    std::cout << "Final Result: " << stack.back() << std::endl;
}