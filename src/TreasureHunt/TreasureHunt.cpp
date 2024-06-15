#include "TreasureHunt.hpp"

struct Wall {
  double x1, y1, x2, y2;
  Wall() = default;
  Wall(double x1, double y1, double x2, double y2)
      : x1(x1), y1(y1), x2(x2), y2(y2) {}

  point_t get_center() const {
    return std::make_pair((x1 + x2) / 2, (y1 + y2) / 2);
  }

  double get_distance_with_point(point_t const &point) const {
    double a = y2 - y1;
    double b = x1 - x2;
    double c = x2 * y1 - x1 * y2;

    double denominator = std::sqrt(a * a + b * b);
    return std::abs(a * point.first + b * point.second + c) / denominator;
  }
} typedef Wall;

size_t calc_number_of_walls(std::vector<Wall> const &walls,
                            point_t const &treasure_point) {
  size_t number_of_walls = -1;

  auto distance_generator = [&, i = static_cast<size_t>(0)]() mutable {
    return walls[i++].get_distance_with_point(treasure_point);
  };
  std::vector<double> distances(walls.size());
  std::generate(distances.begin(), distances.end(), distance_generator);
  std::sort(distances.begin(), distances.end());

  return number_of_walls;
}