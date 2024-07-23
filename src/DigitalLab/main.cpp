#include <fstream>
#include <iostream>
#include <string>

#include "DigitalLab.hpp"

/**
 * @file main.cpp
 * @brief Entry point for the DigitalLab program.
 *
 * This program is the entry point for the DigitalLab problem.
 * It reads the input from the standard input, processes the input
 * according to the problem and outputs the result to the standard output.
 *
 * The program supports two command line arguments: an input file and an
 * output file. If the input file is not specified, the program will read
 * from the standard input. If the output file is not specified, the
 * program will write to the standard output.
 *
 * @param argc The number of command line arguments.
 * @param argv The array of command line arguments.
 *
 * @return 0 if the program runs successfully, 1 if there is an error.
 */
int main(int argc, char **argv) {
  std::string res;
  if (argc == 1) {
    res = Digital_Lab::handle_digital_lab(std::cin);
    std::cout << std::endl << res;
  } else if (argc == 3) {
    std::ifstream input(argv[1]);
    if (!input.is_open()) {
      std::cerr
          << "Failed to open input file: " << argv[1] << std::endl
          << "Usage: " << ".\\Digital_Lab_run.exe"
          << " <input_file> <output_file> (stdin, stdout if not specified)"
          << std::endl;
      return 1;
    }
    res = Digital_Lab::handle_digital_lab(input);
    std::ofstream output(argv[2]);
    output.write(res.c_str(), res.size());
  } else {
    std::cerr << "Usage: " << ".\\Digital_Lab_run.exe"
              << " <input_file> <output_file> (stdin, stdout if not specified)"
              << std::endl;
    return 1;
  }
  return 0;
}
