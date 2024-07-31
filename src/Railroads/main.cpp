#include <iostream>
#include "Railroads.hpp"

int main() {
  try{
  std::cout << Railroads::handle_coaches(std::cin);
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}