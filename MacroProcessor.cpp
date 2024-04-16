//
// Created by Jan Halemba on 16/04/2024.
//

#include "MacroProcessor.h"
#include "ExpressionEvaluator.h"
#include "Utility.h"
#include <fstream>
#include <sstream>
#include <stack>
#include <unordered_map>
#include <vector>
#include <string>

struct RepeatBlock {
    int count;
    std::vector<std::string> lines;
};

void MacroProcessor::processFile(const std::string& inputFile, const std::string& outputFile, const std::string& errorFile) {
    std::ifstream input(inputFile);
    std::ofstream output(outputFile);
    std::ofstream errors(errorFile);

    ExpressionEvaluator evaluator;
    std::unordered_map<std::string, int> constants;
    std::stack<RepeatBlock> repeatStack;
    std::string line;

    int lineCount = 0;

    while (getline(input, line)) {
        ++lineCount;
        line = trim(line);
        if (line.empty() || line[0] == '#')
        {
            ++lineCount;
            continue;
        }

        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;

        if (cmd == ".CONST") {
            std::string constDef;
            std::getline(iss, constDef);
            constDef = trim(constDef);
            size_t equalsPos = constDef.find('=');
            if (equalsPos == std::string::npos) {
                errors << "Syntax error in .CONST definition in line: "<< lineCount << std::endl;
                continue;
            }
            std::string name = constDef.substr(0, equalsPos);
            int value = std::stoi(constDef.substr(equalsPos + 1));
            constants[name] = value;
        } else if (cmd == ".REPT") {
            std::string expr;
            iss >> expr;
            if (expr.empty())
            {
                errors << "No expression detected for .REPT in line: " << lineCount << std::endl;
                continue;
            }
            int count = evaluator.evaluateExpression(expr, constants);
            repeatStack.push({count, {}});
        } else if (cmd == ".ENDM") {
            if (repeatStack.empty()) {
                errors << "Unmached .ENDM in line: " << lineCount << std::endl;
                continue;
            }
            auto block = repeatStack.top();
            repeatStack.pop();
            std::vector<std::string> expanded;
            for (int i = 0; i < block.count; ++i)
            {
                expanded.insert(expanded.end(), block.lines.begin(), block.lines.end());
            }
            if (repeatStack.empty()) {
                for (const auto& line : expanded) {
                    output << line << std::endl;
                }
            } else {
                repeatStack.top().lines.insert(repeatStack.top().lines.end(), expanded.begin(), expanded.end());
            }
        } else if (cmd[0] == '.') {
            errors << "Syntax error: Unrecognized command in line " << lineCount << ": " << cmd << std::endl;
        } else {
            if (!repeatStack.empty()) {
                repeatStack.top().lines.push_back(line);
            } else {
                output << line << std::endl;
            }
        }
    }

    if (!repeatStack.empty()) {
        errors << "Missing .ENDM for .REPT block" << std::endl;
    }

    input.close();
    output.close();
    errors.close();
}
