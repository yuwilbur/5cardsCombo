// 5cardsCombo.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>

#include "ComboVerifier.h"
#include "ExpressionParser.h"

int main()
{
  ComboVerifier<4> cv;
  const std::vector<int> validCards = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
  };
  const int target = 2;

  auto allHands = cv.ConstructAllHands({ validCards });
  std::cout << "UNIQUE HANDS COUNT: " << allHands.size() << std::endl;
  auto goodHands = cv.VerifyAllHands(target, allHands);
  std::cout << std::endl;
  std::cout << "VALID HANDS COUNT: " << goodHands.size() << " FOR " << target << std::endl;
  return 0;
}
