// #include <gtest/gtest.h>

// #include <Pizza/Pizza.hpp>

// // NOTE: Results differs from what is presented in the task description.
// //       Because of the implementation details of the algorithm, we have various
// //       ways of covering the city.


// // deprecated
// // ------------------------------------------------------------------------------
// TEST(PizzaTest, TestCase2x2) {
//   std::size_t n = 2, m = 2;
//   std::vector<Pizza::Point> pizza_coords = {
//       Pizza::Point(0, 0),
//       Pizza::Point(1, 1),
//   };
//   std::vector<std::size_t> pizza_capacities = {1, 1};
//   Pizza::Pizza_City city(m, n, pizza_coords, pizza_capacities);
//   std::vector<Pizza::num_permutation_t> expected_permutations = {
//       {0, 1, 0, 0},
//       {0, 0, 0, 1},
//   };

//   if (city.cover_all_city()) {
//     EXPECT_EQ(city.get_correct_permutations(), expected_permutations);
//   } else {
//     FAIL() << "Failed to cover all city";
//   }
//   EXPECT_TRUE(city.is_city_covered_correctly());
// }

// TEST(PizzaTest, TestCase5x5) {
//   std::size_t n = 5, m = 5;
//   std::vector<Pizza::Point> pizza_coords = {
//       Pizza::Point(0, 2), Pizza::Point(1, 0), Pizza::Point(1, 4),
//       Pizza::Point(2, 2), Pizza::Point(4, 1), Pizza::Point(4, 3),
//   };
//   std::vector<std::size_t> pizza_capacities = {2, 4, 4, 5, 2, 2};
//   Pizza::Pizza_City city(m, n, pizza_coords, pizza_capacities);
//   std::vector<Pizza::num_permutation_t> expected_permutations = {
//       {1, 0, 1, 0}, {0, 2, 1, 1}, {1, 2, 0, 1},
//       {1, 2, 1, 1}, {1, 0, 0, 1}, {0, 0, 1, 1}};

//   if (city.cover_all_city()) {
//     EXPECT_EQ(city.get_correct_permutations(), expected_permutations);
//   } else {
//     FAIL() << "Failed to cover all city";
//   }
//   EXPECT_TRUE(city.is_city_covered_correctly());
// }

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
// // ------------------------------------------------------------------------------

// TEST(PizzaTest, IntegrationTest) {
//   auto inp =
//       "2 2 2\n"
//       "1 1 1\n"
//       "2 2 1\n"
//       "5 5 6\n"
//       "1 3 2\n"
//       "2 1 4\n"
//       "2 5 4\n"
//       "3 3 5\n"
//       "5 2 2\n"
//       "5 4 2\n"
//       "0\n";

//   std::istringstream in(inp);

//   std::string expected =
//       "Case 1:\n"
//       "0 1 0 0\n"
//       "0 0 0 1\n"
//       "\n"
//       "Case 2:\n"
//       "1 0 1 0\n"
//       "0 2 1 1\n"
//       "1 2 0 1\n"
//       "1 2 1 1\n"
//       "1 0 0 1\n"
//       "0 0 1 1\n"
//       "\n";
//   EXPECT_EQ(expected, Pizza::handle_pizza_city(in));
// }


// TEST(PizzaTest, TestCaseMinimalCity1x1) {
//   std::size_t n = 1, m = 1;
//   std::vector<Pizza::Point> pizza_coords = {
//       Pizza::Point(0, 0),
//   };
//   std::vector<std::size_t> pizza_capacities = {0};
//   Pizza::Pizza_City city(m, n, pizza_coords, pizza_capacities);
//   std::vector<Pizza::num_permutation_t> expected_permutations = {{0, 0, 0, 0}};

//   if (city.cover_all_city()) {
//     EXPECT_EQ(city.get_correct_permutations(), expected_permutations);
//   } else {
//     FAIL() << "Failed to cover all city";
//   }
//   EXPECT_TRUE(city.is_city_covered_correctly());
// }

