#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

#include "functions.hpp"

    int main() {
        bool running = true;
        std::cout << "Welcome to the CLI Calculator!" << std::endl;
        std::cout << "Enter 'exit' to quit the program." << std::endl;

        while (running) {
            std::string input;
            std::cout << "Enter an expression (e.g., 5 + 3): ";
            std::getline(std::cin, input);

            if (input == "exit") {
                running = false;
                continue;
            }

            
            // try catch block to handle errors in expression
            try {
                auto tokens = tokenizer(input); // Tokenize the input expression
                auto rpn = shunting_yard(tokens); // Convert the tokens to Reverse Polish Notation (RPN)
                std::cout << "Result: ";
                calculate(rpn);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error: " << e.what() << std::endl;
                continue; // Skip to the next iteration if there's an error
            } catch (const std::exception& e) {
                std::cerr << "An unexpected error occurred: " << e.what() << std::endl;
                continue; // Skip to the next iteration if there's an unexpected error
            }
        }

        return 0;
    }