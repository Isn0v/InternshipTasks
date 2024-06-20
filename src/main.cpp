// #include <DigitalLab/DigitalLab.hpp>
#include "TreasureHunt/TreasureHunt.hpp"
#include <TreasureHunt/TreasureHunt.hpp>

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <vector>

int main() {
  std::vector<Wall> walls = {
      Wall(0, 0, 100, 0),     Wall(0, 0, 0, 100),    Wall(0, 100, 100, 100),
      Wall(100, 0, 100, 100), Wall(20, 0, 37, 100),  Wall(40, 0, 76, 100),
      Wall(85, 0, 0, 75),     Wall(100, 90, 0, 90),  Wall(0, 71, 100, 61),
      Wall(0, 14, 100, 38),   Wall(100, 47, 47, 100)};

  // auto polygons = ray_casting(walls, Point(80, 45));
  // for (const auto &polygon : polygons) {
  //   std::cout << polygon << std::endl;
  // }
  // std::cout << std::endl;
  // std::cout << calc_number_of_walls(walls, Point(54.5, 55.4)) << std::endl;
  std::cout << calc_number_of_walls(walls, Point(70, 87)) << std::endl;

  return 0;
}