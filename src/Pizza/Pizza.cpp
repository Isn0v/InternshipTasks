#include "Pizza.hpp"

#define DEBUG_MODE

#include <algorithm>
#include <cmath>
#include <functional>
#include <limits>
#include <sstream>

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

double Point::get_distance_with_point(const Point &point1,
                                      const Point &point2) {
  std::size_t d_x = point1.x() > point2.x() ? point1.x() - point2.x()
                                            : point2.x() - point1.x();
  std::size_t d_y = point1.y() > point2.y() ? point1.y() - point2.y()
                                            : point2.y() - point1.y();
  return std::sqrt(d_x * d_x + d_y * d_y);
}

std::size_t Point::x() const { return x_; }

std::size_t Point::y() const { return y_; }

std::size_t PointHash::operator()(const Point &point) const {
  return std::hash<std::size_t>()(point.x()) ^
         std::hash<std::size_t>()(point.y());
}

std::vector<Point> Pizza_City::find_empty_points() {
  std::vector<Point> empty_points;
  for (std::size_t i = 0; i < field_height_; i++) {
    for (std::size_t j = 0; j < field_width_; j++) {
      if (city_field_[i][j] == 0) {
        empty_points.push_back({j, i});
      }
    }
  }
  return empty_points;
}

std::vector<Point> Pizza_City::get_expansion_moves(
    const Point &pizzeria) const {
  // TODO code duplication
  if (pizzeria.x() < 0 || pizzeria.x() >= field_width_ || pizzeria.y() < 0 ||
      pizzeria.y() >= field_height_) {
    std::stringstream report;
    report << "Pizzeria Point (" << pizzeria.x() << ", " << pizzeria.y()
           << ") is out of range in get_possible_expansion_moves function";
    throw std::out_of_range(report.str());
  }

  std::size_t pizzeria_id = city_field_[pizzeria.y()][pizzeria.x()];

  std::vector<Point> expansion_moves;

  auto pizza_point_traverse = [&](int dx, int dy) {
    std::size_t j = 0;
    bool inside_field_x, inside_field_y;
    while (1) {
      j++;
      inside_field_x = dx < 0 ? pizzeria.x() >= -dx * j
                              : pizzeria.x() + dx * j < field_width_;
      inside_field_y = dy < 0 ? pizzeria.y() >= -dy * j
                              : pizzeria.y() + dy * j < field_height_;
      if (!inside_field_x || !inside_field_y) break;
      std::size_t current_id =
          city_field_[pizzeria.y() + dy * j][pizzeria.x() + dx * j];
      if (current_id == 0) {
        expansion_moves.push_back(
            Point(pizzeria.x() + dx * j, pizzeria.y() + dy * j));
        break;
      } else if (current_id != pizzeria_id && current_id != 0) {
        break;
      }
    }
  };

  pizza_point_traverse(1, 0);
  pizza_point_traverse(0, 1);
  pizza_point_traverse(-1, 0);
  pizza_point_traverse(0, -1);
  return expansion_moves;
}

// TODO code duplication
std::vector<Point> Pizza_City::get_potential_expansion_moves(
    const Point &pizzeria) const {
  if (pizzeria.x() < 0 || pizzeria.x() >= field_width_ || pizzeria.y() < 0 ||
      pizzeria.y() >= field_height_) {
    std::stringstream report;
    report << "Pizzeria Point (" << pizzeria.x() << ", " << pizzeria.y()
           << ") is out of range in get_possible_expansion_moves function";
    throw std::out_of_range(report.str());
  }

  std::size_t pizzeria_id = city_field_[pizzeria.y()][pizzeria.x()];

  std::vector<Point> potential_expansion_moves;

  auto pizza_point_traverse = [&](int dx, int dy) {
    std::size_t j = 0;
    bool inside_field_x, inside_field_y;
    while (1) {
      j++;
      // TODO code duplication
      inside_field_x = dx < 0 ? pizzeria.x() >= -dx * j
                              : pizzeria.x() + dx * j < field_width_;
      inside_field_y = dy < 0 ? pizzeria.y() >= -dy * j
                              : pizzeria.y() + dy * j < field_height_;
      if (!inside_field_x || !inside_field_y) break;
      std::size_t current_id =
          city_field_[pizzeria.y() + dy * j][pizzeria.x() + dx * j];
      if (current_id != pizzeria_id &&
          pizza_data_[current_id - 1].first !=
              Point(pizzeria.x() + dx * j, pizzeria.y() + dy * j)) {
        potential_expansion_moves.push_back(
            Point(pizzeria.x() + dx * j, pizzeria.y() + dy * j));
        break;
      }
    }
  };

  pizza_point_traverse(1, 0);
  pizza_point_traverse(0, 1);
  pizza_point_traverse(-1, 0);
  pizza_point_traverse(0, -1);
  return potential_expansion_moves;
}

