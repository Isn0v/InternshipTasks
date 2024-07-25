#include <iostream>
#include <sstream>

#include "Pizza.hpp"

int main() {
  // std::cout << Pizza::handle_pizza_city(std::cin);

  // std::size_t n = 5, m = 5;
  // std::vector<Pizza::Point> pizza_coords = {
  //     Pizza::Point(0, 0), Pizza::Point(1, 1), Pizza::Point(2, 2),
  //     Pizza::Point(3, 3), Pizza::Point(4, 4)};
  // std::vector<std::size_t> pizza_capacities = {0, 2, 4, 6, 8};
  // Pizza::Pizza_City city(m, n, pizza_coords, pizza_capacities);
  // std::vector<Pizza::num_permutation_t> expected_permutations = {
  //     {0, 0, 0, 0}, {1, 0, 0, 1}, {2, 0, 0, 2}, {3, 0, 0, 3}, {4, 0, 0, 4}};

  // city.iterating_coverage();
  // for (auto permutation : city.correct_expansion) {
  //   std::cout << std::get<0>(permutation) << " " << std::get<1>(permutation)
  //             << " " << std::get<2>(permutation) << " "
  //             << std::get<3>(permutation) << std::endl;
  // }

  std::stringstream ss;
  ss << "2 2 2\n"
     << "1 1 1\n"
     << "2 2 1\n"
     << "5 5 6\n"
     << "1 3 2\n"
     << "2 1 4\n"
     << "2 5 4\n"
     << "3 3 5\n"
     << "5 2 2\n"
     << "5 4 2\n"
     << "0\n";
  // std::cout << Pizza::handle_iterating_pizza_city(ss);
  std::cout << Pizza::handle_iterating_pizza_city(std::cin);
  return 0;
}