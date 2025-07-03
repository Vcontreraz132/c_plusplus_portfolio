#include <iostream>
#include <string>
#include <vector>

double add(double a, double b);
double subtract(double a, double b);
double multiply(double a, double b);
double divide(double a, double b);
double power(double base, double exponent);
double square_root(double value);
double modulus(double a, double b);

bool is_left_associative(const std::string& operation);


int precedence(const std::string& operation);

bool is_operator(const std::string& c);

std::vector<std::string> tokenizer(const std::string& expression);

std::vector<std::string> shunting_yard(const std::vector<std::string>& tokens);

void calculate(const std::vector<std::string>& tokens);