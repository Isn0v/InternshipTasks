#include "Pizza.hpp"

// #define DEBUG_MODE

#include <algorithm>
#include <cmath>
#include <functional>
#include <limits>
#include <sstream>
#include <stack>

namespace Pizza {

Point::Point(std::size_t x, std::size_t y) : x_(x), y_(y) {}

Point::Point(const Point &point) : x_(point.x_), y_(point.y_) {}

Point &Point::operator=(const Point &point) {
  if (this != &point) {
    x_ = point.x_;
    y_ = point.y_;
  }
  return *this;
}

std::ostream &operator<<(std::ostream &os, const Point &wall) {
  os << "(" << wall.x_ << "," << wall.y_ << ")";
  return os;
}

bool Point::operator==(const Point &point) const {
  return x_ == point.x_ && y_ == point.y_;
}

bool Point::operator!=(const Point &point) const { return !(*this == point); }

/**
 * @brief Calculates the Euclidean distance between two points.
 *
 * @param point1 The first point.
 * @param point2 The second point.
 * @return The Euclidean distance between the two points.
 *
 * This function calculates the Euclidean distance between two points. It
 * calculates the absolute differences in x and y coordinates, squares them,
 * sums them up, and then takes the square root of the sum.
 */
double Point::get_distance_with_point(const Point &point1,
                                      const Point &point2) {
  // Calculate the absolute difference in x coordinates
  int d_x = point1.x() - point2.x();

  // Calculate the absolute difference in y coordinates
  int d_y = point1.y() - point2.y();

  // Calculate the Euclidean distance
  return std::sqrt(d_x * d_x + d_y * d_y);
}

std::size_t Point::x() const { return x_; }

std::size_t Point::y() const { return y_; }

/**
 * @brief Hash function for Point objects.
 *
 * This function calculates the hash value of a Point object by combining the
 * hash values of the x and y coordinates. It uses the bitwise XOR operator to
 * combine the hash values of the coordinates.
 *
 * @param point The Point object to be hashed.
 *
 * @return The hash value of the Point object.
 */
std::size_t PointHash::operator()(const Point &point) const {
  // Get the hash value of the x coordinate using std::hash<std::size_t>
  std::size_t hash_x = std::hash<std::size_t>()(point.x());

  // Get the hash value of the y coordinate using std::hash<std::size_t>
  std::size_t hash_y = std::hash<std::size_t>()(point.y());

  // Combine the hash values of the x and y coordinates using the bitwise XOR
  // operator
  return hash_x ^ hash_y;
}

/**
 * @brief Finds all the empty points in the city field.
 *
 * This function iterates through each element in the city field and checks if
 * it is empty (i.e., has a value of 0). If an empty point is found, it is added
 * to the vector of empty points.
 *
 * @return A vector of Point objects representing the empty points in the city
 * field.
 */
std::vector<Point> Pizza_City::find_empty_points() {
  // Vector to store the empty points
  std::vector<Point> empty_points;

  // Iterate through each element in the city field
  for (std::size_t i = 0; i < field_height_; i++) {
    for (std::size_t j = 0; j < field_width_; j++) {
      // Check if the current element is empty
      if (city_field_[i][j] == 0) {
        // Add the empty point to the vector
        empty_points.push_back({j, i});
      }
    }
  }

  // Return the vector of empty points
  return empty_points;
}

/**
 * @brief Returns a vector of points that are potential expansion moves for the
 * given pizzeria.
 *
 * This function first checks if the given pizzeria is inside the city bounds.
 * If it is not, it throws an out_of_range exception.
 *
 * It then calls the get_potential_expansion_moves() function to get a vector of
 * potential expansion points.
 *
 * Finally, it filters the potential expansion points to only include those
 * that are not occupied by any pizzeria (i.e., have a pizzeria ID of 0).
 *
 * @param pizzeria The pizzeria for which to get the potential expansion moves.
 * @return A vector of Point objects representing the potential expansion moves
 * for the given pizzeria.
 * @throws std::out_of_range If the given pizzeria is outside the city bounds.
 */
std::vector<Point> Pizza_City::get_expansion_moves(
    const Point &pizzeria) const {
  // Check if the given pizzeria is inside the city bounds
  if (!point_inside_city(pizzeria)) {
    throw std::out_of_range("Method get_expansion_moves: Point out of range");
  }

  // Get a vector of potential expansion points
  auto potential_expansion = get_potential_expansion_moves(pizzeria);

  // Filter the potential expansion points to only include those that are not
  // occupied by any pizzeria (i.e., have a pizzeria ID of 0)
  std::vector<Point> expansion;
  std::copy_if(potential_expansion.begin(), potential_expansion.end(),
               std::back_inserter(expansion), [&](const Point &point) {
                 return get_pizzeria_id_by_point(point) == 0;
               });

  // Return the vector of expansion moves
  return expansion;
}

/**
 * @brief Returns a vector of points that are potential expansion moves for the
 * given pizzeria.
 *
 * This function first checks if the given pizzeria is inside the city bounds.
 * If it is not, it throws an out_of_range exception.
 *
 * It then iterates over the four cardinal directions (right, up, left, down)
 * and for each direction, it traverses the city field in that direction from
 * the given pizzeria. For each traversed point, it checks if it is occupied
 * by a different pizzeria or if it is the same as the given pizzeria. If it is
 * neither of those, it is considered a potential expansion move and is added
 * to the vector of potential expansion moves.
 *
 * @param pizzeria The pizzeria for which to get the potential expansion moves.
 * @return A vector of Point objects representing the potential expansion moves
 * for the given pizzeria.
 * @throws std::out_of_range If the given pizzeria is outside the city bounds.
 */
std::vector<Point> Pizza_City::get_potential_expansion_moves(
    const Point &pizzeria) const {
  // Check if the given pizzeria is inside the city bounds
  if (!point_inside_city(pizzeria)) {
    throw std::out_of_range(
        "Method get_potential_expansion_moves: Point out "
        "of range");
  }

  std::size_t pizzeria_id = get_pizzeria_id_by_point(pizzeria);
  std::vector<Point> potential_expansion_moves;

  /**
   * @brief Traverses the city field in a given direction from the given
   * pizzeria and checks if each traversed point is a potential expansion move.
   *
   * @param dx The x-coordinate change for each step in the traversal.
   * @param dy The y-coordinate change for each step in the traversal.
   */
  auto pizza_point_traverse = [&](int dx, int dy) {
    std::size_t j = 0;
    while (1) {
      j++;
      Point point = Point(pizzeria.x() + dx * j, pizzeria.y() + dy * j);
      // Check if the point is inside the city
      if (!point_inside_city(point)) {
        break;
      }
      // Get the pizzeria ID of the current point
      std::size_t current_id = get_pizzeria_id_by_point(point);
      // Check if the point is occupied by a different pizzeria or if it is the
      // same as the given pizzeria
      if (current_id != pizzeria_id &&
          pizza_data_[current_id - 1].first != point) {
        potential_expansion_moves.push_back(point);
        break;
      }
    }
  };

  // Traverse the city field in each of the four cardinal directions
  pizza_point_traverse(1, 0);   // Right
  pizza_point_traverse(0, 1);   // Up
  pizza_point_traverse(-1, 0);  // Left
  pizza_point_traverse(0, -1);  // Down

  return potential_expansion_moves;
}

/**
 * @brief Fixes an empty point by expanding it with a pizzeria.
 *
 * @param point The point to be fixed.
 * @return True if a pizzeria was found to expand the point, false otherwise.
 */
bool Pizza_City::fix_empty_point(const Point &point) {
  // Set to keep track of visited points
  std::unordered_set<Point, PointHash> visited_points;

  // Depth-First Search function
  std::function<bool(Point)> dfs = [&](const Point &traverse_point) -> bool {
    // Directions to traverse the city field
    std::vector<std::pair<int, int>> d_xy = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    // Traverse each direction
    for (auto d : d_xy) {
      // Calculate the neighbor point
      Point neighbor = {traverse_point.x() + d.first,
                        traverse_point.y() + d.second};

      // Check if the point is inside the city and has not been visited
      if (!point_inside_city(neighbor) || visited_points.count(neighbor)) {
        continue;
      }

      // Get the pizzeria ID of the neighbor point
      auto neighbor_id = get_pizzeria_id_by_point(neighbor);

      // Check if the neighbor point is reachable by the pizzeria
      if (neighbor_id == 0 ||
          !is_point_reachable_to_pizzeria(traverse_point, neighbor_id)) {
        continue;
      } else if (is_pizzeria_filled(neighbor_id)) {
        // Get the edge expansion points of the pizzeria
        auto edge_points = get_edge_expansion_points(neighbor_id);

        // Traverse each edge point
        for (auto edge_point : edge_points) {
          // Check if the edge point is untouchable or has been visited
          if (point_is_untouchable(edge_point) ||
              visited_points.count(edge_point)) {
            continue;
          }

          // Reduce by the edge point and expand by the traverse point
          reduce_by_point(edge_point);
          expand_by_point(traverse_point, neighbor_id);

#ifdef DEBUG_MODE
          // Print the city if in debug mode
          std::cout << "City while fixing empty points:" << std::endl;
          std::cout << "Empty point: " << traverse_point << std::endl;
          std::cout << "Pizzeria chosen: " << neighbor_id << std::endl;
          for (std::size_t y = field_height_ - 1; y >= 0 && y < field_height_;
               --y) {
            for (std::size_t x = 0; x < field_width_; x++) {
              std::cout << city_field_[y][x] << " ";
            }
            std::cout << std::endl;
          }
          std::cout << std::endl;
#endif

          // Mark the edge point as visited
          visited_points.insert(edge_point);

          // Recursively search for a solution
          if (dfs(edge_point)) {
            return true;
          } else {
            // If no solution is found, reverse the changes
            reduce_by_point(traverse_point);
            expand_by_point(edge_point, neighbor_id);
          }
        }
      } else if (!is_pizzeria_filled(neighbor_id)) {
        // If the pizzeria is not filled, expand the point
        expand_by_point(traverse_point, neighbor_id);
        return true;
      }
    }
    return false;
  };

  // Start the depth-first search from the given point
  return dfs(point);
}

/**
 * @brief Checks if a point is inside the city boundaries.
 *
 * @param point The point to be checked.
 *
 * @return True if the point is inside the city boundaries, false otherwise.
 *
 * This function checks if the x-coordinate and y-coordinate of the point
 * are within the boundaries of the city (field_width_ and field_height_
 * respectively). If either of the coordinates is outside the boundaries,
 * the function returns false.
 */
bool Pizza_City::point_inside_city(const Point &point) const {
  // Check if the x-coordinate is within the boundaries of the field
  if (point.x() < 0 || point.x() >= field_width_) {
    return false;
  }

  // Check if the y-coordinate is within the boundaries of the field
  if (point.y() < 0 || point.y() >= field_height_) {
    return false;
  }

  // If both coordinates are within the boundaries, the point is inside the
  // city boundaries
  return true;
}

/**
 * @brief Checks if a point is occupied by a pizzeria.
 *
 * @param point The point to be checked.
 *
 * @return True if the point is occupied by a pizzeria, false otherwise.
 *
 * @throws std::out_of_range If the point is outside the city boundaries.
 *
 * This function checks if a point is occupied by a pizzeria. It first checks if
 * the point is inside the city boundaries by calling the point_inside_city()
 * function. If the point is not inside the city boundaries, it throws an
 * std::out_of_range exception with an appropriate error message.
 *
 * If the point is inside the city boundaries, the function calls the
 * get_pizzeria_id_by_point() function to get the pizzeria ID associated with
 * the point. If the pizzeria ID is not zero (indicating an unoccupied
 * pizzeria), the function returns true, indicating that the point is occupied.
 * Otherwise, it returns false.
 */
bool Pizza_City::point_is_occupied(const Point &point) const {
  // Check if the point is inside the city boundaries
  if (!point_inside_city(point)) {
    // If the point is outside the city boundaries, throw an exception
    throw std::out_of_range("Method point_is_occupied: Point out of range");
  }

  // Get the pizzeria ID associated with the point
  std::size_t pizzeria_id = get_pizzeria_id_by_point(point);

  // Check if the pizzeria ID is not zero (indicating an unoccupied pizzeria)
  return pizzeria_id != 0;
}

/**
 * @brief Checks if a point is untouchable.
 *
 * @param point The point to be checked.
 *
 * @return True if the point is untouchable, false otherwise.
 *
 * @throws std::out_of_range If the point is outside the city boundaries.
 *
 * This function checks if a point is untouchable. It first checks if the point
 * is inside the city boundaries by calling the point_inside_city() function. If
 * the point is not inside the city boundaries, it throws an std::out_of_range
 * exception with an appropriate error message.
 *
 * If the point is inside the city boundaries, the function checks the
 * untouchable_points matrix to determine if the point is untouchable. It
 * returns the value at the corresponding coordinates in the matrix.
 */
bool Pizza_City::point_is_untouchable(const Point &point) const {
  // Check if the point is inside the city boundaries
  if (!point_inside_city(point)) {
    // If the point is outside the city boundaries, throw an exception
    throw std::out_of_range("Method point_is_untouchable: Point out of range");
  }

  // Check if the point is untouchable
  return untouchable_points[point.y()][point.x()];
}

/**
 * @brief Checks if a pizzeria is filled.
 *
 * @param pizzeria_id The ID of the pizzeria.
 *
 * @return True if the pizzeria is filled, false otherwise.
 *
 * This function checks if a pizzeria is filled by comparing the amount of pizza
 * sold by the pizzeria with the amount of pizza it has expanded to. It returns
 * true if the two quantities are equal, indicating that the pizzeria is fully
 * occupied.
 */
bool Pizza_City::is_pizzeria_filled(std::size_t pizzeria_id) const {
  // Get the data for the pizzeria with the given ID
  const auto &pizzeria_data = pizza_data_[pizzeria_id - 1];

  // Get the amount of pizza that the pizzeria has expanded to
  std::size_t expanded_pizza_amount = currently_expanded_[pizzeria_id - 1];

  // Check if the pizzeria is filled by comparing the amounts of pizza sold and
  // expanded
  return pizzeria_data.second == expanded_pizza_amount;
}

/**
 * @brief Calculates the distance to the nearest busy point.
 *
 * @param current_point The current point.
 * @param pizzeria_id The ID of the pizzeria.
 *
 * @return The distance to the nearest busy point. If there are no busy points,
 * returns the maximum value of double.
 *
 * This function calculates the distance to the nearest busy point by traversing
 * the city field in a spiral pattern. It starts from the current point and
 * iteratively checks the surrounding points in a spiral manner. If a busy point
 * is found that is not occupied by the same pizzeria and is not filled, it
 * calculates the distance between the current point and the busy point and
 * updates the minimum distance if necessary. The function stops traversing when
 * all points in the current radius are out of range or a busy point is found.
 */
double Pizza_City::get_distance_to_nearest_busy_point(
    const Point &current_point, std::size_t pizzeria_id) {
  // Check if the current point is inside the city boundaries
  if (!point_inside_city(current_point)) {
    // If the current point is outside the city boundaries, throw an exception
    throw std::out_of_range(
        "Method get_distance_to_nearest_busy_point: Point out of range");
  }

  double min_distance = std::numeric_limits<double>::max();
  std::size_t traverse_radius = 0, side_number = 3;
  Point starting_point = current_point, traverse_point;
  bool busy_point_found = false;

  // Define the x and y deltas for traversing the city field in a spiral pattern
  std::vector<std::pair<int, int>> d_xy{{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
  int dx, dy;

  // Start traversing the city field in a spiral pattern
  while (1) {
    bool all_traverse_points_out_of_range = true;

    // Traverse the points in a spiral manner
    for (std::size_t j = 0; j < 8 * (traverse_radius + 1); ++j) {
      if (j % ((traverse_radius + 1) * 2) == 0) {
        // Update the side number and deltas for the next side of the spiral
        side_number = (side_number + 1) % 4;
        std::tie(dx, dy) = d_xy[side_number];
        // Update the starting point for the next side of the spiral
        if (side_number == 0) {
          traverse_point = {starting_point.x() - 1, starting_point.y() - 1};
          starting_point = traverse_point;
        }
      }

      // Check if the traverse point is inside the city boundaries
      if (!point_inside_city(traverse_point)) {
        // If the traverse point is outside the city boundaries, move to the
        // next point in the spiral
        traverse_point = {traverse_point.x() + dx, traverse_point.y() + dy};
        continue;
      } else if (all_traverse_points_out_of_range) {
        // If the traverse point is inside the city boundaries and none of the
        // previous points were out of range, set the flag to false
        all_traverse_points_out_of_range = false;
      }

      // Get the ID of the pizzeria at the traverse point
      std::size_t traverse_id = get_pizzeria_id_by_point(traverse_point);

      // Check if the traverse point is inside the city boundaries and not
      // occupied by the same pizzeria and is not filled
      if (traverse_id == 0) {
        // If the traverse point is outside the city boundaries, move to the
        // next point in the spiral
        traverse_point = {traverse_point.x() + dx, traverse_point.y() + dy};
        continue;
      } else if (traverse_id != pizzeria_id &&
                 !is_pizzeria_filled(traverse_id)) {
        // Calculate the distance between the current point and the busy point
        int distance_x = traverse_point.x() - current_point.x();
        int distance_y = traverse_point.y() - current_point.y();
        double distance =
            std::sqrt(std::pow(distance_x, 2) + std::pow(distance_y, 2));

        // Update the minimum distance if necessary
        min_distance = std::min(min_distance, distance);
        busy_point_found = true;
      }

      // Move to the next point in the spiral
      traverse_point = {traverse_point.x() + dx, traverse_point.y() + dy};
    }

    // If all points in the current radius are out of range or a busy point is
    // found, stop traversing
    if (all_traverse_points_out_of_range || busy_point_found) {
      break;
    }

    // Increase the radius of the spiral for the next iteration
    traverse_radius++;
  }

  // Return the minimum distance to the nearest busy point
  return min_distance;
}

/**
 * @brief Checks if a given point is reachable to other pizzerias from a given
 * pizzeria.
 *
 * @param point The point to check.
 * @param pizzeria_id The ID of the pizzeria.
 *
 * @return True if the point is reachable to other pizzerias, false otherwise.
 *
 * This function checks if a given point is reachable to other pizzerias from a
 * given pizzeria by traversing the city field in four directions (up, down,
 * left, and right) and checking if any of the adjacent points are occupied by
 * other pizzerias. The function uses a lambda function to encapsulate the
 * traversal logic.
 */
bool Pizza_City::is_pizzeria_point_reachable_to_other_pizzerias(
    const Point &point, std::size_t pizzeria_id) const {
  // Lambda function to traverse the city field in a given direction
  auto side_traverse = [&](int dx, int dy) {
    std::size_t j = 0;  // Traversal counter
    bool is_first_encountered_busy_point =
        true;  // Flag to track if the first encountered busy point is found
    std::size_t
        last_encountered_busy_id;  // ID of the last encountered busy point
    std::size_t counter_encountered_on_line =
        0;  // Counter to track the number of encountered points on the same
            // line

    // Traverse the city field in the given direction
    while (1) {
      j++;
      Point point_to_check = {point.x() + dx * j, point.y() + dy * j};

      // Check if the traverse point is inside the city boundaries
      if (!point_inside_city(point_to_check)) break;

      // Get the ID of the pizzeria at the traverse point
      std::size_t current_id = get_pizzeria_id_by_point(point_to_check);

      // Check if the traverse point is outside the city boundaries or occupied
      // by the same pizzeria
      if (current_id == 0) {
        continue;
      } else if (current_id == pizzeria_id) {
        break;
      } else {
        // If the first encountered busy point is not found, update the last
        // encountered busy point
        if (is_first_encountered_busy_point) {
          last_encountered_busy_id = current_id;
          is_first_encountered_busy_point = false;
        }

        // Calculate the capacity available for the current pizzeria
        std::size_t capacity_available = pizza_data_[current_id - 1].second -
                                         currently_expanded_[current_id - 1];

        // Check if the current pizzeria is different from the last encountered
        // busy point
        if (current_id != last_encountered_busy_id) {
          break;
        }

        // Check if the current point is different from the pizzeria's expansion
        // point
        if (pizza_data_[current_id - 1].first != point_to_check) {
          counter_encountered_on_line++;
        } else {
          // Check if there is enough capacity available to expand to the
          // current point
          if (j <= capacity_available + counter_encountered_on_line &&
              !is_pizzeria_filled(current_id)) {
            return true;
          }
        }
      }
    }
    return false;
  };

  // Traverse the city field in four directions (up, down, left, and right)
  auto top = side_traverse(0, 1);
  auto bottom = side_traverse(0, -1);
  auto left = side_traverse(-1, 0);
  auto right = side_traverse(1, 0);

  // Return true if any of the adjacent points are reachable to other pizzerias
  return top || bottom || left || right;
}

/**
 * @brief Checks if a point is reachable to a pizzeria.
 *
 * @param point The point to check.
 * @param pizzeria_id The ID of the pizzeria.
 * @return True if the point is reachable to the pizzeria, false otherwise.
 *
 * This function checks if a point is reachable to a pizzeria by iterating over
 * the edge expansion points of the pizzeria and checking if any of the points
 * is equal to the given point.
 */
bool Pizza_City::is_point_reachable_to_pizzeria(const Point &point,
                                                std::size_t pizzeria_id) const {
  // Get the edge expansion points of the pizzeria
  auto edge_points = get_edge_expansion_points(pizzeria_id);

  // Define the x and y deltas for traversing the edge points
  std::vector<std::pair<int, int>> d_xy = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
  int d_x, d_y;

  // Traverse the edge points and check if any of them is equal to the given
  // point
  for (std::size_t i = 0; i < d_xy.size(); i++) {
    std::tie(d_x, d_y) = d_xy[i];
    Point next_point = {edge_points[i].x() + d_x, edge_points[i].y() + d_y};
    if (next_point == point) {
      return true;
    }
  }

  // If none of the edge points is equal to the given point, return false
  return false;
}

/**
 * @brief Expands unreachable points for a given pizzeria.
 *
 * @param pizzeria_id The ID of the pizzeria.
 *
 * This function expands unreachable points for a given pizzeria. It iterates
 * over the four possible directions and checks if points in those directions
 * are reachable to other pizzerias. If a point is not reachable, it expands the
 * pizzeria to that point. The expansion is limited to the capacity of the
 * pizzeria.
 */
void Pizza_City::expand_unreachable_points(std::size_t pizzeria_id) {
  // Get the pizzeria and its capacity
  pizza_data_t pizzeria_and_capacity = pizza_data_[pizzeria_id - 1];

  // Define the x and y deltas for traversing the city field
  std::vector<std::pair<int, int>> d_xy = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
  int d_x, d_y;

  // Traverse the four possible directions
  for (std::size_t i = 0;
       i < d_xy.size() &&
       currently_expanded_[pizzeria_id - 1] < pizzeria_and_capacity.second;
       i++) {
    std::size_t start_of_expansion = 0, expansion_radius = 0, j = 0;
    std::tie(d_x, d_y) = d_xy[i];

    // Traverse the points in the current direction and check for expansion
    while (j < pizzeria_and_capacity.second -
                   currently_expanded_[pizzeria_id - 1]) {
      j++;
      Point expansion_point = {pizzeria_and_capacity.first.x() + j * d_x,
                               pizzeria_and_capacity.first.y() + j * d_y};

      // Check if the point is inside the city boundaries
      if (!point_inside_city(expansion_point)) {
        break;
      }

      // Get the pizzeria ID of the point
      std::size_t current_id = get_pizzeria_id_by_point(expansion_point);
      if (current_id == pizzeria_id) {
        // If the point is occupied by the same pizzeria, update the start of
        // expansion
        start_of_expansion = j;
        continue;
      }

      // If the point is not reachable to other pizzerias, update the expansion
      // radius
      if (current_id == 0 && !is_pizzeria_point_reachable_to_other_pizzerias(
                                 expansion_point, pizzeria_id)) {
        expansion_radius = j;
      }
    }

    // Expand the pizzeria to the points within the expansion radius
    for (std::size_t j = start_of_expansion + 1; j <= expansion_radius; j++) {
      Point expansion_point = {pizzeria_and_capacity.first.x() + j * d_x,
                               pizzeria_and_capacity.first.y() + j * d_y};
      expand_by_point(expansion_point, pizzeria_id);
      untouchable_points[expansion_point.y()][expansion_point.x()] = true;
    }
  }
}

/**
 * @brief Expands a pizzeria by a given point if the point is not already taken
 * by another pizzeria.
 *
 * @param point The point to expand the pizzeria to.
 * @param pizzeria_id The ID of the pizzeria to expand.
 *
 * @throws std::invalid_argument If the point is already taken by another
 * pizzeria or if the point is not adjacent to the pizzeria.
 */
void Pizza_City::expand_by_point(const Point &point, std::size_t pizzeria_id) {
  // Get the current ID of the point on the city field
  std::size_t current_id = city_field_[point.y()][point.x()];

  // Check if the point is already taken by another pizzeria
  if (current_id != 0 && current_id != pizzeria_id) {
    throw std::invalid_argument(
        "Method expand_by_point: Point is already taken by another pizzeria");
  }
  // If the point is already occupied by the same pizzeria, do nothing and
  // return
  else if (current_id == pizzeria_id) {
    return;
  }

  // Update the city field with the new pizzeria ID
  city_field_[point.y()][point.x()] = pizzeria_id;

  // Get the pizzeria data and calculate the distance to the point
  pizza_data_t pizzeria_and_capacity = pizza_data_[pizzeria_id - 1];
  int dx = point.x() - pizzeria_and_capacity.first.x();
  int dy = point.y() - pizzeria_and_capacity.first.y();

  // Check if the point is not adjacent to the pizzeria
  if (dx != 0 && dy != 0) {
    throw std::invalid_argument(
        "Method expand_by_point: Point is not adjacent to pizzeria");
  }

  // Calculate the increments for the correct expansions
  int top_increment = dy > 0 ? 1 : 0;
  int right_increment = dx > 0 ? 1 : 0;
  int down_increment = dy < 0 ? 1 : 0;
  int left_increment = dx < 0 ? 1 : 0;

  // Update the correct expansions
  correct_expansions[pizzeria_id - 1] = {
      std::get<0>(correct_expansions[pizzeria_id - 1]) + top_increment,
      std::get<1>(correct_expansions[pizzeria_id - 1]) + right_increment,
      std::get<2>(correct_expansions[pizzeria_id - 1]) + down_increment,
      std::get<3>(correct_expansions[pizzeria_id - 1]) + left_increment,
  };

  // Update the number of currently expanded points and busy points count
  currently_expanded_[pizzeria_id - 1]++;
  busy_points_count++;
}

/**
 * @brief Reduces a pizzeria by a given point.
 *
 * @param point The point to reduce the pizzeria from.
 *
 * @throws std::invalid_argument If the point is not occupied by any
 * pizzeria, if the point is not an edge point, or if the point is not
 * adjacent to the pizzeria.
 */
void Pizza_City::reduce_by_point(const Point &point) {
  // Get the ID of the pizzeria occupying the point
  std::size_t pizzeria_id = get_pizzeria_id_by_point(point);

  // If the point is not occupied by any pizzeria, throw an exception
  if (pizzeria_id == 0) {
    throw std::invalid_argument(
        "Method reduce_by_point: Point is not occupied by any pizzeria");
  }

  // Get the list of edge points for the pizzeria
  auto edge_points = get_edge_expansion_points(pizzeria_id);

  // If the point is not an edge point, throw an exception
  if (std::find(edge_points.begin(), edge_points.end(), point) ==
      edge_points.end()) {
    throw std::invalid_argument("Method reduce_by_point: Point is not an edge");
  }

  // Get the pizzeria data and calculate the distance to the point
  pizza_data_t pizzeria_and_capacity = pizza_data_[pizzeria_id - 1];
  int dx = point.x() - pizzeria_and_capacity.first.x();
  int dy = point.y() - pizzeria_and_capacity.first.y();

  // If the point is not adjacent to the pizzeria, throw an exception
  if (dx != 0 && dy != 0) {
    throw std::invalid_argument(
        "Method reduce_by_point: Point is not adjacent to pizzeria");
  }

  // Calculate the decrements for the correct expansions
  int top_decrement = dy > 0 ? 1 : 0;
  int right_decrement = dx > 0 ? 1 : 0;
  int down_decrement = dy < 0 ? 1 : 0;
  int left_decrement = dx < 0 ? 1 : 0;

  // Update the correct expansions
  correct_expansions[pizzeria_id - 1] = {
      std::get<0>(correct_expansions[pizzeria_id - 1]) - top_decrement,
      std::get<1>(correct_expansions[pizzeria_id - 1]) - right_decrement,
      std::get<2>(correct_expansions[pizzeria_id - 1]) - down_decrement,
      std::get<3>(correct_expansions[pizzeria_id - 1]) - left_decrement,
  };

  // Update the city field and the number of currently expanded points and busy
  // points count
  city_field_[point.y()][point.x()] = 0;
  currently_expanded_[pizzeria_id - 1]--;
  busy_points_count--;
}

/**
 * @brief Returns a vector of Points representing the edge expansion points
 * of a pizzeria.
 *
 * @param pizzeria_id The ID of the pizzeria.
 *
 * @return A vector of Points representing the edge expansion points of the
 * pizzeria.
 *
 * @throws None.
 */
std::vector<Point> Pizza_City::get_edge_expansion_points(
    std::size_t pizzeria_id) const {
  // Get the Point of the pizzeria
  Point pizzeria_point = pizza_data_[pizzeria_id - 1].first;

  // Calculate the expansion points based on the correct expansions of the
  // pizzeria
  return std::vector<Point>{
      // Top expansion point
      {pizzeria_point.x(),
       pizzeria_point.y() + std::get<0>(correct_expansions[pizzeria_id - 1])},
      // Right expansion point
      {pizzeria_point.x() + std::get<1>(correct_expansions[pizzeria_id - 1]),
       pizzeria_point.y()},
      // Bottom expansion point
      {pizzeria_point.x(),
       pizzeria_point.y() - std::get<2>(correct_expansions[pizzeria_id - 1])},
      // Left expansion point
      {pizzeria_point.x() - std::get<3>(correct_expansions[pizzeria_id - 1]),
       pizzeria_point.y()},
  };
}

/**
 * @brief Returns the ID of the pizzeria at the given point.
 *
 * @param point The point to get the pizzeria ID of.
 *
 * @return The ID of the pizzeria at the given point.
 *
 * @throws std::out_of_range If the given point is out of city bounds.
 *
 * This function checks if the given point is inside the city boundaries by
 * calling the point_inside_city() function. If the point is not inside the
 * city boundaries, it throws an std::out_of_range exception with an
 * appropriate error message.
 *
 * If the point is inside the city boundaries, it returns the ID of the
 * pizzeria at the given point by accessing the city_field_ array.
 */
std::size_t Pizza_City::get_pizzeria_id_by_point(const Point &point) const {
  // Check if the point is inside the city boundaries
  if (!point_inside_city(point)) {
    // If the point is outside the city boundaries, throw an exception
    throw std::out_of_range(
        "Method get_pizzeria_id_by_point: Point out of range");
  }

  // Return the ID of the pizzeria at the given point
  return city_field_[point.y()][point.x()];
}

/**
 * @brief Iteratively expands the coverage of the city based on the pizza
 *        expansions.
 *
 * This function iteratively expands the coverage of the city based on the
 * pizza expansions. It starts by initializing the correct_expansions vector
 * with {0, 0, 0, 0} for each pizza. It then enters a do-while loop where it
 * checks if any points are being expanded. Within the loop, it iterates over
 * each pizza and expands the unreachable points. If no points are being
 * expanded, it breaks out of the loop. After the loop, it initializes the
 * iterator i to the last pizza. It then enters a while loop where it expands
 * the city by adding points to the pizza with the least capacity. It checks
 * if the pizza has reached its maximum capacity and if so, skips to the next
 * pizza. If there are no more points to expand, it throws a runtime error.
 *
 * @throws std::runtime_error If there are no more points to expand.
 */
void Pizza_City::iterating_coverage() {
  // Enter do-while loop to expand coverage
  bool is_expanding;
  do {
    is_expanding = false;

    // Iterate over each pizza and expand unreachable points
    for (std::size_t i = 0; i < pizza_data_.size(); i++) {
      std::size_t points_changes = busy_points_count;

      // Expand unreachable points for the current pizza
      expand_unreachable_points(i + 1);

      // Check if any points were expanded
      points_changes -= busy_points_count;
      if (points_changes == 0) {
        is_expanding = is_expanding || false;
      } else {
        is_expanding = true;
      }
    }
  } while (is_expanding);

  // Initialize iterator i to the last pizza
  std::size_t i = pizza_data_.size() - 1;

  // Enter while loop to expand the city
  while (busy_points_count <
         field_height_ * field_width_ - pizza_data_.size()) {
    i = (i + 1) % (pizza_data_.size());

    // Get the pizzeria and its capacity
    pizza_data_t pizzeria_and_capacity = pizza_data_[i];
    Point pizza_point = pizzeria_and_capacity.first;
    std::size_t pizza_capacity = pizzeria_and_capacity.second;
    std::size_t pizzeria_id = city_field_[pizza_point.y()][pizza_point.x()];

    // Check if the pizza has reached its maximum capacity
    if (pizza_capacity <= currently_expanded_[pizzeria_id - 1]) {
      continue;
    }

    // Get the expansion moves for the pizza
    std::vector<Point> expansion;
    expansion = get_expansion_moves(pizzeria_and_capacity.first);

    // If no expansion moves are available and the pizza has unexpanded
    // capacity, find empty points and expand them
    if (expansion.size() == 0 &&
        currently_expanded_[pizzeria_id - 1] < pizza_capacity) {
      auto empty_points = find_empty_points();
      if (empty_points.size() == 0) {
        throw std::runtime_error("Error: city can't be expanded");
      }
      for (auto empty_point : empty_points) {
        if (fix_empty_point(empty_point)) {
          break;
        }
      }
      continue;
    }

    // Calculate the distances to the nearest busy point for each expansion
    // move
    std::unordered_map<Point, double, PointHash> distances;
    for (std::size_t j = 0; j < expansion.size(); j++) {
      double distance =
          get_distance_to_nearest_busy_point(expansion[j], pizzeria_id);
      distances[expansion[j]] = distance;
    }

    // Sort the expansion moves based on their distances to the nearest busy
    // point, in descending order
    std::stable_sort(
        expansion.rbegin(), expansion.rend(),
        [&](Point a, Point b) { return distances[a] < distances[b]; });

    // Expand the pizza by adding the point with the least distance to the
    // nearest busy point
    expand_by_point(expansion.back(), pizzeria_id);
  }
}

/**
 * @brief Returns the correct expansions of each pizzeria.
 *
 * The correct expansions of a pizzeria represent the number of points to expand
 * in each of the four directions (north, east, south, west).
 *
 * @return A vector of num_expansion_t representing the correct expansions of
 * each pizzeria.
 */
std::vector<num_expansion_t> Pizza_City::get_correct_expansions() const {
  // Return the correct expansions of each pizzeria
  return correct_expansions;
}

/**
 * @brief Handles a pizza city problem input stream and returns the solution as
 * a string.
 *
 * @param input The input stream to read the problem from.
 *
 * @return The solution as a string.
 *
 * @throws std::runtime_error If the input is invalid.
 */
std::string handle_pizza_city(std::istream &input) {
  // Create a string stream to store the result
  std::stringstream result;

  // Iterate over each case
  std::size_t counter = 1;
  while (counter <= 30) {
    // Read the number of rows and columns in the city
    std::size_t n, m, k;
    input >> n;

    // If the number of rows is 0, break the loop
    if (n == 0) {
      break;
    }

    // Read the number of pizzerias and total capacity
    input >> m >> k;

    // Read the pizzerias and their capacities
    std::vector<pizza_data_t> pizzerias_and_capacities(k);
    std::size_t total_capacity = 0;
    for (std::size_t i = 0; i < k; ++i) {
      std::size_t x, y, c;
      input >> x >> y >> c;
      total_capacity += c;
      pizzerias_and_capacities[i] = {Point(x - 1, y - 1), c};
    }

    // Check if the number of pizzerias and total capacity are consistent
    if (m * n != pizzerias_and_capacities.size() + total_capacity) {
      throw std::runtime_error("Invalid input");
    }

    // Create a Pizza_City object and solve the problem
    Pizza_City pizza_city(m, n, pizzerias_and_capacities);
    result << "Case " << counter << ":\n";

    try {
      pizza_city.iterating_coverage();

      // Write the correct expansions of each pizzeria to the result
      for (auto permutation : pizza_city.get_correct_expansions()) {
        result << std::get<0>(permutation) << " " << std::get<1>(permutation)
               << " " << std::get<2>(permutation) << " "
               << std::get<3>(permutation) << std::endl;
      }
    } catch (std::exception &e) {
      // If no solution is found, write an error message to the result
      result << "No solution: " << e.what() << "" << std::endl;
    }

    // Add a newline between each case
    result << std::endl;
    ++counter;
  }

  // Return the result as a string
  return result.str();
}

/**
 * @brief Constructs a new Pizza_City object.
 *
 * @param field_height The height of the city field.
 * @param field_width The width of the city field.
 * @param pizza_data The data of each pizzeria.
 */
Pizza_City::Pizza_City(std::size_t field_height, std::size_t field_width,
                       const std::vector<pizza_data_t> &pizza_data)
    : field_height_(field_height),
      field_width_(field_width),
      pizza_data_(pizza_data) {
  // Allocate memory for the city field.
  city_field_ = new std::size_t *[field_height_];

  // Initialize each row of the city field with the default value of 0.
  for (std::size_t i = 0; i < field_height_; i++) {
    city_field_[i] = new std::size_t[field_width_];
    for (std::size_t j = 0; j < field_width_; j++) {
      city_field_[i][j] = 0;
    }
  }

  // Initialize the currently expanded vector with 0 for each pizzeria.
  currently_expanded_ = std::vector<std::size_t>(pizza_data_.size(), 0);

  // Initialize the correct expansions vector with {0, 0, 0, 0} for each
  // pizzeria.
  correct_expansions =
      std::vector<num_expansion_t>(pizza_data_.size(), {0, 0, 0, 0});

  // Initialize the untouchable points vector with false for each point.
  untouchable_points = std::vector<std::vector<bool>>(
      field_height_, std::vector<bool>(field_width_, false));

  // Set the initial state of the city field by marking the pizzerias' positions
  // as occupied.
  for (std::size_t i = 0; i < pizza_data_.size(); i++) {
    city_field_[pizza_data_[i].first.y()][pizza_data_[i].first.x()] = i + 1;
    untouchable_points[pizza_data_[i].first.y()][pizza_data_[i].first.x()] = 1;
  }
}

/**
 * @brief Destructor for the Pizza_City class.
 *
 * This destructor frees the dynamically allocated memory for the city field.
 */
Pizza_City::~Pizza_City() {
  // Iterate over each row in the city field.
  for (std::size_t i = 0; i < field_height_; i++) {
    // Delete each row in the city field.
    delete[] city_field_[i];
  }

  // Delete the city field itself.
  delete[] city_field_;
}

}  // namespace Pizza