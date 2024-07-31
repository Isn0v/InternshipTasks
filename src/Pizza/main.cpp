#include <iostream>
#include "Pizza.hpp"

int main() {
  try{
  std::cout << Pizza::handle_pizza_city(std::cin);
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}