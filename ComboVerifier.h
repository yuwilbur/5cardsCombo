#pragma once

#include <array>
#include <vector>
#include <string>
#include <map>

#include <fstream>
#include <iostream>

#include "Buckets.h"
#include "ExpressionParser.h"
#include "Combinatorics.h"

template<int CARDS>
class ComboVerifier
{
public:
  ComboVerifier(const bool debug = true) :
    debug_(debug)
  {
    Initialize();
  }

  std::vector<std::array<int, CARDS>> ConstructUniqueHands(Buckets<CARDS>& bucket) {
    std::vector<std::array<int, CARDS>> hands = {};
    hands.push_back(bucket.GetItems());
    while (bucket.Increment())
      hands.push_back(bucket.GetItems());
    return hands;
  }

  std::vector<std::array<int, CARDS>> ConstructValidUniqueHands(const int target, const std::vector<std::array<int, CARDS>>& hands) {
    std::vector<std::array<int, CARDS>> validHands = {};
    for (auto& hand : hands) {
      if (VerifyHand(target, hand))
        validHands.push_back(hand);
    }
    return validHands;
  }

  size_t CalculateTotalHands(const std::vector<std::array<int, CARDS>>& hands, const Buckets<CARDS>& bucket) {
    size_t count = 0;
    const auto histogram = bucket.GetHistogram();
    for (auto& hand : hands) {
      std::map<int, size_t> cardCount = {};
      for (auto& card : hand) {
        cardCount[card]++;
      }
      size_t handCount = 1;
      for (auto& card : cardCount) {
        handCount *= Combinatorics::Combination(histogram.at(card.first), card.second);
      }
      count += handCount;
    }
    return count;
  }

protected:
  bool VerifyHand(const int target, const std::array<int, CARDS>& hand) {
    std::array<std::string, CARDS> handStr = {};
    for (size_t i = 0; i < hand.size(); ++i) {
      handStr[i] = std::to_string(hand[i]);
    }

    for (auto operation : operations_) {
      for (auto& token : operation) {
        if (token[0] == 'N')
          token = handStr[std::stoi(token.substr(1))];
      }
      if (ExpressionParser::EvaluateExpression(operation) == target) {
        if (debug_) {
          std::string result = "";
          for (auto& token : operation)
            result += token;
          goodFile_ << result << std::endl;
          std::cout << ".";
        }
        return true;
      }
    }
    if (debug_) {
      std::string result = "";
      for (auto& token : hand)
        result += std::to_string(token) + " ";
      badFile_ << result << std::endl;
      std::cout << "X";
    }
    return false;
  }

private:
  const bool debug_;
  std::ofstream badFile_;
  std::ofstream goodFile_;
  std::vector<std::vector<int>> order_;
  std::vector<std::vector<int>> brackets_;
  std::vector<std::vector<int>> primitives_;
  std::vector<std::vector<std::string>> operations_;
  std::array<std::string, 4> primitiveOperations_;

  void Initialize() {
    primitiveOperations_ = { "+", "-", "*", "/" };
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
                operation.push_back("N" + std::to_string(order[index / 2]));
              else
                operation.push_back(primitiveOperations_[primitives[index / 2]]);
            }
          }
          operations_.push_back(operation);
        }
      }
    }

    if (debug_) {
      goodFile_.open("good.txt");
      badFile_.open("bad.txt");
    }
  }
};
