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
  ComboVerifier() = default;

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
        while (indexes[indexesIndex] == indexes[indexesIndex - 1]) {
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
    std::ofstream goodFile;
    goodFile.open("good.txt");
    
    std::ofstream badFile;
    badFile.open("bad.txt");
    for (auto& hand : hands) {
      std::string handStr = "";
      for (auto& card : hand) {
        handStr += std::to_string(card) + " ";
      }
      if (VerifyHand(target, hand)) {
        validHands.push_back(hand);
        std::cout << ".";
        goodFile << handStr << std::endl;
      }
      else {
        std::cout << "X";
        badFile << handStr << std::endl;
      }
    }
    goodFile.close();
    badFile.close();
    std::cout << std::endl;
    return validHands;
  }

  bool VerifyHand(const int target, const std::array<int, CARDS>& hand) {
    std::array<std::string, CARDS> handStr = {};
    for (size_t i = 0; i < hand.size(); ++i) {
      handStr[i] = std::to_string(hand[i]);
    }

    const std::vector<std::vector<int>> brackets =
    {
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
    const std::vector<std::string> primitiveOperations = { "+", "-", "*", "/" };
    std::array<size_t, CARDS> primitiveIndexes = {}; // The extra index is used to check if the indexes have overflown
    while (primitiveIndexes[primitiveIndexes.size() - 1] == 0) {
      std::vector<std::string> operation = {};
      for (size_t i = 0; i < handStr.size(); ++i) {
        operation.push_back(handStr[i]);
        if (i < handStr.size() - 1) {
          operation.push_back(primitiveOperations[primitiveIndexes[i]]);
        }
      }
      if (ExpressionParser::EvaluateExpression(operation) == target)
        return true;

      for (auto& bracket : brackets) {
        std::vector<std::string> bracketOperation = {};
        for (auto& item : bracket) {
          if (item == -1)
            bracketOperation.push_back("(");
          else if (item == -2)
            bracketOperation.push_back(")");
          else
            bracketOperation.push_back(operation[item]);
        }
        if (ExpressionParser::EvaluateExpression(bracketOperation) == target) {
          //for (auto& token : bracketOperation) {
          //  std::cout << token << " ";
          //}
          //std::cout << std::endl;
          return true;
        }
      }

      primitiveIndexes[0]++;
      for (size_t i = 0; i < primitiveIndexes.size() - 1; ++i) {
        if (primitiveIndexes[i] < primitiveOperations.size())
          break;
        primitiveIndexes[i] = 0;
        primitiveIndexes[i + 1]++;
      }
    }
    return false;
  }
};
