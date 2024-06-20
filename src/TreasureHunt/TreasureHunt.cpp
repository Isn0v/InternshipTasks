#include "TreasureHunt.hpp"
#include <limits>

#define PHI_STEP 5
#define RAY_COUNT 360 / PHI_STEP

#define FIELD_START_BOUNDARY 0
#define FIELD_END_BOUNDARY 100

#include <algorithm>
#include <cmath>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

Point::Point(double x, double y) : x_(x), y_(y) {}

Point::Point(const Point &point) : x_(point.x_), y_(point.y_) {}

Point &Point::operator=(const Point &point) {
  if (this != &point) {
    x_ = point.x_;
    y_ = point.y_;
  }
  return *this;
}

double Point::get_distance_with_point(const Point &point) const {
  double a = y_ - point.y_;
  double b = x_ - point.x_;
  return std::sqrt(a * a + b * b);
}

bool Point::out_of_field() const {
  return x_ < FIELD_START_BOUNDARY || x_ > FIELD_END_BOUNDARY ||
         y_ < FIELD_START_BOUNDARY || y_ > FIELD_END_BOUNDARY;
}

std::ostream &operator<<(std::ostream &os, const Point &wall) {
  os << "(" << wall.x_ << "," << wall.y_ << ")";
  return os;
}

bool Point::operator==(const Point &point) const {
  return x_ == point.x_ && y_ == point.y_;
}

bool Point::operator!=(const Point &point) const { return !(*this == point); }

double Point::x() const { return x_; }

double Point::y() const { return y_; }

std::size_t PointHash::operator()(const Point &point) const {
  return std::hash<double>()(point.x()) ^ std::hash<double>()(point.y());
}

Wall::Wall(double x1, double y1, double x2, double y2)
    : x1_(x1), y1_(y1), x2_(x2), y2_(y2) {}

Wall::Wall(const Wall &wall)
    : x1_(wall.x1_), y1_(wall.y1_), x2_(wall.x2_), y2_(wall.y2_) {}

Wall &Wall::operator=(const Wall &wall) {
  if (this != &wall) {
    x1_ = wall.x1_;
    y1_ = wall.y1_;
    x2_ = wall.x2_;
    y2_ = wall.y2_;
  }
  return *this;
}

Point Wall::get_center() const {
  return Point((x1_ + x2_) / 2, (y1_ + y2_) / 2);
}

double Wall::x1() const { return x1_; }
double Wall::y1() const { return y1_; }
double Wall::x2() const { return x2_; }
double Wall::y2() const { return y2_; }

Point Wall::intersection_point(const Wall &wall1, const Wall &wall2) {
  if (is_parallel(wall1, wall2)) {
    return Point(std::numeric_limits<double>::quiet_NaN(),
                 std::numeric_limits<double>::quiet_NaN());
  }

  double x1 = wall1.x1_, y1 = wall1.y1_, x2 = wall1.x2_, y2 = wall1.y2_;
  double x3 = wall2.x1_, y3 = wall2.y1_, x4 = wall2.x2_, y4 = wall2.y2_;

  double Px =
      ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) /
      ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));

  double Py =
      ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) /
      ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));

  return Point(Px, Py);
}

bool Wall::is_parallel(const Wall &wall1, const Wall &wall2) {
  Wall Ox(FIELD_START_BOUNDARY, FIELD_START_BOUNDARY, FIELD_END_BOUNDARY,
          FIELD_START_BOUNDARY);
  double dx1 = std::abs(wall1.x2_ - wall1.x1_),
         dx2 = std::abs(wall2.x2_ - wall2.x1_), dOx = std::abs(Ox.x2_ - Ox.x1_);
  double dy1 = std::abs(wall1.y2_ - wall1.y1_),
         dy2 = std::abs(wall2.y2_ - wall2.y1_), dOy = std::abs(Ox.y2_ - Ox.y1_);

  double cos_a1 = (dx1 * dOx + dy1 * dOy) / (dx1 * dx1 + dy1 * dy1);
  double cos_a2 = (dx2 * dOx + dy2 * dOy) / (dx2 * dx2 + dy2 * dy2);

  return cos_a1 == cos_a2;

  // return std::abs(wall1.x2_ - wall1.x1_) * std::abs(wall2.y2_ - wall2.y1_) ==
  //        std::abs(wall2.x2_ - wall2.x1_) * std::abs(wall1.y2_ - wall1.y1_);
}

double Wall::get_distance_with_point(const Point &point) const {
  double a = y2_ - y1_;
  double b = x1_ - x2_;
  double c = x2_ * y1_ - x1_ * y2_;

  double denominator = std::sqrt(a * a + b * b);
  return std::abs(a * point.x() + b * point.y() + c) / denominator;
}

std::ostream &operator<<(std::ostream &os, const Wall &wall) {
  os << "(" << wall.x1_ << "," << wall.y1_ << ") - (" << wall.x2_ << ","
     << wall.y2_ << ")";
  return os;
}

bool Wall::operator==(const Wall &wall) const {
  return x1_ == wall.x1_ && y1_ == wall.y1_ && x2_ == wall.x2_ &&
         y2_ == wall.y2_;
}

std::size_t WallHash::operator()(const Wall &wall) const {
  return std::hash<double>()(wall.x1()) ^ std::hash<double>()(wall.y1()) ^
         std::hash<double>()(wall.x2()) ^ std::hash<double>()(wall.y2());
}

