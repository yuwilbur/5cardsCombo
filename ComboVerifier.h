#pragma once

#include <array>
#include <vector>
#include <string>
#include <functional>

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
    std::array<size_t, CARDS + 1> indexes = {}; // The extra index is used to check if the indexes have overflown
    std::array<int, CARDS> hand = {};
    while (indexes[indexes.size() - 1] == 0) {
      for (size_t i = 0; i < hand.size(); ++i) {
        hand[i] = validCards[indexes[i]];
      }
      hands.push_back(hand);
      indexes[0]++;
      for (size_t i = 0; i < indexes.size() - 1; ++i) {
        if (indexes[i] < validCards.size())
          break;
        indexes[i] = 0;
        indexes[i + 1]++;
      }
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

    const std::vector<std::string> primitiveOperations = { "*", "/", "+", "-" };
    std::array<size_t, CARDS> primitiveIndexes = {}; // The extra index is used to check if the indexes have overflown
    std::vector<std::string> operation = {};
    for (auto& card : handStr) {
      operation.push_back(card);
      operation.push_back("*");
    }
    operation.pop_back();
    if (ExpressionParser::EvaluateExpression(operation) == target)
      return true;
    return false;
  }
};