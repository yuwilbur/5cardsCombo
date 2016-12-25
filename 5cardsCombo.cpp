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
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
  };

  auto allHands = cv.ConstructAllHands({ validCards });
  //auto goodHands = cv.VerifyAllHands(2, allHands);
  //std::cout << std::endl;
  //std::cout << allHands.size() << " " << goodHands.size() << std::endl;
  std::cout << allHands.size() << std::endl;
  return 0;
}