// TEST(PizzaTest, TestCaseHorizontalCity1x4) {
//   std::size_t n = 4, m = 1;
//   std::vector<Pizza::Point> pizza_coords = {
//       Pizza::Point(0, 0),
//   };
//   std::vector<std::size_t> pizza_capacities = {3};
//   Pizza::Pizza_City city(m, n, pizza_coords, pizza_capacities);
//   std::vector<Pizza::num_permutation_t> expected_permutations = {{0, 3, 0, 0}};

//   if (city.cover_all_city()) {
//     EXPECT_EQ(city.get_correct_permutations(), expected_permutations);
//   } else {
//     FAIL() << "Failed to cover all city";
//   }
//   EXPECT_TRUE(city.is_city_covered_correctly());
// }

// TEST(PizzaTest, TestCaseVerticalCity4x1) {
//   std::size_t n = 1, m = 4;
//   std::vector<Pizza::Point> pizza_coords = {
//       Pizza::Point(0, 0),
//   };
//   std::vector<std::size_t> pizza_capacities = {3};
//   Pizza::Pizza_City city(m, n, pizza_coords, pizza_capacities);
//   std::vector<Pizza::num_permutation_t> expected_permutations = {{0, 0, 3, 0}};

//   if (city.cover_all_city()) {
//     EXPECT_EQ(city.get_correct_permutations(), expected_permutations);
//   } else {
//     FAIL() << "Failed to cover all city";
//   }
//   EXPECT_TRUE(city.is_city_covered_correctly());
// }

// TEST(PizzaTest, TestCaseFullCity2x2) {
//   std::size_t n = 2, m = 2;
//   std::vector<Pizza::Point> pizza_coords = {
//       Pizza::Point(0, 0),
//       Pizza::Point(0, 1),
//       Pizza::Point(1, 0),
//       Pizza::Point(1, 1),
//   };
//   std::vector<std::size_t> pizza_capacities = {0, 0, 0, 0};
//   Pizza::Pizza_City city(m, n, pizza_coords, pizza_capacities);
//   std::vector<Pizza::num_permutation_t> expected_permutations = {
//       {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

//   if (city.cover_all_city()) {
//     EXPECT_EQ(city.get_correct_permutations(), expected_permutations);
//   } else {
//     FAIL() << "Failed to cover all city";
//   }
//   EXPECT_TRUE(city.is_city_covered_correctly());
// }

// TEST(PizzaTest, TestCaseFullStrechedCity5x5) {
//   std::size_t n = 5, m = 5;
//   std::vector<Pizza::Point> pizza_coords = {
//       Pizza::Point(0, 0), Pizza::Point(1, 1), Pizza::Point(2, 2),
//       Pizza::Point(3, 3), Pizza::Point(4, 4)};
//   std::vector<std::size_t> pizza_capacities = {0, 2, 4, 6, 8};
//   Pizza::Pizza_City city(m, n, pizza_coords, pizza_capacities);
//   std::vector<Pizza::num_permutation_t> expected_permutations = {
//       {0, 0, 0, 0}, {1, 0, 0, 1}, {2, 0, 0, 2}, {3, 0, 0, 3}, {4, 0, 0, 4}};

//   if (city.cover_all_city()) {
//     EXPECT_EQ(city.get_correct_permutations(), expected_permutations);
//   } else {
//     FAIL() << "Failed to cover all city";
//   }
//   EXPECT_TRUE(city.is_city_covered_correctly());
// }

// TEST(PizzaTest, TestCaseCityWithPizzaAtTheCorner2x2) {
//   std::size_t n = 2, m = 2;
//   std::vector<Pizza::Point> pizza_coords = {
//       Pizza::Point(0, 0)};
//   std::vector<std::size_t> pizza_capacities = {2};
//   Pizza::Pizza_City city(m, n, pizza_coords, pizza_capacities);
  
//   if (city.cover_all_city()) {
//     FAIL() << "Should not cover all city";
//   } else {
//     EXPECT_FALSE(city.is_city_covered_correctly());
//   }
// }