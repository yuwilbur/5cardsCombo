// 5cardsCombo.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include <iomanip>

#include "ComboVerifier.h"
#include "ExpressionParser.h"
#include "Buckets.h"
#include "Combinatorics.h"

int main()
{
  const std::vector<int> validCards = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
  };
  const size_t number_of_cards = 4;
  ComboVerifier<number_of_cards> cv(false);
  Buckets<number_of_cards> bucket;
  for (size_t i = 0; i < validCards.size(); ++i) {
    bucket.AddSpace(validCards[i]);
  }
  bucket.Initialize();

  auto uniqueHands = cv.ConstructUniqueHands(bucket);
  const size_t uniquePossibleCount = uniqueHands.size();
  const size_t possibleCount = cv.CalculateTotalHands(uniqueHands, bucket);
  std::cout << std::fixed << std::setprecision(2);
  std::cout << "TARGET" << "\t| ";
  std::cout << "TOTAL ( / " << possibleCount << " ) " << "\t| ";
  std::cout << "UNIQUE ( / " << uniquePossibleCount << " )";
  std::cout << std::endl;
  for (size_t i = 1; i < 30; ++i) {
    const int target = i;
    auto validHands = cv.ConstructValidUniqueHands(target, uniqueHands);
    const size_t uniqueValidCount = validHands.size();
    const size_t validCount = cv.CalculateTotalHands(validHands, bucket);
    const float validPercent = (float)validCount / (float)possibleCount * 100;
    const float uniqueValidPercent = (float)uniqueValidCount / (float)uniquePossibleCount * 100;

    std::cout << target << "\t| ";
    std::cout << validCount << " (" << validPercent << "%)" << "\t| ";
    std::cout << uniqueValidCount << " (" << uniqueValidPercent << "%)";
    std::cout << std::endl;
  }
  return 0;
}
