#include "DigitalLab.hpp"

#include <sstream>
#include <stdexcept>
#include <unordered_map>

namespace Digital_Lab {

// Mapping of pattern values to corresponding values in the matrix
static std::unordered_map<char, char> pattern_map = {{'0', '*'}, {'1', '2'}};

template <typename T>
T &get_value(T *array, size_t *shape, size_t x, size_t y) {
  if (x >= shape[1] || y >= shape[0]) {
    throw std::out_of_range("Index out of range");
  }
  return array[y * shape[1] + x];
}

bool is_match(char *pattern, size_t *pattern_shape, char *b, size_t *b_shape,
              size_t initial_x, size_t initial_y) {
  if (initial_x + pattern_shape[1] > b_shape[1] ||
      initial_y + pattern_shape[0] > b_shape[0]) {
    return false;
  }

  for (size_t local_y = 0; local_y < pattern_shape[0]; local_y++) {
    for (size_t local_x = 0; local_x < pattern_shape[1]; local_x++) {
      auto local_value = get_value(pattern, pattern_shape, local_x, local_y);
      auto b_value =
          get_value(b, b_shape, initial_x + local_x, initial_y + local_y);

      if (local_value != b_value) {
        return false;
      }
    }
  }

  return true;
}

void transform_by_pattern(char *pattern, size_t *pattern_shape, char *b,
                          bool *mask, size_t *b_shape, size_t initial_x,
                          size_t initial_y) {
  for (size_t local_x = 0; local_x < pattern_shape[1]; local_x++) {
    for (size_t local_y = 0; local_y < pattern_shape[0]; local_y++) {
      get_value(mask, b_shape, initial_x + local_x, initial_y + local_y) = true;

      char value = get_value(pattern, pattern_shape, local_x, local_y);

      auto value_to_set = pattern_map[value];
      if (!value_to_set) {
        throw std::invalid_argument("Unspecified value in pattern");
      }
      get_value(b, b_shape, initial_x + local_x, initial_y + local_y) =
          value_to_set;
    }
  }
}

void matrix_pattern_matching(char *pattern, size_t *pattern_shape, char *b,
                             size_t *b_shape, char *result) {
  size_t b_size = b_shape[1] * b_shape[0];
  bool *mask = new bool[b_size];

  for (size_t i = 0; i < b_size; i++) {
    mask[i] = false;
    result[i] = b[i];
  }

  for (size_t x = 0; x < b_shape[1]; x++) {
    for (size_t y = 0; y < b_shape[0]; y++) {
      if (!get_value(mask, b_shape, x, y) &&
          is_match(pattern, pattern_shape, b, b_shape, x, y)) {
        try {
          transform_by_pattern(pattern, pattern_shape, result, mask, b_shape, x,
                               y);
        } catch (...) {
          delete[] mask;
          throw;
        }
      }
    }
  }
  delete[] mask;
}

std::string handle_matrix_by_pattern(std::string input) {
  std::stringstream result;
  std::istringstream ss(input);

  std::size_t pattern_width, pattern_height;
  ss >> pattern_height >> pattern_width;
  char *pattern = new char[pattern_height * pattern_width];
  std::size_t pattern_shape[]{pattern_height, pattern_width};
  for (std::size_t i = 0; i < pattern_height * pattern_width; i++) {
    ss >> pattern[i];
  }
  std::size_t matrix_width, matrix_height;
  ss >> matrix_height >> matrix_width;
  char *matrix = new char[matrix_height * matrix_width];
  std::size_t matrix_shape[]{matrix_height, matrix_width};
  for (std::size_t i = 0; i < matrix_height * matrix_width; i++) {
    ss >> matrix[i];
  }
  char *result_matrix = new char[matrix_height * matrix_width];

  try {
    matrix_pattern_matching(pattern, pattern_shape, matrix, matrix_shape,
                            result_matrix);
    for (std::size_t i = 0; i < matrix_height * matrix_width; i++) {
      result << result_matrix[i] << " ";
      if (i % matrix_width == matrix_width - 1) {
        result << std::endl;
      }
    }
  } catch (const std::invalid_argument &e) {
    result << "Invalid argument: " << e.what() << std::endl;
  }

  delete[] pattern;
  delete[] matrix;
  delete[] result_matrix;
  return result.str();
}

}  // namespace Digital_Lab