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

/**
 * Checks if a given pattern matches a submatrix of another matrix.
 *
 * @param pattern Pointer to the pattern to be matched.
 * @param pattern_shape Pointer to the shape of the pattern.
 * @param b Pointer to the matrix where the pattern will be matched.
 * @param b_shape Pointer to the shape of the matrix where the pattern will be
 * matched.
 * @param initial_x The initial x-coordinate where the pattern will be matched.
 * @param initial_y The initial y-coordinate where the pattern will be matched.
 *
 * @return True if the pattern matches the submatrix, false otherwise.
 *
 * @throws None
 */
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

/**
 * Applies a pattern to a matrix and stores the result in another matrix.
 *
 * @param pattern Pointer to the pattern to be applied.
 * @param pattern_shape Pointer to the shape of the pattern.
 * @param b Pointer to the matrix where the pattern will be applied.
 * @param mask Pointer to the mask matrix.
 * @param b_shape Pointer to the shape of the matrix where the pattern will be
 * applied.
 * @param initial_x The initial x-coordinate where the pattern will be applied.
 * @param initial_y The initial y-coordinate where the pattern will be applied.
 *
 * @throws std::invalid_argument If an unspecified value is found in the
 * pattern.
 */
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

/**
 * @brief Applies the pattern to the matrix b and stores the result in the
 * result matrix.
 *
 * @param pattern Pointer to the pattern
 * @param pattern_shape Pointer to the pattern shape
 * @param b Pointer to the matrix
 * @param result Pointer to the result matrix
 * @param b_shape Pointer to the matrix shape
 */
void matrix_pattern_matching(char *pattern, size_t *pattern_shape, char *b,
                             size_t *b_shape, char *result) {
  // TODO: improve logic using DSU if has time
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

/**
 * Function that handles matrix operations based on a given pattern.
 *
 * @param input The input string containing pattern and matrix data.
 *
 * @return A string representing the result of the matrix operations.
 */
std::string handle_digital_lab(std::istream &input) {
  std::stringstream result;

  std::size_t pattern_width, pattern_height;
  input >> pattern_height >> pattern_width;
  char *pattern = new char[pattern_height * pattern_width];
  std::size_t pattern_shape[]{pattern_height, pattern_width};
  for (std::size_t i = 0; i < pattern_height * pattern_width; i++) {
    input >> pattern[i];
  }

  // NOTE: in task there wasn't specified the height and width of the matrix
  // so it is assumed that it doesn't contain more than 10^3 and 10^3
  // symbols in each side
  std::size_t matrix_width, matrix_height;
  input >> matrix_height >> matrix_width;
  char *matrix = new char[matrix_height * matrix_width];
  std::size_t matrix_shape[]{matrix_height, matrix_width};
  for (std::size_t i = 0; i < matrix_height * matrix_width; i++) {
    input >> matrix[i];
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