#include <iostream>
#include "RGBGame.hpp"

int main() {
  try{
  std::cout << RGB_Game::handle_rgb_game(std::cin);
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}