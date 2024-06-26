#include <gtest/gtest.h>

#include <DigitalLab/DigitalLab.hpp>
#include <sstream>
#include <string>

template <typename T>
bool equals(T *a, size_t *a_shape, T *b, size_t *b_shape) {
  if (a_shape[0] != b_shape[0] || a_shape[1] != b_shape[1]) {
    return false;
  }

  for (size_t i = 0; i < b_shape[0] * b_shape[1]; i++) {
    if (a[i] != b[i]) {
      return false;
    }
  }

  return true;
}

std::string matrix_to_string(char *matrix, size_t *matrix_shape) {
  std::stringstream ss;

  for (size_t i = 0; i < matrix_shape[0]; i++) {
    for (size_t j = 0; j < matrix_shape[1]; j++) {
      ss << matrix[i * matrix_shape[1] + j];
    }

    ss << std::endl;
  }

  return ss.str();
}

// Pattern :  1 0
//            1 1

// Matrix b:      1 1 0 0 0
//                0 1 1 0 0
//                1 0 0 1 0
//                1 1 1 1 0
//                0 0 1 1 1

// Expected result:
//                1 2 * 0 0
//                0 2 2 0 0
//                2 * 0 1 0
//                2 2 1 2 *
//                0 0 1 2 2
TEST(DigitalLab, PatternShape2x2Test) {
  size_t pattern_shape[]{2, 2};
  char pattern[]{'1', '0', '1', '1'};

  size_t b_shape[]{5, 5};
  char b[]{'1', '1', '0', '0', '0', '0', '1', '1', '0', '0', '1', '0', '0',
           '1', '0', '1', '1', '1', '1', '0', '0', '0', '1', '1', '1'};

  char expected[]{'1', '2', '*', '0', '0', '0', '2', '2', '0',
                  '0', '2', '*', '0', '1', '0', '2', '2', '1',
                  '2', '*', '0', '0', '1', '2', '2'};

  char result[5 * 5];

  Digital_Lab::matrix_pattern_matching(pattern, pattern_shape, b, b_shape,
                                       result);

  EXPECT_TRUE(equals(result, b_shape, expected, b_shape));
}

// Pattern :  1

// Matrix b:      1 1 0 0 0
//                0 1 1 0 0
//                1 0 0 1 0
//                1 1 1 1 0
//                0 0 1 1 1

// Expected result:
//                2 2 0 0 0
//                0 2 2 0 0
//                2 0 0 2 0
//                2 2 2 2 0
//                0 0 2 2 2
TEST(DigitalLab, PatternShape1x1With0Test) {
  size_t pattern_shape[]{1, 1};
  char pattern[]{'1'};

  size_t b_shape[]{5, 5};
  char b[]{'1', '1', '0', '0', '0', '0', '1', '1', '0', '0', '1', '0', '0',
           '1', '0', '1', '1', '1', '1', '0', '0', '0', '1', '1', '1'};

  char expected[]{'2', '2', '0', '0', '0', '0', '2', '2', '0',
                  '0', '2', '0', '0', '2', '0', '2', '2', '2',
                  '2', '0', '0', '0', '2', '2', '2'};

  char result[5 * 5];
  Digital_Lab::matrix_pattern_matching(pattern, pattern_shape, b, b_shape,
                                       result);

  EXPECT_TRUE(equals(result, b_shape, expected, b_shape));
}

// Pattern :  0

// Matrix b:      1 1 0 0 0
//                0 1 1 0 0
//                1 0 0 1 0
//                1 1 1 1 0
//                0 0 1 1 1

// Expected result:
//                1 1 * * *
//                * 1 1 * *
//                1 * * 1 *
//                1 1 1 1 *
//                * * 1 1 1
TEST(DigitalLab, PatternShape1x1With1Test) {
  size_t pattern_shape[]{1, 1};
  char pattern[]{'0'};

  size_t b_shape[]{5, 5};
  char b[]{'1', '1', '0', '0', '0', '0', '1', '1', '0', '0', '1', '0', '0',
           '1', '0', '1', '1', '1', '1', '0', '0', '0', '1', '1', '1'};

  char expected[]{'1', '1', '*', '*', '*', '*', '1', '1', '*',
                  '*', '1', '*', '*', '1', '*', '1', '1', '1',
                  '1', '*', '*', '*', '1', '1', '1'};

  char result[5 * 5];
  Digital_Lab::matrix_pattern_matching(pattern, pattern_shape, b, b_shape,
                                       result);

  EXPECT_TRUE(equals(result, b_shape, expected, b_shape));
}