bool Pizza_City::fix_expansion(std::size_t pizzeria_id) {
  pizza_data_t pizzeria_and_capacity = pizza_data_[pizzeria_id - 1];
  auto potential_expansion =
      get_potential_expansion_moves(pizzeria_and_capacity.first);
  auto empty_points = find_empty_points();
  Point chosen_empty_point;
  double min_distance = std::numeric_limits<double>::max();
  for (auto empty_point : empty_points) {
    for (auto potential_expansion_point : potential_expansion) {
      double distance = Point::get_distance_with_point(
          potential_expansion_point, empty_point);
      if (distance < min_distance) {
        min_distance = distance;
        chosen_empty_point = empty_point;
      }
    }
  }

  std::sort(potential_expansion.begin(), potential_expansion.end(),
            [&](const Point &p1, const Point &p2) {
              return Point::get_distance_with_point(p1, chosen_empty_point) <
                     Point::get_distance_with_point(p2, chosen_empty_point);
            });

  auto it = std::find_if(
      potential_expansion.begin(), potential_expansion.end(),
      [&](const Point &p) {
        std::size_t current_id = city_field_[p.y()][p.x()];
        return untouchable_points[p.y()][p.x()] == false &&
               fixed_expansions[pizzeria_id - 1][current_id - 1] == false;
      });

  if (potential_expansion.size() == 0 || it == potential_expansion.end()) {
    return false;
  }

  Point next_point = *it, iterable_point_on_line;
  std::size_t next_pizzeria_id = city_field_[next_point.y()][next_point.x()];
  Point next_pizzeria_point = pizza_data_[next_pizzeria_id - 1].first;

  int next_d_x = next_point.x() - next_pizzeria_point.x();
  int next_d_y = next_point.y() - next_pizzeria_point.y();
  if (next_d_x != 0) {
    next_d_x /= std::abs(next_d_x);
  }
  if (next_d_y != 0) {
    next_d_y /= std::abs(next_d_y);
  }

  iterable_point_on_line = {next_point.x() + next_d_x,
                            next_point.y() + next_d_y};

  while (1) {
    if (!point_inside_city(iterable_point_on_line)) {
      break;
    }
    if (city_field_[iterable_point_on_line.y()][iterable_point_on_line.x()] !=
        next_pizzeria_id) {
      break;
    }

    reduce_by_point(iterable_point_on_line);
    iterable_point_on_line = {iterable_point_on_line.x() + next_d_x,
                              iterable_point_on_line.y() + next_d_y};
  }

  fixed_expansions[pizzeria_id - 1][next_pizzeria_id - 1] = true;
  fixed_expansions[next_pizzeria_id - 1][pizzeria_id - 1] = true;

  reduce_by_point(next_point);
  expand_by_point(next_point, pizzeria_id);
  return true;
}

bool Pizza_City::point_inside_city(const Point &point) const {
  if (point.x() < 0 || point.x() >= field_width_ || point.y() < 0 ||
      point.y() >= field_height_) {
    return false;
  }
  return true;
}

bool Pizza_City::point_is_occupied(const Point &point) const {
  if (!point_inside_city(point)) {
    throw std::out_of_range("Method point_is_occupied: Point out of range");
  }
  return city_field_[point.y()][point.x()] != 0;
}

