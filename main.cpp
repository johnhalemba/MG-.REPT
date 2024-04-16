#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

struct RepeatBlock {
    int count;
    std::vector<std::string> lines;
};

std::vector<std::string> infixToRPN (const std::string& expr, const std::unordered_map<std::string, int>& consts) {
    std::stack<char> operators;
    std::vector<std::string> output;
    std::string numberOrConst;

    for (char ch : expr) {
        if (std::isdigit(ch) || std::isalpha(ch)) {
            numberOrConst += ch;
        } else if (isOperator(ch)) {
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
        }
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
        output.push_back(std::string(1, operators.top()));
        operators.pop();
    }
    return output;
}

int evaluateRPN(const std::vector<std::string>& rpn) {
    std::stack<int> values;
    for (const auto& token : rpn) {
        if (isOperator(token[0]) && token.size() == 1) {
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

int evaluateExpression(const std::string& expr, const std::unordered_map<std::string, int>& consts) {
    auto rpn = infixToRPN(expr, consts);
    return evaluateRPN(rpn);
}

void processFile(const std::string& inputFile, const std::string& outputFile, const std::string& errorFile) {
    std::ifstream input(inputFile);
    std::ofstream output(outputFile);
    std::ofstream errors(errorFile);

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
            int count = evaluateExpression(expr, constants);
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

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <input file> <output file> <error file>" << std::endl;
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    std::string errorFile = argv[3];

    processFile(inputFile, outputFile, errorFile);

    return 0;
}