// pattern:  1 1
//           1 1

// b:        1 1 1 0 0
//           1 1 1 0 0
//           1 0 1 1 1
//           1 1 1 1 1
//           0 0 1 1 1

// expected: 2 2 1 0 0
//           2 2 1 0 0
//           1 0 2 2 1
//           1 1 2 2 1
//           0 0 1 1 1
TEST(DigitalLab, PatternShape2x2WithAll1Test) {
  size_t pattern_shape[]{2, 2};
  char pattern[]{'1', '1', '1', '1'};

  size_t b_shape[]{5, 5};
  char b[]{'1', '1', '1', '0', '0', '1', '1', '1', '0', '0', '1', '0', '1',
           '1', '1', '1', '1', '1', '1', '1', '0', '0', '1', '1', '1'};

  char expected[]{'2', '2', '1', '0', '0', '2', '2', '1', '0',
                  '0', '1', '0', '2', '2', '1', '1', '1', '2',
                  '2', '1', '0', '0', '1', '1', '1'};

  char result[5 * 5];
  Digital_Lab::matrix_pattern_matching(pattern, pattern_shape, b, b_shape,
                                       result);
  EXPECT_TRUE(equals(result, b_shape, expected, b_shape));
}

// pattern: 1 0 0 1 0 1
//          1 1 1 0 1 0

// Matrix b:      1 1 0 0 0
//                0 1 1 0 0
//                1 0 0 1 0
//                1 1 1 1 0
//                0 0 1 1 1

// Expected result:
//                1 1 0 0 0
//                0 1 1 0 0
//                1 0 0 1 0
//                1 1 1 1 0
//                0 0 1 1 1
TEST(DigitalLab, IncorrectPatternShape2x6Test) {
  size_t pattern_shape[]{2, 6};
  char pattern[]{'1', '0', '0', '1', '0', '1', '1', '1', '0', '1', '0', '1'};

  size_t b_shape[]{5, 5};
  char b[]{'1', '1', '0', '0', '0', '0', '1', '1', '0', '0', '1', '0', '0',
           '1', '0', '1', '1', '1', '1', '0', '0', '0', '1', '1', '1'};

  char *expected = b;
  char result[5 * 5];
  Digital_Lab::matrix_pattern_matching(pattern, pattern_shape, b, b_shape,
                                       result);
  EXPECT_TRUE(equals(result, b_shape, expected, b_shape));
}

// pattern: 1 0 0 1
//          1 1 1 0

// Matrix b:      1 1 0 0 0
//                0 1 1 0 0
//                1 0 0 1 0
//                1 1 1 0 0
//                1 1 1 0 1

// Expected result:
//                1 1 0 0 0
//                0 1 1 0 0
//                2 * * 2 0
//                2 2 2 * 0
//                1 1 1 0 1
TEST(DigitalLab, PatternShape2x4Test) {
  size_t pattern_shape[]{2, 4};
  char pattern[]{'1', '0', '0', '1', '1', '1', '1', '0'};

  size_t b_shape[]{5, 5};
  char b[]{'1', '1', '0', '0', '0', '0', '1', '1', '0', '0', '1', '0', '0',
           '1', '0', '1', '1', '1', '0', '0', '1', '1', '1', '0', '1'};

  char expected[]{'1', '1', '0', '0', '0', '0', '1', '1', '0',
                  '0', '2', '*', '*', '2', '0', '2', '2', '2',
                  '*', '0', '1', '1', '1', '0', '1'};

  char result[5 * 5];
  Digital_Lab::matrix_pattern_matching(pattern, pattern_shape, b, b_shape,
                                       result);
  EXPECT_TRUE(equals(result, b_shape, expected, b_shape));
}

// we throw an error if we have a pattern with unspecified values

// pattern: 1 0 2
//          1 2 3

