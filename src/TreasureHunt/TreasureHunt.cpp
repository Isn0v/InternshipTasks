#include "TreasureHunt.hpp"

#include <limits>
#include <sstream>

#define MAXIMUM_WALLS 30
#define PHI_STEP 5
#define RAY_COUNT 360 / PHI_STEP

#define NUMBER_OF_OUTER_WALLS 4
#define FIELD_START_BOUNDARY 0
#define FIELD_END_BOUNDARY 100

#include <algorithm>
#include <cmath>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace Treasure_Hunt {

Point::Point(double x, double y) : x_(x), y_(y) {}

Point::Point(const Point &point) : x_(point.x_), y_(point.y_) {}

Point &Point::operator=(const Point &point) {
  if (this != &point) {
    x_ = point.x_;
    y_ = point.y_;
  }
  return *this;
}

/**
 * @brief Calculates the Euclidean distance between two points.
 *
 * @param point The point to calculate the distance with.
 * @return The Euclidean distance between the current point and the given point.
 *
 * This function calculates the Euclidean distance between the current point and
 * the given point. It does so by subtracting the y-coordinates and the
 * x-coordinates of the points, squaring the results, adding them together, and
 * then taking the square root of the sum.
 */
double Point::get_distance_with_point(const Point &point) const {
  double a = y_ - point.y_;
  double b = x_ - point.x_;

  return std::sqrt(a * a + b * b);
}

/**
 * @brief Checks if the point is out of the field.
 *
 * @return True if the point is out of the field, false otherwise.
 *
 * This function checks if the point is out of the field by comparing its
 * x-coordinate with the start and end boundaries of the field, and its
 * y-coordinate with the start and end boundaries of the field. If any of these
 * comparisons is true, the function returns true, indicating that the point is
 * out of the field.
 */
bool Point::out_of_field() const {
  bool out_of_field_x = x_ < FIELD_START_BOUNDARY || x_ > FIELD_END_BOUNDARY;
  bool out_of_field_y = y_ < FIELD_START_BOUNDARY || y_ > FIELD_END_BOUNDARY;

  return out_of_field_x || out_of_field_y;
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

/**
 * @brief Hash function for Point objects.
 *
 * @param point The Point object to be hashed.
 *
 * @return The hash value of the Point object.
 *
 * This function overloads the operator() function to provide a hash function
 * for Point objects. It combines the hash values of the x-coordinate and
 * the y-coordinate of the Point object using the bitwise XOR operator.
 */
std::size_t PointHash::operator()(const Point &point) const {
  // Get the hash values of the x-coordinate and y-coordinate of the Point
  // object
  std::size_t hash_x = std::hash<double>()(point.x());
  std::size_t hash_y = std::hash<double>()(point.y());

  // Combine the hash values using the bitwise XOR operator
  return hash_x ^ hash_y;
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

/**
 * @brief Calculates the intersection point of two walls.
 *
 * @param wall1 The first wall.
 * @param wall2 The second wall.
 * @return The intersection point of the two walls as a Point object. If the
 * walls are parallel, the Point object contains NaN values.
 *
 * This function calculates the intersection point of two walls. It first
 * checks if the walls are parallel and returns NaN values if they are. Then,
 * it calculates the intersection point using the parametric line equation.
 */
Point Wall::intersection_point(const Wall &wall1, const Wall &wall2) {
  // Check if the walls are parallel
  if (is_parallel(wall1, wall2)) {
    // Return NaN values if the walls are parallel
    return Point(std::numeric_limits<double>::quiet_NaN(),
                 std::numeric_limits<double>::quiet_NaN());
  }

  // Extract the coordinates of the four points defining the walls
  double x1 = wall1.x1_, y1 = wall1.y1_, x2 = wall1.x2_, y2 = wall1.y2_;
  double x3 = wall2.x1_, y3 = wall2.y1_, x4 = wall2.x2_, y4 = wall2.y2_;

  // Calculate the x-coordinate and y-coordinate of the intersection point
  double Px =
      ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) /
      ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));
  double Py =
      ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) /
      ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));

  // Return the intersection point as a Point object
  return Point(Px, Py);
}

