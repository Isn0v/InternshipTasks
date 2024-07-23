#pragma once

#include <cstddef>
#include <string>

namespace Digital_Lab {


void matrix_pattern_matching(char *pattern, size_t *pattern_shape, char *b,
                             size_t *b_shape, char *result);


std::string handle_digital_lab(std::istream &input);                          

}  // namespace Digital_Lab