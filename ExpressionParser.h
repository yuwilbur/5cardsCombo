#pragma once

#include <string>
#include <stack>
#include <vector>
#include <cctype>

namespace ExpressionParser {
  /// <summary>
  /// Converts infix notation to postfix notation by using Shunting-yard algorithm
  /// </summary>
  std::vector<std::string> InfixToPostfix(const std::vector<std::string>& infix) {
    std::vector<std::string> postfix = {};
    postfix.reserve(infix.size());
    std::stack<std::string> operatorStack;
    for (auto& token : infix) {
      const bool isDigit = (std::isdigit(token[0]) != 0);
      if (isDigit) {
        postfix.push_back(token);
        continue;
      }
      operatorStack.push(token);
    }
    while (!operatorStack.empty()) {
      postfix.push_back(operatorStack.top());
      operatorStack.pop();
    }
    return postfix;
  }

  int EvaluateExpression(const std::vector<std::string>& expression) {
    const auto evaluatePostfix = [](const std::string& leftOperand, const std::string& rightOperand, const std::string& operatorValue) -> int {
      const int leftValue = std::stoi(leftOperand);
      const int rightValue = std::stoi(rightOperand);
      if (operatorValue == "*") return leftValue * rightValue;
      if (operatorValue == "/") return leftValue / rightValue;
      if (operatorValue == "-") return leftValue - rightValue;
      if (operatorValue == "+") return leftValue + rightValue;
      throw std::invalid_argument("Operator is not supported");
    };
    std::vector<std::string> postfix = InfixToPostfix(expression);
    for (size_t i = 0; i < postfix.size(); ++i) {
      if (std::isdigit(postfix[i][0]))
        continue;
      postfix[i] = std::to_string(evaluatePostfix(postfix[i - 2], postfix[i - 1], postfix[i]));
      postfix.erase(postfix.begin() + i - 2);
      postfix.erase(postfix.begin() + i - 2);
      i -= 2;
    }
    if (postfix.size() != 1)
      throw std::runtime_error("Expression failed to be evaluated");
    return std::stoi(postfix[0]);
  }
}