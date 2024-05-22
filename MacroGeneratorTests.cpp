#include <gtest/gtest.h>
#include "MacroProcessor.h"
#include <fstream>

class MacroProcessorTest : public ::testing::Test {
protected:
    MacroProcessor processor;

    void SetUp() override {
        std::ofstream testFile("test_input.txt");
        testFile << ".CONST PI=3\n";
        testFile << ".CONST E=2\n";
        testFile << ".REPT PI+1\n";
        testFile << "Line inside repeat block\n";
        testFile << ".ENDM\n";
        testFile << "Line outside repeat block\n";
        testFile.close();
    }

    void TearDown() override {
        std::remove("test_input.txt");
        std::remove("test_output.txt");
        std::remove("test_error.txt");
    }
};

TEST_F(MacroProcessorTest, ProcessFile_ValidInput) {
    processor.processFile("test_input.txt", "test_output.txt", "test_error.txt");

    std::ifstream outputFile("test_output.txt");
    std::string line;
    std::vector<std::string> lines;
    while (getline(outputFile, line)) {
        lines.push_back(line);
    }
    outputFile.close();

    ASSERT_EQ(lines.size(), 5);
    EXPECT_EQ(lines[0], "Line inside repeat block");
    EXPECT_EQ(lines[1], "Line inside repeat block");
    EXPECT_EQ(lines[2], "Line inside repeat block");
    EXPECT_EQ(lines[3], "Line inside repeat block");
    EXPECT_EQ(lines[4], "Line outside repeat block");

    std::ifstream errorFile("test_error.txt");
    std::string errorContent((std::istreambuf_iterator<char>(errorFile)), std::istreambuf_iterator<char>());
    errorFile.close();
    EXPECT_TRUE(errorContent.empty());
}

TEST_F(MacroProcessorTest, ProcessFile_InvalidConstDefinition) {
    std::ofstream testFile("test_input.txt", std::ios::app);
    testFile << ".CONST InvalidConstDefinition\n";
    testFile.close();

    processor.processFile("test_input.txt", "test_output.txt", "test_error.txt");

    std::ifstream errorFile("test_error.txt");
    std::string errorContent((std::istreambuf_iterator<char>(errorFile)), std::istreambuf_iterator<char>());
    errorFile.close();
    EXPECT_NE(errorContent.find("Syntax error in .CONST definition"), std::string::npos);
}

TEST_F(MacroProcessorTest, ProcessFile_RepeatWithoutEndm) {
    std::ofstream testFile("test_input.txt", std::ios::app);
    testFile << ".REPT 3\n";
    testFile << "Unfinished repeat block\n";
    testFile.close();

    processor.processFile("test_input.txt", "test_output.txt", "test_error.txt");

    std::ifstream errorFile("test_error.txt");
    std::string errorContent((std::istreambuf_iterator<char>(errorFile)), std::istreambuf_iterator<char>());
    errorFile.close();
    EXPECT_NE(errorContent.find("Missing .ENDM for .REPT block"), std::string::npos);
}

TEST_F(MacroProcessorTest, ProcessFile_SyntaxError) {
    std::ofstream testFile("test_input.txt", std::ios::app);
    testFile << ".UNKNOWNCOMMAND\n";
    testFile.close();

    processor.processFile("test_input.txt", "test_output.txt", "test_error.txt");

    std::ifstream errorFile("test_error.txt");
    std::string errorContent((std::istreambuf_iterator<char>(errorFile)), std::istreambuf_iterator<char>());
    errorFile.close();
    EXPECT_NE(errorContent.find("Syntax error: Unrecognized command"), std::string::npos);
}

TEST_F(MacroProcessorTest, ProcessFile_NestedRept) {
    std::ofstream testFile("test_input.txt", std::ios::trunc);
    testFile << ".REPT 2\n";
    testFile << "Outer line\n";
    testFile << ".REPT 3\n";
    testFile << "Inner line\n";
    testFile << ".ENDM\n";
    testFile << ".ENDM\n";
    testFile.close();

    processor.processFile("test_input.txt", "test_output.txt", "test_error.txt");

    std::ifstream outputFile("test_output.txt");
    std::string line;
    std::vector<std::string> lines;
    while (getline(outputFile, line)) {
        lines.push_back(line);
    }
    outputFile.close();

    ASSERT_EQ(lines.size(), 8);
    EXPECT_EQ(lines[0], "Outer line");
    EXPECT_EQ(lines[1], "Inner line");
    EXPECT_EQ(lines[2], "Inner line");
    EXPECT_EQ(lines[3], "Inner line");
    EXPECT_EQ(lines[4], "Outer line");
    EXPECT_EQ(lines[5], "Inner line");
    EXPECT_EQ(lines[6], "Inner line");
    EXPECT_EQ(lines[7], "Inner line");

    std::ifstream errorFile("test_error.txt");
    std::string errorContent((std::istreambuf_iterator<char>(errorFile)), std::istreambuf_iterator<char>());
    errorFile.close();
    EXPECT_TRUE(errorContent.empty());
}

TEST_F(MacroProcessorTest, ProcessFile_FreeText) {
    std::ofstream testFile("test_input.txt", std::ios::app);
    testFile << "This is the test line.\n";
    testFile.close();

    processor.processFile("test_input.txt", "test_output.txt", "test_error.txt");

    std::ifstream outputFile("test_output.txt");
    std::string line;
    std::vector<std::string> lines;
    while (getline(outputFile, line)) {
        lines.push_back(line);
    }
    outputFile.close();

    ASSERT_EQ(lines.size(), 6);
    EXPECT_EQ(lines[5], "This is the test line.");

    std::ifstream errorFile("test_error.txt");
    std::string errorContent((std::istreambuf_iterator<char>(errorFile)), std::istreambuf_iterator<char>());
    errorFile.close();
    EXPECT_TRUE(errorContent.empty());
}

TEST_F(MacroProcessorTest, ProcessFile_EmptyFile) {
    std::ofstream testFile("test_input.txt");
    testFile.close();

    processor.processFile("test_input.txt", "test_output.txt", "test_error.txt");

    std::ifstream outputFile("test_output.txt");
    std::string line;
    std::vector<std::string> lines;
    while (getline(outputFile, line)) {
        lines.push_back(line);
    }
    outputFile.close();

    EXPECT_TRUE(lines.empty());

    std::ifstream errorFile("test_error.txt");
    std::string errorContent((std::istreambuf_iterator<char>(errorFile)), std::istreambuf_iterator<char>());
    errorFile.close();
    EXPECT_TRUE(errorContent.empty());
}