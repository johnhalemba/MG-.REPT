//
// Created by Jan Halemba on 16/04/2024.
//

#ifndef ECOTE_PROJECT_MACROPROCESSOR_H
#define ECOTE_PROJECT_MACROPROCESSOR_H

#include <string>

class MacroProcessor {
public:
    void processFile(const std::string& inputFile, const std::string& outputFile, const std::string& errorFile);
};


#endif //ECOTE_PROJECT_MACROPROCESSOR_H
