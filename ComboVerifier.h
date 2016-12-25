#pragma once

#include <array>
#include <vector>
#include <functional>

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

  std::vector<std::array<char, CARDS>> VerifyAllHands(const int target, const std::vector<std::array<char, CARDS>>& hands) {
    std::vector<std::array<char, CARDS>> validHands = {};
    for (auto& hand : hands) {
      if (VerifyHand(target, hand, ConstructAllOperations()))
        validHands.push_back(hand);
    }
    return validHands;
  }

  bool VerifyHand(const int target, const std::array<char, CARDS>& hand, const std::vector<std::function<int(const std::array<char, CARDS>&)>>& operations) {
    for (auto& operation : operations) {
      if (operation(hand) == target)
        return true;
    }
    return false;
  }

  std::vector<std::function<int(const std::array<char, CARDS>&)>> ConstructAllOperations() {
    auto operation = [](const std::array<char, CARDS>& cards) -> int {
      int result = 0;
      for (auto& card : cards) {
        result += card;
      }
      return result;
    };
    return{operation};
  }
};