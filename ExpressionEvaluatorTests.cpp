#include <gtest/gtest.h>
#include "ExpressionEvaluator.h"

class ExpressionEvaluatorTest : public ::testing::Test {
protected:
    ExpressionEvaluator evaluator;
};

TEST_F(ExpressionEvaluatorTest, EvaluateExpression_SimpleAddition) {
    std::unordered_map<std::string, int> consts;
    std::string expr = "3+5";
    int result = evaluator.evaluateExpression(expr, consts);
    EXPECT_EQ(result, 8);
}

TEST_F(ExpressionEvaluatorTest, EvaluateExpression_SimpleSubtraction) {
    std::unordered_map<std::string, int> consts;
    std::string expr = "10-2";
    int result = evaluator.evaluateExpression(expr, consts);
    EXPECT_EQ(result, 8);
}

TEST_F(ExpressionEvaluatorTest, EvaluateExpression_SimpleMultiplication) {
    std::unordered_map<std::string, int> consts;
    std::string expr = "4*2";
    int result = evaluator.evaluateExpression(expr, consts);
    EXPECT_EQ(result, 8);
}

TEST_F(ExpressionEvaluatorTest, EvaluateExpression_SimpleDivision) {
    std::unordered_map<std::string, int> consts;
    std::string expr = "16/2";
    int result = evaluator.evaluateExpression(expr, consts);
    EXPECT_EQ(result, 8);
}

TEST_F(ExpressionEvaluatorTest, EvaluateExpression_MultipleOperations) {
    std::unordered_map<std::string, int> consts;
    std::string expr = "3+5*2-8/4";
    int result = evaluator.evaluateExpression(expr, consts);
    EXPECT_EQ(result, 11);
}

TEST_F(ExpressionEvaluatorTest, EvaluateExpression_WithConstants) {
    std::unordered_map<std::string, int> consts = {{"PI", 3}, {"E", 2}};
    std::string expr = "PI+E*2";
    int result = evaluator.evaluateExpression(expr, consts);
    EXPECT_EQ(result, 7);
}

TEST_F(ExpressionEvaluatorTest, EvaluateExpression_InvalidCharacter) {
    std::unordered_map<std::string, int> consts;
    std::string expr = "3+5*2-8/4$";
    EXPECT_THROW(evaluator.evaluateExpression(expr, consts), std::runtime_error);
}

TEST_F(ExpressionEvaluatorTest, EvaluateExpression_MissingOperand) {
    std::unordered_map<std::string, int> consts;
    std::string expr = "3+";
    EXPECT_THROW(evaluator.evaluateExpression(expr, consts), std::runtime_error);
}

TEST_F(ExpressionEvaluatorTest, EvaluateExpression_DivisionByZero) {
    std::unordered_map<std::string, int> consts;
    std::string expr = "3/0";
    EXPECT_THROW(evaluator.evaluateExpression(expr, consts), std::runtime_error);
}
