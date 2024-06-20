#pragma once

#include <cstddef>
#include <iostream>
#include <unordered_set>
#include <vector>

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

std::unordered_set<Wall, WallHash>
ray_casting(const std::vector<Wall> &initial_walls, const Point &casting_point);

std::size_t calc_number_of_walls(const std::vector<Wall> &initial_walls,
                                 const Point &initial_treasure_point);