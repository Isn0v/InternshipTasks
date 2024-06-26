#include <gtest/gtest.h>

#include <Pizza/Pizza.hpp>

// NOTE: Results differs from what is presented in the task description.
//       Because of the implementation details of the algorithm, we have various
//       ways of covering the city.
TEST(PizzaTest, TestCase2x2) {
  std::size_t n = 2, m = 2;
  std::vector<Pizza::Point> pizza_coords = {
      Pizza::Point(0, 0),
      Pizza::Point(1, 1),
  };
  std::vector<std::size_t> pizza_capacities = {1, 1};
  Pizza::Pizza_City city(m, n, pizza_coords, pizza_capacities);
  std::vector<Pizza::num_permutation_t> expected_permutations = {
      {0, 1, 0, 0},
      {0, 0, 0, 1},
  };

  if (city.cover_all_city()) {
    EXPECT_EQ(city.get_correct_permutations(), expected_permutations);
  } else {
    FAIL() << "Failed to cover all city";
  }
  EXPECT_TRUE(city.is_city_covered_correctly());
}

TEST(PizzaTest, TestCase5x5) {
  std::size_t n = 5, m = 5;
  std::vector<Pizza::Point> pizza_coords = {
      Pizza::Point(0, 2), Pizza::Point(1, 0), Pizza::Point(1, 4),
      Pizza::Point(2, 2), Pizza::Point(4, 1), Pizza::Point(4, 3),
  };
  std::vector<std::size_t> pizza_capacities = {2, 4, 4, 5, 2, 2};
  Pizza::Pizza_City city(m, n, pizza_coords, pizza_capacities);
  std::vector<Pizza::num_permutation_t> expected_permutations = {
      {1, 0, 1, 0}, {0, 2, 1, 1}, {1, 2, 0, 1},
      {1, 2, 1, 1}, {1, 0, 0, 1}, {0, 0, 1, 1}};

  if (city.cover_all_city()) {
    EXPECT_EQ(city.get_correct_permutations(), expected_permutations);
  } else {
    FAIL() << "Failed to cover all city";
  }
  EXPECT_TRUE(city.is_city_covered_correctly());
}

TEST(PizzaTest, IntegrationTest) {
  std::string inp =
      "2 2 2\n"
      "1 1 1\n"
      "2 2 1\n"
      "5 5 6\n"
      "1 3 2\n"
      "2 1 4\n"
      "2 5 4\n"
      "3 3 5\n"
      "5 2 2\n"
      "5 4 2\n"
      "0\n";

  std::string expected =
      "Case 1:\n"
      "0 1 0 0\n"
      "0 0 0 1\n"
      "\n"
      "Case 2:\n"
      "1 0 1 0\n"
      "0 2 1 1\n"
      "1 2 0 1\n"
      "1 2 1 1\n"
      "1 0 0 1\n"
      "0 0 1 1\n"
      "\n";
  EXPECT_EQ(expected, Pizza::handle_pizza_city(inp));
}