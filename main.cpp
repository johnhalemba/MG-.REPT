#include <iostream>
#include "MacroProcessor.h"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <input file> <output file> <error file>" << std::endl;
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    std::string errorFile = argv[3];

    MacroProcessor::processFile(inputFile, outputFile, errorFile);

    return 0;
}