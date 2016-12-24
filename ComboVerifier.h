#pragma once

#include <array>
#include <vector>

template<int CARDS>
class ComboVerifier
{
public:
  ComboVerifier() = default;

  std::vector<std::array<char, CARDS>> ConstructAllHands(const std::vector<char>& validCards) {
    if (validCards.size() == 0)
      throw std::invalid_argument("No valid cards detected");
    std::vector<std::array<char, CARDS>> hands = {};
    std::array<size_t, CARDS + 1> indexes = {}; // The extra index is used to check if the indexes have overflown
    std::array<char, CARDS> hand = {};
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

  std::vector<std::array<char, CARDS>> VerifyAllHands(const size_t target, const std::vector<std::array<char, CARDS>>& hands) {
    std::vector<std::array<char, CARDS>> validHands = {};
    for (auto& hand : hands) {
      if (VerifyHand(target, hand))
        validHands.push_back(hand);
    }
    return validHands;
  }

  bool VerifyHand(const size_t target, const std::array<char, CARDS>& hand) {
    size_t result = 0;
    for (size_t i = 0; i < hand.size(); ++i) {
      result += hand[i];
    }
    return result == target;
  }
};