// 5cardsCombo.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>

#include "ComboVerifier.h"

int main()
{
  ComboVerifier<5> cv;
  const std::vector<char> validCards = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10
  };

  auto allHands = cv.ConstructAllHands({ validCards });
  std::cout << allHands.size() << std::endl;
  auto goodHands = cv.VerifyAllHands(24, allHands);
  std::cout << goodHands.size() << std::endl;

  return 0;
}

