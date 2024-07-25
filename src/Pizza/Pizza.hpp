#pragma once

#include <iostream>
#include <unordered_map>
#include <unordered_set>
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

typedef std::pair<Point, std::size_t> pizza_data_t;

struct PointHash {
  std::size_t operator()(const Point &point) const;
};

class Pizza_City {
 private:
  std::size_t **city_field_;
  std::size_t field_height_;
  std::size_t field_width_;

  std::vector<pizza_data_t> pizza_data_;
  std::vector<num_permutation_t>
      correct_expansions;  // pizzeria_id -> (up, right, down, left)

  std::vector<Point> get_possible_expansion_moves(const Point &pizzeria) const;
  double get_distance_to_nearest_busy_point(Point current_point,
                                            std::size_t pizzeria_id,
                                            std::size_t filling_step);
  bool is_pizzeria_point_reachable_to_other_pizzeria(
      const Point &point, std::size_t pizzeria_id) const;

 public:
  
  void iterating_coverage();
  std::vector<num_permutation_t> get_correct_expansions() const;
  

  Pizza_City(std::size_t field_height, std::size_t field_width,
             const std::vector<pizza_data_t> &pizza_data);

  ~Pizza_City();
};

std::string handle_iterating_pizza_city(std::istream &input);

}  // namespace Pizza