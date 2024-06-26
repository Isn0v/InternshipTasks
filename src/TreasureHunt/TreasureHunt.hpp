#pragma once

#include <cstddef>
#include <iostream>
#include <unordered_set>
#include <vector>

namespace Treasure_Hunt {

class Point {
 private:
  double x_, y_;

 public:
  double x() const;
  double y() const;

  Point() = default;
  Point(double x, double y);
  Point(const Point &point);
  Point &operator=(const Point &point);

  double get_distance_with_point(const Point &point) const;

  bool out_of_field() const;

  bool operator==(const Point &point) const;
  bool operator!=(const Point &point) const;

  friend std::ostream &operator<<(std::ostream &os, const Point &wall);
};

struct PointHash {
  std::size_t operator()(const Point &point) const;
};

class Wall {
 private:
  double x1_, y1_, x2_, y2_;

 public:
  Wall() = default;
  Wall(double x1, double y1, double x2, double y2);
  Wall(const Wall &wall);

  Wall &operator=(const Wall &wall);

  double x1() const;
  double y1() const;
  double x2() const;
  double y2() const;

  Point get_center() const;

  static bool is_parallel(const Wall &wall1, const Wall &wall2);
  static Point intersection_point(const Wall &wall1, const Wall &wall2);

  double get_distance_with_point(const Point &point) const;

  bool operator==(const Wall &wall) const;

  friend std::ostream &operator<<(std::ostream &os, const Wall &wall);
};

struct WallHash {
  std::size_t operator()(const Wall &wall) const;
};

/**
 * @brief      Casts rays from a given point in a field and returns the nearest
 * walls that the rays intersect with.
 *
 * @param[in]  initial_walls   The initial walls in the field
 * @param[in]  casting_point   The point from which to cast the rays
 *
 * @return     An unordered set of walls that the rays intersect with
 */
std::unordered_set<Wall, WallHash> ray_casting(
    const std::vector<Wall> &initial_walls, const Point &casting_point);

/**
 * @brief      Calculates the number of doors in a field.
 *
 * @param[in]  initial_walls   The initial walls in the field
 * @param[in]  initial_treasure_point   The initial treasure point
 *
 * @return     The number of doors in the field
 *
 * The algorithm returns the number of walls we should go through to get to
 * treasure_point as specified in task.
 */
std::size_t calc_number_of_doors(const std::vector<Wall> &initial_walls,
                                 const Point &initial_treasure_point);

/**
 * @brief Parses the input string and handles the Treasure Hunt based on the
 * provided input.
 *
 * @param input The input string containing the field information and the
 * treasure point.
 *
 * @return The solution string containing the number of doors to the treasure.
 *
 * The input string is expected to be in the following format:
 * [NUMBER_OF_WALLS]
 * [WALL1_X1 WALL1_Y1 WALL1_X2 WALL1_Y2]
 * [WALL2_X1 WALL2_Y1 WALL2_X2 WALL2_Y2]
 * ...
 * [WALLn_X1 WALLn_Y1 WALLn_X2 WALLn_Y2]
 * [TREASURE_X TREASURE_Y].
 *
 * The function initializes the field based on the input string and calculates
 * the number of doors to the treasure using the `calc_number_of_doors`
 * function. It then returns the solution string like Number of doors = [NUMBER_OF_DOORS].
 */
std::string handle_treasure_hunt(const std::string &input);

}  // namespace Treasure_Hunt