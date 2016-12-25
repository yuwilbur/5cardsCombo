#pragma once

#include <array>
#include <vector>
#include <string>
#include <functional>

#include <fstream>
#include <iostream>

#include "ExpressionParser.h"

template<int CARDS>
class ComboVerifier
{
public:
  ComboVerifier() :
    primitiveOperations_({ "+", "-", "*", "/" })
  {
    Initialize();
  }

  std::vector<std::array<int, CARDS>> ConstructAllHands(std::vector<int> validCards) {
    if (validCards.size() == 0)
      throw std::invalid_argument("No valid cards detected");
    std::sort(validCards.begin(), validCards.end());
    std::vector<std::array<int, CARDS>> hands = {};
    std::array<size_t, CARDS> indexes = {};
    for (size_t i = 0; i < indexes.size(); ++i) {
      indexes[i] = i;
    }

    size_t indexesIndex = 0;
    while (indexes[0] < validCards.size() - indexes.size()) {
      std::array<int, CARDS> hand = {};
      for (size_t i = 0; i < indexes.size(); ++i) {
        hand[i] = validCards[indexes[i]];
      }
      bool isHandsNew = true;
      for (int i = hands.size() - 1; i >= 0; --i) {
        if (hands[i] == hand) {
          isHandsNew = false;
          break;
        }
      }
      if (isHandsNew) {
        hands.push_back(hand);
      }
        
      if (indexes[indexesIndex] + 1 != indexes[indexesIndex + 1]) {
        indexes[indexesIndex]++;
        continue;
      }
      while (indexes[indexesIndex] + 1 == indexes[indexesIndex + 1]) {
        indexesIndex++;
        if (indexesIndex == indexes.size() - 1)
          break;
      }
      indexes[indexesIndex]++;
      for (size_t i = 0; i < indexesIndex; ++i) {
        indexes[i] = i;
      }
      indexesIndex = 0;
    }

    return hands;
  }

  std::vector<std::array<int, CARDS>> VerifyAllHands(const int target, const std::vector<std::array<int, CARDS>>& hands) {
    std::vector<std::array<int, CARDS>> validHands = {};
    for (auto& hand : hands) {
      if (VerifyHand(target, hand))
        validHands.push_back(hand);
    }
    return validHands;
  }

protected:
  bool VerifyHand(const int target, const std::array<int, CARDS>& hand) {
    std::array<std::string, CARDS> handStr = {};
    for (size_t i = 0; i < hand.size(); ++i) {
      handStr[i] = std::to_string(hand[i]);
    }

    for (auto& order : order_) {
      for (auto& primitives : primitives_) {
        for (auto& brackets : brackets_) {
          std::vector<std::string> operation = {};
          for (size_t i = 0; i < brackets.size(); ++i) {
            const int index = brackets[i];
            if (index == -1)
              operation.push_back("(");
            else if (index == -2)
              operation.push_back(")");
            else {
              if (index % 2 == 0)
                operation.push_back(handStr[order[index / 2]]);
              else
                operation.push_back(primitiveOperations_[primitives[index / 2]]);
            }
          }
          if (ExpressionParser::EvaluateExpression(operation) == target) {
            std::string result = "";
            for (auto& token : operation) {
              result += token;
            }
            goodFile_ << result << std::endl;
            std::cout << ".";
            return true;
          }
        }
      }
    }
    std::string result = "";
    for (auto& token : hand) {
      result += std::to_string(token) + " ";
    }
    badFile_ << result << std::endl;
    std::cout << "X";
    return false;
  }

private:
  std::ofstream badFile_;
  std::ofstream goodFile_;
  std::vector<std::vector<int>> order_;
  std::vector<std::vector<int>> brackets_;
  std::vector<std::vector<int>> primitives_;
  const std::array<std::string, 4> primitiveOperations_;

  void Initialize() {
    std::vector<int> originalBlacket(CARDS * 2 - 1);
    for (size_t i = 0; i < originalBlacket.size(); ++i) {
      originalBlacket[i] = i;
    }
    brackets_.push_back(originalBlacket);
    for (size_t cycle = 0; cycle < CARDS - 2; ++cycle) {
      std::vector<std::vector<int>> brackets = {};
      for (int i = brackets_.size() - 1; i >= 0; --i) {
        if (brackets_[i].size() < brackets_.back().size())
          break;
        const std::vector<int> bracket = brackets_[i];
        for (size_t start = 0; start < bracket.size(); ++start) {
          // Look for number
          if (bracket[start] < 0 || bracket[start] % 2 != 0)
            continue;
          for (size_t end = start + 1; end < bracket.size(); ++end) {
            // If encounter a bracket, just immediately terminate
            if (bracket[end] < 0)
              break;
            // Look for number
            if (bracket[end] % 2 != 0)
              continue;
            // If both brackets are the both ends of the operation, then ignore this combination
            if (
              start == 0 && end == bracket.size() - 1 ||
              start != 0 && bracket[start - 1] == -1 && end != bracket.size() - 1 && bracket[end + 1] == -2
              )
              break;

            auto newBracket = bracket;
            newBracket.insert(newBracket.begin() + end + 1, -2);
            newBracket.insert(newBracket.begin() + start, -1);
            brackets.push_back(newBracket);
          }
        }
      }
      if (brackets.size() == 0)
        break;
      brackets_.insert(brackets_.end(), brackets.begin(), brackets.end());
    }

    std::vector<int> order(CARDS);
    for (size_t i = 0; i < order.size(); ++i) {
      order[i] = i;
    }
    for (size_t i = 0; i < CARDS; ++i) {
      order_.push_back(order);
      const int orderTemp = order[0];
      for (size_t j = 0; j < order.size() - 1; ++j) {
        order[j] = order[j + 1];
      }
      order[order.size() - 1] = orderTemp;
    }

    std::vector<int> primitives(CARDS - 1);
    primitives_.push_back(primitives);
    for (size_t i = 0; i < std::pow(primitiveOperations_.size(), primitives.size()) - 1; ++i) {
      primitives[0]++;
      for (size_t j = 0; j < primitives.size(); ++j) {
        if (primitives[j] < 4)
          break;
        primitives[j] = 0;
        primitives[j + 1]++;
      }
      primitives_.push_back(primitives);
    }

    goodFile_.open("good.txt");
    badFile_.open("bad.txt");
  }
};