/**
 * @brief Checks if two walls are parallel.
 *
 * @param wall1 The first wall.
 * @param wall2 The second wall.
 * @return True if the walls are parallel, false otherwise.
 *
 * This function checks if two walls are parallel. It calculates the cosine
 * of the angle between the two walls and Ox (the x-axis) and compares the
 * values. If the cosines are equal, the walls are parallel.
 */
bool Wall::is_parallel(const Wall &wall1, const Wall &wall2) {
  // Create a wall Ox representing the x-axis
  Wall Ox(FIELD_START_BOUNDARY, FIELD_START_BOUNDARY, FIELD_END_BOUNDARY,
          FIELD_START_BOUNDARY);

  // Calculate the lengths of the walls and Ox
  double dx1 = std::abs(wall1.x2_ - wall1.x1_);
  double dx2 = std::abs(wall2.x2_ - wall2.x1_);
  double dOx = std::abs(Ox.x2_ - Ox.x1_);

  double dy1 = std::abs(wall1.y2_ - wall1.y1_);
  double dy2 = std::abs(wall2.y2_ - wall2.y1_);
  double dOy = std::abs(Ox.y2_ - Ox.y1_);

  // Calculate the cosine of the angle between the walls and Ox
  double cos_a1 = (dx1 * dOx + dy1 * dOy) / (dx1 * dx1 + dy1 * dy1);
  double cos_a2 = (dx2 * dOx + dy2 * dOy) / (dx2 * dx2 + dy2 * dy2);

  // Check if the cosines are equal, indicating that the walls are parallel
  return cos_a1 == cos_a2;
}

/**
 * @brief Calculates the distance from a point to a wall.
 *
 * @param point The point to calculate the distance with.
 * @return The distance from the point to the wall.
 *
 * This function calculates the distance from a given point to a wall. It does
 * so by using the formula for the distance from a point to a line, which is
 * given by the equation:
 *
 *     |a*x + b*y + c| / sqrt(a^2 + b^2)
 *
 * where a = y2 - y1, b = x1 - x2, and c = x2*y1 - x1*y2. These values are
 * precomputed and stored in the Wall class.
 */
