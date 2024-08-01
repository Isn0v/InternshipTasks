
#include <gtest/gtest.h>
#include <Pizza/Pizza.hpp>
#include <sstream>
#include <fstream>

// // NOTE: Results differs from what is presented in the task description.
// //       Because of the implementation details of the algorithm, we have various
// //       ways of covering the city.


class PizzaTest : public ::testing::TestWithParam<int> {};
INSTANTIATE_TEST_SUITE_P(Pizza, PizzaTest, ::testing::Range(1, 10));

TEST_P(PizzaTest, IntegrationTest) {
  int num_test = GetParam();
  std::stringstream ss_in, ss_exp;

  ss_in << CMAKE_PROJECT_SOURCE_DIR << "/test/data/Pizza/input_"
        << num_test << ".txt";
  ss_exp << CMAKE_PROJECT_SOURCE_DIR << "/test/data/Pizza/expected_"
         << num_test << ".txt";

  std::ifstream in(ss_in.str());
  std::ifstream exp(ss_exp.str());
  std::ostringstream expected;
  expected << exp.rdbuf();

  if (!exp.is_open() || !in.is_open()) {
    FAIL() << "Failed to open expected output file";
  }

  EXPECT_EQ(Pizza::handle_pizza_city(in), expected.str());
}
