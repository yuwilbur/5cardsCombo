// 5cardsCombo.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>

#include "ComboVerifier.h"
#include "ExpressionParser.h"

int main()
{
  /*ComboVerifier<4> cv;
  const std::vector<int> validCards = {
    1, 2, 3, 4, 5, 6
  };

  auto allHands = cv.ConstructAllHands({ validCards });
  auto goodHands = cv.VerifyAllHands(24, allHands);
  for (auto& hand : goodHands) {
    for (auto& card : hand) {
      std::cout << std::to_string(card) << " ";
    }
    std::cout << std::endl;
  }
*/

  std::vector<std::string> expression = {
    "(", "3", "+", "5", ")", "-" , "10", "/" , "2"
  }; 
  //std::vector<std::string> expression = {
  //  "3", "+", "5", "*" , "4", "/" , "2"
  //};
  std::cout << ExpressionParser::EvaluateExpression(expression) << std::endl;

  return 0;
}
