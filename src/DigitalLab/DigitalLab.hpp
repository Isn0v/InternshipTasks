#pragma once
#include <cstddef>

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