double Wall::get_distance_with_point(const Point &point) const {
  // Calculate the coefficients of the line equation
  double a = y2_ - y1_;              // slope of the line
  double b = x1_ - x2_;              // negative of the slope of the line
  double c = x2_ * y1_ - x1_ * y2_;  // y-intercept of the line

  // Calculate the denominator of the distance formula
  double denominator = std::sqrt(a * a + b * b);

  // Calculate the distance from the point to the wall
  double distance = std::abs(a * point.x() + b * point.y() + c) / denominator;

  return distance;
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

bool Wall::operator!=(const Wall &wall) const { return !(*this == wall); }

/**
 * @brief Hash function for Wall objects.
 *
 * @param wall The Wall object to be hashed.
 *
 * @return The hash value of the Wall object.
 *
 * This function overloads the operator() function to provide a hash function
 * for Wall objects. It combines the hash values of the x-coordinates,
 * y-coordinates, and z-coordinates of the Wall object using the bitwise XOR
 * operator.
 */
std::size_t WallHash::operator()(const Wall &wall) const {
  // Combine the hash values of the x-coordinates, y-coordinates, and
  // z-coordinates of the Wall object using the bitwise XOR operator
  return std::hash<double>()(wall.x1())     // x1 coordinate
         ^ std::hash<double>()(wall.y1())   // y1 coordinate
         ^ std::hash<double>()(wall.x2())   // x2 coordinate
         ^ std::hash<double>()(wall.y2());  // y2 coordinate
}

/**
 * @brief Casts rays from a given point in a field and returns the nearest
 * walls that the rays intersect with.
 *
 * @param initial_walls The initial walls in the field
 * @param casting_point The point from which to cast the rays
 *
 * @return An unordered set of walls that the rays intersect with
 *
 * This function casts rays from a given point in a field and returns the
 * nearest walls that the rays intersect with. It generates rays from the
 * casting point and checks for intersections with the initial walls. The
 * function uses the ray-casting algorithm to find the walls that are
 * closest to the rays.
 */
std::unordered_set<Wall, WallHash> ray_casting(
    const std::vector<Wall> &initial_walls, const Point &casting_point) {
  // Define type alias for Ray
  using Ray = Wall;

  // Generate rays from the casting point
  auto ray_generator = [&, phi = 0]() mutable {
    int old_phi = phi++;
    return Ray(casting_point.x(), casting_point.y(),
               casting_point.x() + FIELD_END_BOUNDARY * std::cos(old_phi),
               casting_point.y() + FIELD_END_BOUNDARY * std::sin(old_phi));
  };
  std::vector<Ray> rays(RAY_COUNT);
  std::generate(rays.begin(), rays.end(), ray_generator);

  // Store the potential walls and their intersections with the rays
  std::unordered_map<Point, Wall, PointHash> potential_walls;
  std::vector<Point> intersections;
  std::unordered_set<Wall, WallHash> polygon_walls;

  // Function to compare points by their distance from the casting point
  auto compare_points_by_dist = [&](const Point &p1, const Point &p2) {
    return p1.get_distance_with_point(casting_point) <
           p2.get_distance_with_point(casting_point);
  };

  // Function to check if a point is beyond a ray
  auto point_beyond_ray = [](const Point &point, const Ray &ray) {
    double x1, y1, x2, y2;
    std::tie(x1, x2) = (ray.x1() > ray.x2())
                           ? std::make_tuple(ray.x2(), ray.x1())
                           : std::make_tuple(ray.x1(), ray.x2());
    std::tie(y1, y2) = (ray.y1() > ray.y2())
                           ? std::make_tuple(ray.y2(), ray.y1())
                           : std::make_tuple(ray.y1(), ray.y2());

    return point.x() < x1 || point.x() > x2 || point.y() < y1 || point.y() > y2;
  };

  // Iterate over the rays and check for intersections with the initial walls
  for (const auto &ray : rays) {
    for (const auto &wall : initial_walls) {
      auto intersection = Wall::intersection_point(wall, ray);
      if (intersection.out_of_field() || Wall::is_parallel(ray, wall) ||
          point_beyond_ray(intersection, ray)) {
        continue;
      }
      // Store the intersection and the wall that it intersects with
      intersections.push_back(intersection);
      potential_walls[intersections.back()] = wall;
    }

    // Find the closest intersection among the intersections of the current ray
    auto min_intersection = std::min_element(
        intersections.begin(), intersections.end(), compare_points_by_dist);

    if (min_intersection != intersections.end()) {
      // Add the wall that was the closest to the ray
      polygon_walls.insert(potential_walls[*min_intersection]);
    }

    // Clear the potential walls and intersections for the next ray
    potential_walls.clear();
    intersections.clear();
  }
  return polygon_walls;
}

/**
 * Recursively traverses the walls to find the minimum number of walls
 * needed to reach the treasure point.
 *
 * @param walls The vector of walls to traverse.
 * @param entering_wall The wall that led to the current position.
 * @param treasure_point The point where the treasure is located.
 * @param minimal_number_of_walls The minimum number of walls needed to reach
 * the treasure point.
 * @param number_of_walls The current number of walls encountered.
 */
void recursive_wall_traverse(std::vector<Wall> &walls, Wall entering_wall,
                             const Point &treasure_point,
                             std::size_t &minimal_number_of_walls,
                             std::size_t number_of_walls = 0) {
  // If the number of walls encountered is greater than the current minimum,
  // stop the recursion
  if (number_of_walls > minimal_number_of_walls) {
    return;
  }

  // Define the epsilon value for floating point comparisons
  double epsilon = 1e-8;

  // Define the external walls of the field
  const std::vector<Wall> external_walls = {
      Wall(FIELD_START_BOUNDARY, FIELD_START_BOUNDARY, FIELD_END_BOUNDARY,
           FIELD_START_BOUNDARY),
      Wall(FIELD_START_BOUNDARY, FIELD_START_BOUNDARY, FIELD_START_BOUNDARY,
           FIELD_END_BOUNDARY),
      Wall(FIELD_START_BOUNDARY, FIELD_END_BOUNDARY, FIELD_END_BOUNDARY,
           FIELD_END_BOUNDARY),
      Wall(FIELD_END_BOUNDARY, FIELD_START_BOUNDARY, FIELD_END_BOUNDARY,
           FIELD_END_BOUNDARY)};

  // Perform ray casting to find the walls that limit the treasure point
  auto polygon_walls = ray_casting(walls, treasure_point);

  // Check if the treasure point is outside the field
  for (const auto &wall : external_walls) {
    if (polygon_walls.find(wall) != polygon_walls.end()) {
      number_of_walls++;
      minimal_number_of_walls =
          std::min(minimal_number_of_walls, number_of_walls);
      return;
    }
  }

  // Find all possible intersections between the walls that limit the point
  std::unordered_map<Wall, std::unordered_set<Point, PointHash>, WallHash>
      wall_to_point_intersections;
  for (auto wall1 : polygon_walls) {
    for (auto wall2 : polygon_walls) {
      if (wall1 == wall2 || Wall::is_parallel(wall1, wall2)) {
        continue;
      }
      auto intersection = Wall::intersection_point(wall1, wall2);
      if (intersection.out_of_field()) {
        continue;
      }
      wall_to_point_intersections[wall1].insert(intersection);
      wall_to_point_intersections[wall2].insert(intersection);
    }
  }

  // Find the next valid move by checking if the center of the wall is inside
  // the intersection limits
  for (const auto &wall_intersections : wall_to_point_intersections) {
    auto wall = wall_intersections.first;
    if (wall == entering_wall && wall != Wall(-1, -1, -1, -1)) {
      continue;
    }
    auto intersections = wall_intersections.second;
    std::vector<Point> bounds;

    for (const auto &intersection : intersections) {
      auto min_x = std::min(intersection.x(), treasure_point.x());
      auto max_x = std::max(intersection.x(), treasure_point.x());
      auto min_y = std::min(intersection.y(), treasure_point.y());
      auto max_y = std::max(intersection.y(), treasure_point.y());
      bool bound_found = true;

      Wall temp_wall(treasure_point.x(), treasure_point.y(), intersection.x(),
                     intersection.y());
      for (const auto &checking_wall : polygon_walls) {
        if (checking_wall == wall || checking_wall == entering_wall) {
          continue;
        }
        auto intersection_point =
            Wall::intersection_point(temp_wall, checking_wall);

        // Check if the intersection point is inside the bounds of the walls
        bool inside_bound_x = intersection_point.x() - min_x >= epsilon &&
                              max_x - intersection_point.x() >= epsilon;
        bool inside_bound_y = intersection_point.y() - min_y >= epsilon &&
                              max_y - intersection_point.y() >= epsilon;
        if (inside_bound_x && inside_bound_y) {
          bound_found = false;
          break;
        }
      }

      if (bound_found) {
        bounds.push_back(intersection);
      }
    }

    if (bounds.empty()) {
      continue;
    }

    // Find the bounds of the intersection limits
    auto comp = [](const Point &p1, const Point &p2) {
      return p1.get_distance_with_point(Point(0, 0)) <
             p2.get_distance_with_point(Point(0, 0));
    };
    auto first_bound = *std::min_element(bounds.begin(), bounds.end(), comp);
    auto second_bound = *std::max_element(bounds.begin(), bounds.end(), comp);

    // Check if the center of the wall is inside the bounds
    auto center = wall.get_center();
    if (center.x() <= second_bound.x() && center.x() >= first_bound.x() &&
        center.y() <= second_bound.y() && center.y() >= first_bound.y()) {
      double dx = (center.x() - treasure_point.x());
      double dy = (center.y() - treasure_point.y());
      double dist = std::sqrt(dx * dx + dy * dy);
      dx /= (dist);
      dy /= (dist);
      dx /= (dist);
      dy /= (dist);

      Point new_treasure_point = Point(center.x() + dx, center.y() + dy);
      recursive_wall_traverse(walls, wall, new_treasure_point,
                              minimal_number_of_walls, number_of_walls + 1);
    }
  }
}

/**
 * @brief      Calculates the number of doors in a field.
 *
 * @param[in]  initial_walls   The initial walls in the field
 * @param[in]  initial_treasure_point   The initial treasure point
 *
 * @return     The number of doors in the field
 *
 * The algorithm recursively traverses the walls to find the minimum number of
 * walls needed to reach the treasure point.
 */
std::size_t calc_number_of_doors(const std::vector<Wall> &initial_walls,
                                 const Point &initial_treasure_point) {
  // Copy the initial treasure point to avoid modifying the original one
  Point treasure_point = initial_treasure_point;

  // Initialize the minimal number of walls to the maximum possible value
  std::size_t minimal_number_of_walls = MAXIMUM_WALLS;

  // Recursively traverse the walls to find the minimum number of doors
  recursive_wall_traverse(
      const_cast<std::vector<Wall> &>(
          initial_walls),    // Cast away constness to avoid duplicating code
      Wall(-1, -1, -1, -1),  // Dummy wall to initiate recursion
      treasure_point, minimal_number_of_walls, 0);  // Start from the first wall

  // Return the minimal number of doors
  return minimal_number_of_walls;
}

/**
 * @brief      Handles the treasure hunt based on the provided input.
 *
 * @param[in]  input   The input stream containing the field information and
 *                    the treasure point.
 *
 * @return     The solution string containing the number of doors to the
 * treasure.
 *
 * This function reads the input from the input stream, extracts the number of
 * walls and their coordinates, and the treasure point coordinates. It then
 * calculates the number of doors needed to reach the treasure using the
 * calc_number_of_doors function. If the number of doors is greater than or
 * equal to the maximum number of walls, it returns a string indicating that it
 * is impossible to reach the treasure. Otherwise, it returns a string
 * containing the number of doors.
 */
std::string handle_treasure_hunt(std::istream &input) {
  std::stringstream result;

  // Read the number of walls from the input stream
  std::size_t number_of_walls;
  input >> number_of_walls;

  // Create a vector to store the walls
  std::vector<Wall> walls(number_of_walls + NUMBER_OF_OUTER_WALLS);

  // Initialize the outer walls of the field
  walls[0] = Wall(FIELD_START_BOUNDARY, FIELD_START_BOUNDARY,
                  FIELD_END_BOUNDARY, FIELD_START_BOUNDARY);
  walls[1] = Wall(FIELD_START_BOUNDARY, FIELD_START_BOUNDARY,
                  FIELD_START_BOUNDARY, FIELD_END_BOUNDARY);
  walls[2] = Wall(FIELD_START_BOUNDARY, FIELD_END_BOUNDARY, FIELD_END_BOUNDARY,
                  FIELD_END_BOUNDARY);
  walls[3] = Wall(FIELD_END_BOUNDARY, FIELD_START_BOUNDARY, FIELD_END_BOUNDARY,
                  FIELD_END_BOUNDARY);

  // Read the coordinates of the walls from the input stream
  for (std::size_t i = NUMBER_OF_OUTER_WALLS;
       i < number_of_walls + NUMBER_OF_OUTER_WALLS; i++) {
    double x1, y1, x2, y2;
    input >> x1 >> y1 >> x2 >> y2;
    walls[i] = Wall(x1, y1, x2, y2);
  }

  // Read the coordinates of the treasure point from the input stream
  double treasure_x, treasure_y;
  input >> treasure_x >> treasure_y;

  // Calculate the number of doors needed to reach the treasure
  auto number_of_doors =
      calc_number_of_doors(walls, Point(treasure_x, treasure_y));

  // Check if the number of doors is possible
  if (number_of_doors >= MAXIMUM_WALLS) {
    result << "Impossible to get to the given treasure point" << std::endl;
  } else {
    result << "Number of doors = " << number_of_doors << std::endl;
  }

  return result.str();
}

}  // namespace Treasure_Hunt