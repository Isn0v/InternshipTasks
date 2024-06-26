#pragma once

#include <cstddef>
#include <string>

namespace Digital_Lab {

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
                             size_t *b_shape, char *result);


/**
 * Function that handles matrix operations based on a given pattern.
 *
 * @param input The input string containing pattern and matrix data.
 *
 * @return A string representing the result of the matrix operations.
 */
std::string handle_matrix_by_pattern(std::string input);                          

}  // namespace Digital_Lab