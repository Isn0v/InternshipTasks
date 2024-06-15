#include <cstddef>
#include <iostream>
#include <DigitalLab/DigitalLab.hpp>

int main() {
  // size_t pattern_shape[]{2, 2};
  // char pattern[]{'1', '0', '1', '1'};

  size_t pattern_shape[]{1, 1};
  char pattern[]{'0'};

  size_t b_shape[]{5, 5};
  char b[]{'1', '1', '0', '0', '0', '0', '1', '1', '0', '0', '1', '0', '0',
           '1', '0', '1', '1', '1', '1', '0', '0', '0', '1', '1', '1'};

  int expected[]{'1', '2', '*', '0', '0', '0', '2', '2', '0',
                 '0', '2', '*', '0', '1', '0', '2', '2', '1',
                 '2', '*', '0', '0', '1', '2', '2'};

  char result[5 * 5];

  matrix_pattern_matching(pattern, pattern_shape, b, b_shape, result);
  for (size_t x = 0; x < b_shape[1]; x++) {
    for (size_t y = 0; y < b_shape[0]; y++) {
      std::cout << b[x * b_shape[0] + y] << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;

  for (size_t x = 0; x < b_shape[1]; x++) {
    for (size_t y = 0; y < b_shape[0]; y++) {
      std::cout << result[x * b_shape[0] + y] << " ";
    }
    std::cout << std::endl;
  }

  return 0;
}