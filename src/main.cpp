// #include <DigitalLab/DigitalLab.hpp>
// #include <TreasureHunt/TreasureHunt.hpp>
// #include <RGBGame/RGBGame.hpp>
// #include <Railroads/Railroads.hpp>
#include <Pizza/Pizza.hpp>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

int main() {
  // std::size_t n = 5, m = 5;
  // std::vector<Pizza::Point> pizza_coords = {
  //     Pizza::Point(0, 2), Pizza::Point(1, 0), Pizza::Point(1, 4),
  //     Pizza::Point(2, 2), Pizza::Point(4, 1), Pizza::Point(4, 3),
  // };
  // std::vector<std::size_t> pizza_capacities = {2, 4, 4, 5, 2, 2};
  // Pizza::Pizza_City city(m, n, pizza_coords, pizza_capacities);

  // std::size_t n = 2, m = 2;
  // std::vector<Pizza::Point> pizza_coords = {
  //     Pizza::Point(0, 0),
  //     Pizza::Point(1, 1),
  // };
  // std::vector<std::size_t> pizza_capacities = {1, 1};
  // Pizza::Pizza_City city(m, n, pizza_coords, pizza_capacities);

  // if (city.cover_all_city()) {
  //   for (const auto &permutation : city.get_correct_permutations()) {
  //     std::cout << "(" << Pizza::get_permutation_value(permutation, 0) << ",
  //     "
  //               << Pizza::get_permutation_value(permutation, 1) << ", "
  //               << Pizza::get_permutation_value(permutation, 2) << ", "
  //               << Pizza::get_permutation_value(permutation, 3) << ")"
  //               << std::endl;
  //   }
  // }

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


  std::cout << Pizza::handle_pizza_city(inp);
  return 0;
}