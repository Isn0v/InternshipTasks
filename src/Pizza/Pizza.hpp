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
  bool is_city_covered() const;
  std::size_t **get_city_field() const;
  const std::vector<num_permutation_t> &get_correct_permutations() const;

  ~Pizza_City();
};

std::vector<num_permutation_t> find_permutations(std::size_t capacity,
                                                 std::size_t target);
std::size_t get_permutation_value(const num_permutation_t &permutation,
                                  std::size_t index);

std::string handle_pizza_city(const std::string &input);

}  // namespace Pizza