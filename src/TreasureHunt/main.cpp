#include <iostream>
#include "TreasureHunt.hpp"

int main() {
  try{
  std::cout << Treasure_Hunt::handle_treasure_hunt(std::cin);
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}