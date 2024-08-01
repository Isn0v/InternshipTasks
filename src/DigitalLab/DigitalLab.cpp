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
 * @brief Checks if a given pattern matches a submatrix of another matrix.
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
  // Check if the pattern fits within the matrix
  if (initial_x + pattern_shape[1] > b_shape[1] ||
      initial_y + pattern_shape[0] > b_shape[0]) {
    return false;
  }

  // Iterate over the pattern and check if the values match
  for (size_t local_y = 0; local_y < pattern_shape[0]; local_y++) {
    for (size_t local_x = 0; local_x < pattern_shape[1]; local_x++) {
      // Get the value from the pattern and the corresponding value from the
      // matrix
      auto local_value = get_value(pattern, pattern_shape, local_x, local_y);
      auto b_value =
          get_value(b, b_shape, initial_x + local_x, initial_y + local_y);

      // If the values do not match, the pattern does not match the submatrix
      if (local_value != b_value) {
        return false;
      }
    }
  }

  // If all values in the pattern match the submatrix, the pattern matches
  return true;
}

/**
 * @brief Applies a pattern to a matrix and stores the result in another matrix.
 *
 * This function applies a pattern to a matrix and stores the result in another
 * matrix. The pattern is applied starting from the specified initial
 * coordinates.
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
  // Iterate over each element in the pattern
  for (size_t local_x = 0; local_x < pattern_shape[1]; local_x++) {
    for (size_t local_y = 0; local_y < pattern_shape[0]; local_y++) {
      // Set the corresponding element in the mask matrix to true
      get_value(mask, b_shape, initial_x + local_x, initial_y + local_y) = true;

      // Get the value from the pattern
      char value = get_value(pattern, pattern_shape, local_x, local_y);

      // Get the corresponding value to set in the matrix 'b'
      auto value_to_set = pattern_map[value];

      // If the value is not specified in the pattern map, throw an exception
      if (!value_to_set) {
        throw std::invalid_argument("Unspecified value in pattern");
      }

      // Set the value in the matrix 'b'
      get_value(b, b_shape, initial_x + local_x, initial_y + local_y) =
          value_to_set;
    }
  }
}

/**
 * Function that applies a pattern to a matrix based on a given mask.
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
void matrix_pattern_matching(char *pattern, size_t *pattern_shape, char *b,
                             size_t *b_shape, char *result) {
  // Get the total size of the matrix
  size_t b_size = b_shape[1] * b_shape[0];

  // Allocate memory for the mask matrix
  bool *mask = new bool[b_size];

  // Initialize the mask matrix and the result matrix with the values from the
  // input matrix
  for (size_t i = 0; i < b_size; i++) {
    // Set the corresponding element in the mask matrix to false
    mask[i] = false;
    // Set the corresponding element in the result matrix with the value from
    // the input matrix
    result[i] = b[i];
  }

  // Iterate over each element in the matrix
  for (size_t x = 0; x < b_shape[1]; x++) {
    for (size_t y = 0; y < b_shape[0]; y++) {
      // If the element is not marked in the mask matrix and a match is found
      // in the input matrix, apply the pattern to the corresponding element in
      // the matrix
      if (!get_value(mask, b_shape, x, y) &&
          is_match(pattern, pattern_shape, b, b_shape, x, y)) {
        try {
          transform_by_pattern(pattern, pattern_shape, result, mask, b_shape, x,
                               y);
        } catch (...) {
          // Delete the mask matrix and rethrow the exception
          delete[] mask;
          throw;
        }
      }
    }
  }

  // Delete the mask matrix
  delete[] mask;
}

/**
 * @brief Handles matrix operations based on a given pattern.
 * Function that handles matrix operations based on a given pattern.
 *
 * This function reads the input stream, extracts the pattern and matrix
 * information, performs the matrix pattern matching, and returns the result as
 * a string.
 * @param input The input string containing pattern and matrix data.
 *
 * @param input The input stream containing pattern and matrix data.
 * @return A string representing the result of the matrix operations.
 * @throws std::invalid_argument If an invalid argument is encountered.
 */
std::string handle_digital_lab(std::istream &input) {
  std::stringstream result;  // Result string stream

  // Read the pattern dimensions
  std::size_t pattern_width, pattern_height;  // Pattern dimensions
  input >> pattern_height >> pattern_width;

  // Allocate memory for the pattern matrix
  char *pattern = new char[pattern_height * pattern_width];
  std::size_t pattern_shape[]{pattern_height, pattern_width};  // Pattern shape

  // Read the pattern matrix
  for (std::size_t i = 0; i < pattern_height * pattern_width; i++) {
    input >> pattern[i];
  }

  // Read the matrix dimensions
  std::size_t matrix_width, matrix_height;  // Matrix dimensions
  input >> matrix_height >> matrix_width;

  // Allocate memory for the matrix and the result matrix
  char *matrix = new char[matrix_height * matrix_width];
  std::size_t matrix_shape[]{matrix_height, matrix_width};  // Matrix shape

  // Read the matrix
  for (std::size_t i = 0; i < matrix_height * matrix_width; i++) {
    input >> matrix[i];
  }
  char *result_matrix = new char[matrix_height * matrix_width];

  try {
    // Perform the matrix pattern matching
    matrix_pattern_matching(pattern, pattern_shape, matrix, matrix_shape,
                            result_matrix);

    // Write the result matrix to the result string stream
    for (std::size_t i = 0; i < matrix_height * matrix_width; i++) {
      result << result_matrix[i] << " ";
      if (i % matrix_width == matrix_width - 1) {
        result << std::endl;
      }
    }
  } catch (const std::invalid_argument &e) {
    // Catch and handle invalid argument exceptions
    result << "Invalid argument: " << e.what() << std::endl;
  }

  // Delete the allocated memory
  delete[] pattern;
  delete[] matrix;
  delete[] result_matrix;

  return result.str();  // Return the result string
}

}  // namespace Digital_Lab