std::unordered_set<Wall, WallHash>
ray_casting(const std::vector<Wall> &initial_walls,
            const Point &casting_point) {
  using Ray = Wall;

  auto ray_generator = [&, phi = 0]() mutable {
    int old_phi = phi++;
    return Ray(casting_point.x(), casting_point.y(),
               casting_point.x() + FIELD_END_BOUNDARY * std::cos(old_phi),
               casting_point.y() + FIELD_END_BOUNDARY * std::sin(old_phi));
  };

  std::vector<Ray> rays(RAY_COUNT);
  std::generate(rays.begin(), rays.end(), ray_generator);

  std::unordered_map<Point, Wall, PointHash> potential_walls;
  std::vector<Point> intersections;
  std::unordered_set<Wall, WallHash> polygon_walls;

  auto compare_points_by_dist = [&](const Point &p1, const Point &p2) {
    return p1.get_distance_with_point(casting_point) <
           p2.get_distance_with_point(casting_point);
  };

  auto point_beyond_ray = [](const Point &point, const Ray &ray) {
    double x1, y1, x2, y2;
    if (ray.x2() > ray.x1()) {
      x1 = ray.x1();
      x2 = ray.x2();
    } else {
      x1 = ray.x2();
      x2 = ray.x1();
    }

    if (ray.y2() > ray.y1()) {
      y1 = ray.y1();
      y2 = ray.y2();
    } else {
      y1 = ray.y2();
      y2 = ray.y1();
    }

    return point.x() < x1 || point.x() > x2 || point.y() < y1 || point.y() > y2;
  };

  for (const auto &ray : rays) {
    for (const auto &wall : initial_walls) {
      auto intersection = Wall::intersection_point(wall, ray);
      if (intersection.out_of_field() || Wall::is_parallel(ray, wall) ||
          point_beyond_ray(intersection, ray)) {
        continue;
      }

      intersections.push_back(intersection);
      potential_walls[intersections.back()] = wall;
    }

    auto min_intersection = std::min_element(
        intersections.begin(), intersections.end(), compare_points_by_dist);

    if (min_intersection != intersections.end()) {
      polygon_walls.insert(potential_walls[*min_intersection]);
    }

    potential_walls.clear();
    intersections.clear();
  }
  return polygon_walls;
}

void recursive_wall_traverse(std::vector<Wall> &walls, Point &treasure_point,
                             std::size_t &minimal_number_of_walls,
                             std::size_t number_of_walls = 0) {

  const std::vector<Wall> external_walls = {
      Wall(FIELD_START_BOUNDARY, FIELD_START_BOUNDARY, FIELD_END_BOUNDARY,
           FIELD_START_BOUNDARY),
      Wall(FIELD_START_BOUNDARY, FIELD_START_BOUNDARY, FIELD_START_BOUNDARY,
           FIELD_END_BOUNDARY),
      Wall(FIELD_START_BOUNDARY, FIELD_END_BOUNDARY, FIELD_END_BOUNDARY,
           FIELD_END_BOUNDARY),
      Wall(FIELD_END_BOUNDARY, FIELD_START_BOUNDARY, FIELD_END_BOUNDARY,
           FIELD_END_BOUNDARY)};

  auto polygon_walls = ray_casting(walls, treasure_point);
  for (const auto &wall : external_walls) {
    if (polygon_walls.find(wall) != polygon_walls.end()) {
      minimal_number_of_walls = minimal_number_of_walls < number_of_walls
                                    ? minimal_number_of_walls
                                    : number_of_walls;
      return;
    }
  }

  std::unordered_map<Wall, std::vector<Point>, WallHash>
      wall_point_intersections;

  for (auto wall1 : polygon_walls) {
    for (auto wall2 : polygon_walls) {
      if (wall1 == wall2) {
        continue;
      }
      auto intersection = Wall::intersection_point(wall1, wall2);
      if (intersection.out_of_field()) {
        continue;
      }
      wall_point_intersections[wall1].push_back(intersection);
      wall_point_intersections[wall2].push_back(intersection);
    }
  }
  for (const auto &wall_intersections : wall_point_intersections) {
    auto wall = wall_intersections.first;
    auto intersections = wall_intersections.second;
    auto center = wall.get_center();
    if (center.x() <= std::max(intersections[0].x(), intersections[1].x()) &&
        center.x() >= std::min(intersections[0].x(), intersections[1].x()) &&
        center.y() <= std::max(intersections[0].y(), intersections[1].y()) &&
        center.y() >= std::min(intersections[0].y(), intersections[1].y())) {
      double dx = (center.x() - treasure_point.x());
      double dy = (center.y() - treasure_point.y());
      double dist = std::sqrt(dx * dx + dy * dy);
      dx /= (dist / (std::numeric_limits<double>::epsilon() * 2));
      dy /= (dist / (std::numeric_limits<double>::epsilon() * 2));

      treasure_point = Point(center.x() + dx, center.y() + dy);
      recursive_wall_traverse(walls, treasure_point, minimal_number_of_walls,
                              number_of_walls + 1);
    }
  }
}

std::size_t calc_number_of_walls(const std::vector<Wall> &initial_walls,
                                 const Point &initial_treasure_point) {
  Point treasure_point = initial_treasure_point;
  std::size_t minimal_number_of_walls = std::numeric_limits<std::size_t>::max();

  recursive_wall_traverse(const_cast<std::vector<Wall> &>(initial_walls),
                          treasure_point, minimal_number_of_walls, 0);

  return minimal_number_of_walls;
}