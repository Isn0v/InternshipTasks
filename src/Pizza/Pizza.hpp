#pragma once

#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Pizza {
  
// 0 -> north, 1 -> east, 2 -> south, 3 -> west indexes for coverage
typedef std::tuple<std::size_t, std::size_t, std::size_t, std::size_t>
    num_permutation_t;
class Point {
 private:
  std::size_t x_, y_;

 public:
  std::size_t x() const;
  std::size_t y() const;

  Point() = default;
  Point(std::size_t x, std::size_t y);
  Point(const Point &point);
  Point &operator=(const Point &point);

  bool operator==(const Point &point) const;
  bool operator!=(const Point &point) const;

  friend std::ostream &operator<<(std::ostream &os, const Point &wall);
};

struct PointHash {
  std::size_t operator()(const Point &point) const;
};

/** @brief A class representing a city with pizza shops and covered area.
*
* This class represents a city with pizza shops and their covered area.
* It provides methods to cover all city with pizza shops and to check if
* the covered area is correctly covered.
*
* @details
* The class stores the city's size, the pizza shops' coordinates and capacities,
* and the covered area. The covered area is represented by a 2D array of size
* field_height_ and field_width_, where each element represents a cell in
* the city. If a cell is covered by a pizza shop, the value of the cell is
* equal to the index of the pizza shop + 1. If the cell is not covered, the
* value is 0.
*
* The class provides methods to add and remove coverage of a pizza shop on
* the covered area, and to check if all pizza shops are covered.
*
* The class also provides methods to get the covered area and to get the
* correct permutations of coverage of each pizza shop.
*
* The class' destructor frees the memory allocated for the covered area.
*/
class Pizza_City {
 private:
  std::size_t **city_field_;
  std::size_t field_height_;
  std::size_t field_width_;

  std::vector<Point> pizza_coords_;
  std::vector<std::size_t> pizza_capacities_;

  std::vector<num_permutation_t> correct_permutations_;

  bool add_pizzeria_coverage(const Point &pizzeria,
                             const num_permutation_t &permutation);
  void remove_pizzeria_coverage(const Point &pizzeria,
                                const num_permutation_t &permutation);

  bool backtracking(std::size_t pizzeria_id);

 public:
  Pizza_City(std::size_t field_height, std::size_t field_width,
             const std::vector<Point> &pizza_coords,
             const std::vector<std::size_t> &pizza_capacities);

  bool cover_all_city();
  bool is_city_covered_correctly() const;
  std::size_t **get_city_field() const;
  const std::vector<num_permutation_t> &get_correct_permutations() const;

  ~Pizza_City();
};

/**
 * @brief Finds all permutations of size target in the interval [0, capacity].
 *
 * @param capacity The upper bound of the interval.
 * @param target The size of the permutations to find.
 * @return A vector of tuples representing the permutations.
 */
std::vector<num_permutation_t> find_permutations(std::size_t capacity,
                                                 std::size_t target);

/**
 * @brief Gets the value at the specified index from the given permutation.
 *
 * @param permutation The permutation to get the value from.
 * @param index The index of the value to get.
 * @return The value at the specified index.
 * @throws std::invalid_argument If the index is invalid.
 */
std::size_t get_permutation_value(const num_permutation_t &permutation,
                                  std::size_t index);

/**
 * @brief Parses the input string and generates a string with the solution.
 *
 * @param input The input string to parse.
 * @return The generated string with the solution.
 */
std::string handle_pizza_city(const std::string &input);

}  // namespace Pizza