// Matrix b:      1 0 2 0 0
//                1 2 3 0 0
//                1 2 3 1 0
//                1 1 1 0 2
//                0 0 1 2 3

// Expected result:
//                std::invalid_argument: Unspecified value in pattern
TEST(DigitalLab, PatternShape2x3WithUnspecifiedValuesTest) {
  size_t pattern_shape[]{2, 3};
  char pattern[]{'1', '0', '2', '1', '2', '3'};

  size_t b_shape[]{5, 5};
  char b[]{'1', '0', '2', '0', '0', '1', '2', '3', '0', '0', '1', '2', '3',
           '1', '0', '1', '1', '1', '0', '2', '0', '0', '1', '2', '3'};

  char result[5 * 5];
  EXPECT_THROW(Digital_Lab::matrix_pattern_matching(pattern, pattern_shape, b,
                                                    b_shape, result),
               std::invalid_argument);
}

TEST(DigitalLab, IntegrationTestFromTestCase1) {
  std::string inp =
      "2 2\n"
      "1 0\n"
      "1 1\n"
      "5 5\n"
      "1 1 0 0 0\n"
      "0 1 1 0 0\n"
      "1 0 0 1 0\n"
      "1 1 1 1 0\n"
      "0 0 1 1 1\n";

  std::string expected =
      "1 2 * 0 0 \n"
      "0 2 2 0 0 \n"
      "2 * 0 1 0 \n"
      "2 2 1 2 * \n"
      "0 0 1 2 2 \n";

  EXPECT_EQ(Digital_Lab::handle_matrix_by_pattern(inp), expected);
}

TEST(DigitalLab, IntegrationTestFromTestCase2) {
  std::string inp =
      "1 1\n"
      "1\n"
      "5 5\n"
      "1 1 0 0 0\n"
      "0 1 1 0 0\n"
      "1 0 0 1 0\n"
      "1 1 1 1 0\n"
      "0 0 1 1 1\n";

  std::string expected =
      "2 2 0 0 0 \n"
      "0 2 2 0 0 \n"
      "2 0 0 2 0 \n"
      "2 2 2 2 0 \n"
      "0 0 2 2 2 \n";

  EXPECT_EQ(Digital_Lab::handle_matrix_by_pattern(inp), expected);
}

TEST(DigitalLab, IntegrationTestFromTestCase3) {
  std::string inp =
      "1 1\n"
      "0\n"
      "5 5\n"
      "1 1 0 0 0\n"
      "0 1 1 0 0\n"
      "1 0 0 1 0\n"
      "1 1 1 1 0\n"
      "0 0 1 1 1\n";

  std::string expected =
      "1 1 * * * \n"
      "* 1 1 * * \n"
      "1 * * 1 * \n"
      "1 1 1 1 * \n"
      "* * 1 1 1 \n";

  EXPECT_EQ(Digital_Lab::handle_matrix_by_pattern(inp), expected);
}

TEST(DigitalLab, IntegrationTestFromTestCase4) {
  std::string inp =
      "2 6\n"
      "1 0 0 1 0 1\n"
      "1 1 1 0 1 0\n"
      "5 5\n"
      "1 1 0 0 0\n"
      "0 1 1 0 0\n"
      "1 0 0 1 0\n"
      "1 1 1 1 0\n"
      "0 0 1 1 1\n";

  std::string expected =
      "1 1 0 0 0 \n"
      "0 1 1 0 0 \n"
      "1 0 0 1 0 \n"
      "1 1 1 1 0 \n"
      "0 0 1 1 1 \n";

  EXPECT_EQ(Digital_Lab::handle_matrix_by_pattern(inp), expected);
}

TEST(DigitalLab, IntegrationTestWithIncorrectPattern) {
  std::string inp =
      "2 3\n"
      "1 0 2\n"
      "1 2 3\n"
      "5 5\n"
      "1 0 2 0 0\n"
      "1 2 3 0 0\n"
      "1 2 3 1 0\n"
      "1 1 1 0 2\n"
      "0 0 1 2 3\n";

  std::string expected = "Invalid argument: Unspecified value in pattern\n";

  EXPECT_EQ(Digital_Lab::handle_matrix_by_pattern(inp), expected);
}