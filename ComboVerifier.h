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
    brackets_ = {
      { 0, 1, 2, 3, 4, 5, 6},
      { -1, 0, 1, 2, -2, 3, 4, 5, 6 },
      { 0, 1, -1, 2, 3, 4, -2, 5, 6 },
      { 0, 1, 2, 3, -1, 4, 5, 6, -2 },
      { -1, 0, 1, 2, 3, 4, -2, 5, 6 },
      { 0, 1, -1, 2, 3, 4, 5, 6, -2 },
      { -1, 0, 1, 2, -2, -1, 3, 4, 5, 6, -2 },
      { -1, -1, 0, 1, 2, -2, 3, 4, -2, 5, 6 },
      { -1, 0, 1, -1, 2, 3, 4, -2, -2, 5, 6 },
      { 0, 1, -1, -1, 2, 3, 4, -2, 5, 6, -2 },
      { 0, 1, -1, 2, 3, -1, 4, 5, 6, -2, -2 },
    };

    std::vector<int> order(CARDS);
    for (size_t i = 0; i < CARDS; ++i) {
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

  std::vector<std::array<int, CARDS>> ConstructAllHands(const std::vector<int>& validCards) {
    if (validCards.size() == 0)
      throw std::invalid_argument("No valid cards detected");
    std::vector<std::array<int, CARDS>> hands = {};
    std::array<size_t, CARDS> indexes = {}; // The extra index is used to check if the indexes have overflown
    std::array<int, CARDS> hand = {};
    size_t indexesIndex = 0;
    while (indexes[0] < validCards.size()) {
      for (size_t i = 0; i < hand.size(); ++i) {
        hand[i] = validCards[indexes[i]];
      }
      hands.push_back(hand);

      if (indexesIndex == indexes.size()) {
        indexesIndex--;
        while (indexesIndex != 0 && indexes[indexesIndex] == indexes[indexesIndex - 1]) {
          indexes[indexesIndex] = 0;
          indexesIndex--;
        }
      }
      indexes[indexesIndex++]++;
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
};
