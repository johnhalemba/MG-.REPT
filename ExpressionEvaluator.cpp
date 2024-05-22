//
// Created by Jan Halemba on 16/04/2024.
//

#include "ExpressionEvaluator.h"
#include "Utility.h"
#include <stack>
#include <stdexcept>

std::vector<std::string> ExpressionEvaluator::infixToRPN (const std::string& expr, const std::unordered_map<std::string, int>& consts) {
    std::stack<char> operators;
    std::vector<std::string> output;
    std::string numberOrConst;
    bool lastWasOperator = true;

    for (char ch : expr) {
        if (std::isdigit(ch) || std::isalpha(ch)) {
            numberOrConst += ch;
            lastWasOperator = false;
        } else if (isOperator(ch)) {
            if (lastWasOperator) {
                throw std::runtime_error("Missing operand.");
            }
            if (!numberOrConst.empty()) {
                if (std::isdigit(numberOrConst[0])) {
                    output.push_back(numberOrConst);
                } else {
                    if (consts.find(numberOrConst) == consts.end()) {
                        throw std::runtime_error("Undefined constant: " + numberOrConst);
                    }
                    output.push_back(std::to_string(consts.at(numberOrConst)));
                }
                numberOrConst.clear();
            }
            while (!operators.empty() && precedence(operators.top()) >= precedence(ch)) {
                output.push_back(std::string(1, operators.top()));
                operators.pop();
            }
            operators.push(ch);
            lastWasOperator = true;
        } else if (!std::isspace(ch)) {
            throw std::runtime_error(std::string("Invalid character: ") + ch);
        }
    }
    if(lastWasOperator && !output.empty()) {
        throw std::runtime_error("Missing operand.");
    }
    if (!numberOrConst.empty()) {
        if (std::isdigit(numberOrConst[0])) {
            output.push_back(numberOrConst);
        } else {
            if (consts.find(numberOrConst) == consts.end()) {
                throw std::runtime_error("Undefined constant: " + numberOrConst);
            }
            output.push_back(std::to_string(consts.at(numberOrConst)));
        }
        numberOrConst.clear();
    }
    while (!operators.empty()) {
        if (isOperator(operators.top())) {
            output.push_back(std::string(1, operators.top()));
            operators.pop();
        } else {
            throw std::runtime_error("Invalid operator in stack.");
        }
    }
    return output;
}

int ExpressionEvaluator::evaluateRPN(const std::vector<std::string>& rpn) {
    std::stack<int> values;
    for (const auto& token : rpn) {
        if (isOperator(token[0]) && token.size() == 1) {
            if (values.size() < 2) {
                throw std::runtime_error("Missing operand.");
            }
            int right = values.top(); values.pop();
            int left = values.top(); values.pop();
            switch (token[0]) {
                case '+': values.push(left + right); break;
                case '-': values.push(left - right); break;
                case '*': values.push(left * right); break;
                case '/':
                    if (right == 0) throw std::runtime_error("Division by zero.");
                    values.push(left / right);
                    break;
            }
        } else {
            values.push(std::stoi(token));
        }
    }
    return values.top();
}

int ExpressionEvaluator::evaluateExpression(const std::string& expr, const std::unordered_map<std::string, int>& consts) {
    auto rpn = infixToRPN(expr, consts);
    return evaluateRPN(rpn);
}