double Pizza_City::get_distance_to_nearest_busy_point(
    const Point &current_point, std::size_t pizzeria_id) {
  if (current_point.x() < 0 || current_point.x() >= field_width_ ||
      current_point.y() < 0 || current_point.y() >= field_height_) {
    std::stringstream report;
    report
        << "Point (" << current_point.x() << ", " << current_point.y()
        << ") is out of range in get_distance_to_nearest_busy_point function";
    throw std::out_of_range(report.str());
  }
  double min_distance = std::numeric_limits<double>::max();
  std::size_t traverse_radius = 0, side_number = 3;
  Point starting_point = current_point, traverse_point;
  bool busy_point_found = false;

  std::vector<std::pair<int, int>> d_xy{{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
  int dx, dy;

  while (1) {
    bool all_traverse_points_out_of_range = true;
    for (std::size_t j = 0; j < 8 * (traverse_radius + 1); ++j) {
      if (j % ((traverse_radius + 1) * 2) == 0) {
        side_number = (side_number + 1) % 4;
        std::tie(dx, dy) = d_xy[side_number];
        if (side_number == 0) {
          traverse_point = {starting_point.x() - 1, starting_point.y() - 1};
          starting_point = traverse_point;
        }
      }
      if (traverse_point.x() < 0 || traverse_point.x() >= field_width_ ||
          // TODO code duplication
          traverse_point.y() < 0 || traverse_point.y() >= field_height_) {
        traverse_point = {traverse_point.x() + dx, traverse_point.y() + dy};
        continue;
      } else if (all_traverse_points_out_of_range) {
        all_traverse_points_out_of_range = false;
      }
      std::size_t traverse_id =
          city_field_[traverse_point.y()][traverse_point.x()];
      if (traverse_id == 0) {
        // TODO code duplication
        traverse_point = {traverse_point.x() + dx, traverse_point.y() + dy};
        continue;
      }

      if (traverse_id != pizzeria_id &&
          pizza_data_[traverse_id - 1].second >
              currently_expanded_[traverse_id - 1]) {
        std::size_t distance_x = traverse_point.x() > current_point.x()
                                     ? traverse_point.x() - current_point.x()
                                     : current_point.x() - traverse_point.x();
        std::size_t distance_y = traverse_point.y() > current_point.y()
                                     ? traverse_point.y() - current_point.y()
                                     : current_point.y() - traverse_point.y();
        double distance =
            std::sqrt(std::pow(distance_x, 2) + std::pow(distance_y, 2));
        min_distance = std::min(min_distance, distance);
        busy_point_found = true;
      }
      traverse_point = {traverse_point.x() + dx, traverse_point.y() + dy};
    }

    if (all_traverse_points_out_of_range || busy_point_found) {
      break;
    }
    traverse_radius++;
  }
  return min_distance;
}

bool Pizza_City::is_pizzeria_point_reachable_to_other_pizzerias(
    const Point &point, std::size_t pizzeria_id) const {
  auto side_traverse = [&](int dx, int dy) {
    std::size_t j = 0;
    bool inside_field_x, inside_field_y, is_first_encountered_busy_point = true;
    std::size_t last_encountered_busy_id, counter_encountered_on_line = 0;
    while (1) {
      j++;
      inside_field_x =
          dx < 0 ? point.x() >= -dx * j : point.x() + dx * j < field_width_;
      inside_field_y =
          dy < 0 ? point.y() >= -dy * j : point.y() + dy * j < field_height_;
      if (!inside_field_x || !inside_field_y) break;
      std::size_t current_id =
          city_field_[point.y() + dy * j][point.x() + dx * j];
      if (current_id == 0) {
        continue;
      } else if (current_id == pizzeria_id) {
        break;
      } else if (current_id != pizzeria_id) {
        if (is_first_encountered_busy_point) {
          last_encountered_busy_id = current_id;
          is_first_encountered_busy_point = false;
        }

        if (current_id != last_encountered_busy_id) {
          break;
        }
        if (pizza_data_[current_id - 1].first !=
            Point(point.x() + dx * j, point.y() + dy * j)) {
          counter_encountered_on_line++;
        } else if (j <= pizza_data_[current_id - 1].second -
                            currently_expanded_[current_id - 1] +
                            counter_encountered_on_line &&
                   pizza_data_[current_id - 1].second >
                       currently_expanded_[current_id - 1]) {
          return true;
        }
      }
    }
    return false;
  };

  auto top = side_traverse(0, 1);
  auto bottom = side_traverse(0, -1);
  auto left = side_traverse(-1, 0);
  auto right = side_traverse(1, 0);

  return side_traverse(1, 0) || side_traverse(-1, 0) || side_traverse(0, 1) ||
         side_traverse(0, -1);
}

void Pizza_City::expand_unreachable_points(std::size_t pizzeria_id) {
  pizza_data_t pizzeria_and_capacity = pizza_data_[pizzeria_id - 1];
  std::vector<std::pair<int, int>> d_xy = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
  int d_x, d_y;
  for (std::size_t i = 0;
       i < d_xy.size() &&
       currently_expanded_[pizzeria_id - 1] < pizzeria_and_capacity.second;
       i++) {
    std::size_t start_of_expansion = 0, expansion_radius = 0, j = 0;
    std::tie(d_x, d_y) = d_xy[i];
    while (j < pizzeria_and_capacity.second -
                   currently_expanded_[pizzeria_id - 1]) {
      j++;
      Point expansion_point = {pizzeria_and_capacity.first.x() + j * d_x,
                               pizzeria_and_capacity.first.y() + j * d_y};

      if (expansion_point.x() < 0 || expansion_point.x() >= field_width_ ||
          expansion_point.y() < 0 || expansion_point.y() >= field_height_) {
        break;
      }
      std::size_t current_id =
          city_field_[expansion_point.y()][expansion_point.x()];
      if (current_id == pizzeria_id) {
        start_of_expansion = j;
        continue;
      }

      if (current_id == 0 && !is_pizzeria_point_reachable_to_other_pizzerias(
                                 expansion_point, pizzeria_id)) {
        expansion_radius = j;
      }
    }

    for (std::size_t j = start_of_expansion + 1; j <= expansion_radius; j++) {
      Point expansion_point = {pizzeria_and_capacity.first.x() + j * d_x,
                               pizzeria_and_capacity.first.y() + j * d_y};
      expand_by_point(expansion_point, pizzeria_id);
      untouchable_points[expansion_point.y()][expansion_point.x()] = true;
    }
  }
}

void Pizza_City::expand_pizzeria_with_no_choices(std::size_t pizzeria_id) {
  pizza_data_t pizzeria_and_capacity = pizza_data_[pizzeria_id - 1];
  Point pizzeria_point = pizzeria_and_capacity.first;
  std::size_t pizzeria_capacity_available =
      pizzeria_and_capacity.second - currently_expanded_[pizzeria_id - 1];

  if (pizzeria_capacity_available == 0) {
    return;
  }

  auto expansion = get_expansion_moves(pizzeria_point);

  if (expansion.size() == 1) {
    expand_by_point(expansion[0], pizzeria_id);
  }
}

void Pizza_City::expand_by_point(const Point &point, std::size_t pizzeria_id) {
  std::size_t current_id = city_field_[point.y()][point.x()];
  if (current_id != 0 && current_id != pizzeria_id) {
    throw std::invalid_argument(
        "Method expand_by_point: Point is already taken by another pizzeria");
  } else if (current_id == pizzeria_id) {
    return;
  }

  city_field_[point.y()][point.x()] = pizzeria_id;
  pizza_data_t pizzeria_and_capacity = pizza_data_[pizzeria_id - 1];

  int dx = point.x() - pizzeria_and_capacity.first.x();
  int dy = point.y() - pizzeria_and_capacity.first.y();
  if (dx != 0 && dy != 0) {
    throw std::invalid_argument(
        "Method expand_by_point: Point is not adjacent to pizzeria");
  }

  int top_increment = dy > 0 ? 1 : 0;
  int right_increment = dx > 0 ? 1 : 0;
  int down_increment = dy < 0 ? 1 : 0;
  int left_increment = dx < 0 ? 1 : 0;

  correct_expansions[pizzeria_id - 1] = {
      std::get<0>(correct_expansions[pizzeria_id - 1]) + top_increment,
      std::get<1>(correct_expansions[pizzeria_id - 1]) + right_increment,
      std::get<2>(correct_expansions[pizzeria_id - 1]) + down_increment,
      std::get<3>(correct_expansions[pizzeria_id - 1]) + left_increment,
  };

  currently_expanded_[pizzeria_id - 1]++;
  busy_points_count++;
}

void Pizza_City::reduce_by_point(const Point &point) {
  std::size_t pizzeria_id = city_field_[point.y()][point.x()];
  if (pizzeria_id == 0) {
    throw std::invalid_argument(
        "Method reduce_by_point: Point is not occupied by any pizzeria");
  }

  pizza_data_t pizzeria_and_capacity = pizza_data_[pizzeria_id - 1];

  int dx = point.x() - pizzeria_and_capacity.first.x();
  int dy = point.y() - pizzeria_and_capacity.first.y();
  if (dx != 0 && dy != 0) {
    throw std::invalid_argument(
        "Method reduce_by_point: Point is not adjacent to pizzeria");
  }

  int top_decrement = dy > 0 ? 1 : 0;
  int right_decrement = dx > 0 ? 1 : 0;
  int down_decrement = dy < 0 ? 1 : 0;
  int left_decrement = dx < 0 ? 1 : 0;

  correct_expansions[pizzeria_id - 1] = {
      std::get<0>(correct_expansions[pizzeria_id - 1]) - top_decrement,
      std::get<1>(correct_expansions[pizzeria_id - 1]) - right_decrement,
      std::get<2>(correct_expansions[pizzeria_id - 1]) - down_decrement,
      std::get<3>(correct_expansions[pizzeria_id - 1]) - left_decrement,
  };

  city_field_[point.y()][point.x()] = 0;
  currently_expanded_[pizzeria_id - 1]--;
  busy_points_count--;
}

std::vector<Point> Pizza_City::get_edge_expansion_points(
    std::size_t pizzeria_id) {
  Point pizzeria_point = pizza_data_[pizzeria_id - 1].first;
  return std::vector<Point>{
      {pizzeria_point.x(),
       pizzeria_point.y() + std::get<0>(correct_expansions[pizzeria_id - 1])},
      {pizzeria_point.x() + std::get<1>(correct_expansions[pizzeria_id - 1]),
       pizzeria_point.y()},
      {pizzeria_point.x(),
       pizzeria_point.y() - std::get<2>(correct_expansions[pizzeria_id - 1])},
      {pizzeria_point.x() - std::get<3>(correct_expansions[pizzeria_id - 1]),
       pizzeria_point.y()},
  };
}

std::size_t Pizza_City::get_pizzeria_id_by_point(const Point &point) {
  if (!point_inside_city(point)) {
    throw std::out_of_range(
        "Method get_pizzeria_id_by_point: Point out of range");
  }
  return city_field_[point.y()][point.x()];
}

void Pizza_City::iterating_coverage() {
  correct_expansions =
      std::vector<num_expansion_t>(pizza_data_.size(), {0, 0, 0, 0});
  bool is_expanding;
  do {
    is_expanding = false;
    for (std::size_t i = 0; i < pizza_data_.size(); i++) {
      std::size_t points_changes = busy_points_count;
      expand_unreachable_points(i + 1);
      points_changes -= busy_points_count;
      if (points_changes == 0) {
        is_expanding = is_expanding || false;
      } else {
        is_expanding = true;
      }
    }
  } while (is_expanding);

  // do {
  //   is_expanding = false;
  //   for (std::size_t i = 0; i < pizza_data_.size(); i++) {
  //     std::size_t points_changes = busy_points_count;
  //     expand_pizzeria_with_no_choices(i + 1);
  //     points_changes -= busy_points_count;
  //     if (points_changes == 0) {
  //       is_expanding = is_expanding || false;
  //     } else {
  //       is_expanding = true;
  //     }
  //   }
  // } while (is_expanding);

  std::size_t i = pizza_data_.size() - 1;
  bool first_cycle = true;
  auto pizza_data_sorted = pizza_data_;
  std::stable_sort(pizza_data_sorted.rbegin(), pizza_data_sorted.rend(),
                   [&](pizza_data_t a, pizza_data_t b) -> bool {
                     return a.second < b.second;
                   });

  while (busy_points_count <
         field_height_ * field_width_ - pizza_data_.size()) {
    i = (i + 1) % (pizza_data_.size());
    pizza_data_t pizzeria_and_capacity = pizza_data_sorted[i];
    Point pizza_point = pizzeria_and_capacity.first;
    std::size_t pizza_capacity = pizzeria_and_capacity.second;
    std::size_t pizzeria_id = city_field_[pizza_point.y()][pizza_point.x()];

    if (pizza_capacity <= currently_expanded_[pizzeria_id - 1]) {
      continue;
    }

#ifdef DEBUG_MODE
    std::cout << "Pizzeria_id = " << pizzeria_id << std::endl;
    std::cout << "Pizzeria: " << pizza_point << std::endl;
    std::cout << "Capacity: " << pizza_capacity << std::endl;
    for (std::size_t y = field_height_ - 1; y >= 0 && y < field_height_; --y) {
      for (std::size_t x = 0; x < field_width_; x++) {
        std::cout << city_field_[y][x] << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
#endif

    std::vector<Point> expansion;
    expansion = get_expansion_moves(pizzeria_and_capacity.first);

    if (expansion.size() == 0 &&
        currently_expanded_[pizzeria_id - 1] < pizza_capacity) {
      if (!fix_expansion(pizzeria_id)) {
        throw std::runtime_error("Error in fix_expansion");
      }
      i = (i + 1) % (pizza_data_.size());
      continue;
    }

    std::unordered_map<Point, double, PointHash> distances;

    for (std::size_t j = 0; j < expansion.size(); j++) {
      double distance =
          get_distance_to_nearest_busy_point(expansion[j], pizzeria_id);

      distances[expansion[j]] = distance;
    }

    std::stable_sort(
        expansion.rbegin(), expansion.rend(),
        [&](Point a, Point b) { return distances[a] < distances[b]; });

    expand_by_point(expansion.back(), pizzeria_id);
  }
}

std::vector<num_expansion_t> Pizza_City::get_correct_expansions() const {
  return correct_expansions;
}

std::string handle_pizza_city(std::istream &input) {
  std::stringstream result;
  std::size_t n, m, k, counter = 1;
  while (counter <= 30) {
    input >> n;
    if (n == 0) {
      break;
    }
    input >> m >> k;
    std::vector<pizza_data_t> pizzerias_and_capacities(k);
    std::size_t total_capacity = 0;
    for (std::size_t i = 0; i < k; ++i) {
      std::size_t x, y, c;
      input >> x >> y >> c;
      total_capacity += c;
      pizzerias_and_capacities[i] = {Point(x - 1, y - 1), c};
    }

    if (m * n != pizzerias_and_capacities.size() + total_capacity) {
      throw std::runtime_error("Invalid input");
    }

    Pizza_City pizza_city(m, n, pizzerias_and_capacities);
    result << "Case " << counter << ":\n";

    try {
      pizza_city.iterating_coverage();
      for (auto permutation : pizza_city.get_correct_expansions()) {
        result << std::get<0>(permutation) << " " << std::get<1>(permutation)
               << " " << std::get<2>(permutation) << " "
               << std::get<3>(permutation) << std::endl;
      }
    } catch (std::exception &e) {
      result << "No solution: " << e.what() << "" << std::endl;
    }
    result << std::endl;
    ++counter;
  }

  return result.str();
}

Pizza_City::Pizza_City(std::size_t field_height, std::size_t field_width,
                       const std::vector<pizza_data_t> &pizza_data)
    : field_height_(field_height),
      field_width_(field_width),
      pizza_data_(pizza_data) {
  city_field_ = new std::size_t *[field_height_];

  for (std::size_t i = 0; i < field_height_; i++) {
    city_field_[i] = new std::size_t[field_width_];
    for (std::size_t j = 0; j < field_width_; j++) {
      city_field_[i][j] = 0;
    }
  }

  currently_expanded_ = std::vector<std::size_t>(pizza_data_.size(), 0);

  fixed_expansions = std::vector<std::vector<bool>>(
      pizza_data_.size(), std::vector<bool>(pizza_data_.size(), false));
  untouchable_points = std::vector<std::vector<bool>>(
      field_height_, std::vector<bool>(field_width_, false));

  for (std::size_t i = 0; i < pizza_data_.size(); i++) {
    city_field_[pizza_data_[i].first.y()][pizza_data_[i].first.x()] = i + 1;
  }
}

Pizza_City::~Pizza_City() {
  for (std::size_t i = 0; i < field_height_; i++) {
    delete[] city_field_[i];
  }
  delete[] city_field_;
}

}  // namespace Pizza