#include <gtest/gtest.h>

#include <DigitalLab/DigitalLab.hpp>
#include <fstream>
#include <sstream>
#include <string>

// we throw an error if we have a pattern with unspecified values

// Expected result:
//                std::invalid_argument: Unspecified value in pattern (catched
//                in handler)

TEST(DigitalLab, IntegrationTestWithIncorrectPattern) {
  auto inp =
      "2 3\n"
      "1 0 2\n"
      "1 2 3\n"
      "5 5\n"
      "1 0 2 0 0\n"
      "1 2 3 0 0\n"
      "1 2 3 1 0\n"
      "1 1 1 0 2\n"
      "0 0 1 2 3\n";

  std::istringstream in(inp);

  std::string expected = "Invalid argument: Unspecified value in pattern\n";

  EXPECT_EQ(Digital_Lab::handle_digital_lab(in), expected);
}


class DigitalLabTest : public ::testing::TestWithParam<int> {};
INSTANTIATE_TEST_SUITE_P(DigitalLab, DigitalLabTest, ::testing::Range(1, 9));

// TODO add more tests if needed
TEST_P(DigitalLabTest, IntegrationTest) {
  int num_test = GetParam();
  std::stringstream ss_in, ss_exp;

  ss_in << CMAKE_PROJECT_SOURCE_DIR << "/test/data/DigitalLab/input_"
        << num_test << ".txt";
  ss_exp << CMAKE_PROJECT_SOURCE_DIR << "/test/data/DigitalLab/expected_"
         << num_test << ".txt";

  std::ifstream in(ss_in.str());
  std::ifstream exp(ss_exp.str());
  std::ostringstream expected;
  expected << exp.rdbuf();

  if (!exp.is_open() || !in.is_open()) {
    FAIL() << "Failed to open expected output file";
  }

  EXPECT_EQ(Digital_Lab::handle_digital_lab(in), expected.str());
}
