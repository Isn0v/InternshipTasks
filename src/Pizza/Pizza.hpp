#pragma once

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace Pizza {

// 0 -> north, 1 -> east, 2 -> south, 3 -> west indexes for coverage
typedef std::tuple<std::size_t, std::size_t, std::size_t, std::size_t>
    num_expansion_t;

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

  static double get_distance_with_point(const Point &point1,
                                        const Point &point2);
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
  std::size_t busy_points_count = 0;

  std::vector<pizza_data_t> pizza_data_;
  std::vector<num_expansion_t> correct_expansions;
  std::vector<std::size_t> currently_expanded_;
  std::vector<Point> find_empty_points();

  std::vector<std::vector<bool>> fixed_expansions;
  std::vector<std::vector<bool>> untouchable_points;


  std::vector<Point> get_expansion_moves(const Point &pizzeria) const;
  std::vector<Point> get_potential_expansion_moves(const Point &pizzeria) const;

  bool fix_expansion(std::size_t pizzeria_id);
  bool point_inside_city(const Point &point) const;
  bool point_is_occupied(const Point &point) const;

  double get_distance_to_nearest_busy_point(const Point &current_point,
                                            std::size_t pizzeria_id);

  bool is_pizzeria_point_reachable_to_other_pizzerias(
      const Point &point, std::size_t pizzeria_id) const;

  void expand_unreachable_points(std::size_t pizzeria_id);
  void expand_pizzeria_with_no_choices(std::size_t pizzeria_id);

  void expand_by_point(const Point &point, std::size_t pizzeria_id);
  void reduce_by_point(const Point &point);


  std::vector<Point> get_edge_expansion_points(std::size_t pizzeria_id);

  std::size_t get_pizzeria_id_by_point(const Point &point);

 public:
  void iterating_coverage();
  std::vector<num_expansion_t> get_correct_expansions() const;

  Pizza_City(std::size_t field_height, std::size_t field_width,
             const std::vector<pizza_data_t> &pizza_data);

  ~Pizza_City();
};

std::string handle_pizza_city(std::istream &input);

}  // namespace Pizza