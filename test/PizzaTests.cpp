
// TODO add test with incorrect behaviour

// TEST(PizzaTest, IncorrectCase5x5) {
//   std::size_t n = 5, m = 5;
//   std::vector<Pizza::Point> pizza_coords = {
//       Pizza::Point(1, 0),
//       Pizza::Point(1, 4),
//       Pizza::Point(2, 2),
//   };
//   std::vector<std::size_t> pizza_capacities = {2, 4, 4};
//   Pizza::Pizza_City city(m, n, pizza_coords, pizza_capacities);

//   if (city.cover_all_city()) {
//     FAIL() << "Should not cover all city";
//   } else {
//     EXPECT_FALSE(city.is_city_covered_correctly());
//   }
// }


// TEST(PizzaTest, TestCaseFullStrechedCity5x5) {
//   std::size_t n = 5, m = 5;
//   std::vector<Pizza::Point> pizza_coords = {
//       Pizza::Point(0, 0), Pizza::Point(1, 1), Pizza::Point(2, 2),
//       Pizza::Point(3, 3), Pizza::Point(4, 4)};
//   std::vector<std::size_t> pizza_capacities = {0, 2, 4, 6, 8};
//   Pizza::Pizza_City city(m, n, pizza_coords, pizza_capacities);
//   std::vector<Pizza::num_expansion_t> expected_permutations = {
//       {0, 0, 0, 0}, {1, 0, 0, 1}, {2, 0, 0, 2}, {3, 0, 0, 3}, {4, 0, 0, 4}};

//   if (city.cover_all_city()) {
//     EXPECT_EQ(city.get_correct_permutations(), expected_permutations);
//   } else {
//     FAIL() << "Failed to cover all city";
//   }
//   EXPECT_TRUE(city.is_city_covered_correctly());
// }


#include <gtest/gtest.h>
#include <Pizza/Pizza.hpp>
#include <sstream>
#include <fstream>

// // NOTE: Results differs from what is presented in the task description.
// //       Because of the implementation details of the algorithm, we have various
// //       ways of covering the city.


class PizzaTest : public ::testing::TestWithParam<int> {};
INSTANTIATE_TEST_SUITE_P(Pizza, PizzaTest, ::testing::Range(1, 8));

// TODO add more tests if needed
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
