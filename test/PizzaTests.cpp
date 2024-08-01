
#include <gtest/gtest.h>

#include <Pizza/Pizza.hpp>
#include <fstream>
#include <sstream>

// NOTE: Results differs from what is presented in the task description.
//       Because of the implementation details of the algorithm, we have various
//       ways of covering the city.

// P.S: IntegrationTest/7 (Param = 8, 8'th input and expected files) is supposed
// not to be passed because this is the test case fo which I couldn't correclty
// expand my solution. Spent a lot of time for that and decided to skip it. I am
// still working on it and if I success with it I will add it and send.

class PizzaTest : public ::testing::TestWithParam<int> {};
INSTANTIATE_TEST_SUITE_P(Pizza, PizzaTest, ::testing::Range(1, 10));

TEST_P(PizzaTest, IntegrationTest) {
  int num_test = GetParam();
  std::stringstream ss_in, ss_exp;

  ss_in << CMAKE_PROJECT_SOURCE_DIR << "/test/data/Pizza/input_" << num_test
        << ".txt";
  ss_exp << CMAKE_PROJECT_SOURCE_DIR << "/test/data/Pizza/expected_" << num_test
         << ".txt";

  std::ifstream in(ss_in.str());
  std::ifstream exp(ss_exp.str());
  std::ostringstream expected;
  expected << exp.rdbuf();

  if (!exp.is_open() || !in.is_open()) {
    FAIL() << "Failed to open expected output file";
  }

  EXPECT_EQ(Pizza::handle_pizza_city(in), expected.str());
}
