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
      // 1. If digit, push to output
      // 2. If "(", push to stack
      // 3. If ")", pop stack until "(" is found
      // 4. If operator, pop all higher precedence in stack then push current operator to stack
      if (std::isdigit(token[0]) != 0) {
        postfix.push_back(token);
      }
      else if (token == "(") {
        operatorStack.push(token);
      }
      else if (token == ")") {
        while (!operatorStack.empty()) {
          const std::string op = operatorStack.top();
          operatorStack.pop();
          if (op == "(")
            break;
          postfix.push_back(op);
        }
      }
      else {
        if (token == "+" || token == "-") {
          while (!operatorStack.empty()) {
            const std::string op = operatorStack.top();
            if (op != "(") {
              postfix.push_back(op);
              operatorStack.pop();
            }
            else {
              break;
            }
          }
        }
        if (token == "*" || token == "/") {
          while (!operatorStack.empty()) {
            const std::string op = operatorStack.top();
            if (op != "(" && op != "+" && op != "-") {
              postfix.push_back(op);
              operatorStack.pop();
            }
            else {
              break;
            }
          }
        }
        operatorStack.push(token);
      }
    }
    while (!operatorStack.empty()) {
      postfix.push_back(operatorStack.top());
      operatorStack.pop();
    }
    return postfix;
  }

  float EvaluateExpression(const std::vector<std::string>& expression) {
    const auto evaluatePostfix = [](const std::string& leftOperand, const std::string& rightOperand, const std::string& operatorValue) -> float {
      const float leftValue = std::stof(leftOperand);
      const float rightValue = std::stof(rightOperand);
      if (operatorValue == "*") return leftValue * rightValue;
      if (operatorValue == "-") return leftValue - rightValue;
      if (operatorValue == "+") return leftValue + rightValue;
      if (operatorValue == "/") {
        if (rightValue == 0.f) return 99999.9f;
        return leftValue / rightValue;
      }
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
    return std::stof(postfix[0]);
  }
}