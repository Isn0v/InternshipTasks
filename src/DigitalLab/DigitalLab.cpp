#include "DigitalLab.hpp"
#include <stdexcept>
#include <unordered_map>

// Mapping of pattern values to corresponding values in the matrix
static std::unordered_map<char, char> pattern_map = {{'0', '*'}, {'1', '2'}};

/**
 * @brief Returns a reference to the value at the specified coordinates in the
 * array.
 *
 * @tparam T Type of the array elements
 * @param array Pointer to the array
 * @param shape Pointer to the array shape
 * @param x X-coordinate
 * @param y Y-coordinate
 * @return Reference to the value at the specified coordinates
 * @throws std::out_of_range If the coordinates are out of range
 */
template <typename T>
T &get_value(T *array, size_t *shape, size_t x, size_t y) {
  if (x >= shape[1] || y >= shape[0]) {
    throw std::out_of_range("Index out of range");
  }
  return array[y * shape[1] + x];
}

/**
 * @brief Checks if the specified pattern matches the submatrix in the matrix
 * b starting at the specified coordinates.
 *
 * @param pattern Pointer to the pattern
 * @param pattern_shape Pointer to the pattern shape
 * @param b Pointer to the matrix
 * @param b_shape Pointer to the matrix shape
 * @param initial_x X-coordinate of the starting point
 * @param initial_y Y-coordinate of the starting point
 * @return True if the pattern matches the submatrix, false otherwise
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
 * @brief Transforms the submatrix in the matrix b starting at the specified
 * coordinates by replacing values according to the pattern.
 *
 * @param pattern Pointer to the pattern
 * @param pattern_shape Pointer to the pattern shape
 * @param b Pointer to the matrix
 * @param mask Pointer to the mask indicating which elements have been
 * transformed
 * @param b_shape Pointer to the matrix shape
 * @param initial_x X-coordinate of the starting point
 * @param initial_y Y-coordinate of the starting point
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
      get_value(b, b_shape, initial_x + local_x, initial_y + local_y) = value_to_set;
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
        transform_by_pattern(pattern, pattern_shape, result, mask, b_shape, x,
                             y);
      }
    }
  }

  delete[] mask;
}

