//
// Created by Jan Halemba on 16/04/2024.
//

#ifndef ECOTE_PROJECT_EXPRESSIONEVALUATOR_H
#define ECOTE_PROJECT_EXPRESSIONEVALUATOR_H

#include <vector>
#include <string>
#include <unordered_map>

class ExpressionEvaluator {
public:
    int evaluateExpression(const std::string& expr, const std::unordered_map<std::string, int>& consts);
private:
    std::vector<std::string> infixToRPN(const std::string& expr, const std::unordered_map<std::string, int>& consts);
    int evaluateRPN(const std::vector<std::string>& rpn);
};


#endif //ECOTE_PROJECT_EXPRESSIONEVALUATOR_H
