#pragma once
#include <stdexcept>

namespace Combinatorics {
  size_t Factorial(const size_t n) {
    return (n == 0 || n == 1) ? 1 : Factorial(n - 1) * n;
  }

  size_t Combination(size_t choices, const size_t choose) {
    if (choices < choose)
      throw std::invalid_argument("Choices need to be larger than choose");
    size_t result = 1;
    for (size_t i = 1; i <= choose; ++i) {
      result *= choices--;
      result /= i;
    }
    return result;
  }
}
