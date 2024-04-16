//
// Created by Jan Halemba on 16/04/2024.
//

#ifndef ECOTE_PROJECT_UTILITY_H
#define ECOTE_PROJECT_UTILITY_H

#include <string>

bool isOperator(char c);
int precedence(char op);
std::string trim(const std::string& str);

#endif //ECOTE_PROJECT_UTILITY_H
