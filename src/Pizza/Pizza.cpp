#include "Pizza.hpp"

// #define DEBUG_MODE

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

std::size_t Point::x() const { return x_; }

std::size_t Point::y() const { return y_; }

std::size_t PointHash::operator()(const Point &point) const {
  return std::hash<std::size_t>()(point.x()) ^
         std::hash<std::size_t>()(point.y());
}

std::vector<num_permutation_t> find_permutations(
    std::size_t capacity, std::size_t target, num_permutation_t constraints) {
  std::vector<num_permutation_t> result;
  for (std::size_t i = 0;
       i <= capacity && i < get_permutation_value(constraints, 3) &&
       i <= target;
       ++i) {
    for (std::size_t j = 0;
         j <= capacity && j < get_permutation_value(constraints, 2) &&
         j <= target - i;
         ++j) {
      for (std::size_t k = 0;
           k <= capacity && k < get_permutation_value(constraints, 1) &&
           k <= target - i - j;
           ++k) {
        for (std::size_t l = 0;
             l <= capacity && l < get_permutation_value(constraints, 0) &&
             l <= target - i - j - k;
             ++l) {
          if (i + j + k + l == target) {
            result.push_back({l, k, j, i});
          }
        }
      }
    }
  }
  return result;
}

std::size_t get_permutation_value(const num_permutation_t &permutation,
                                  std::size_t index) {
  switch (index) {
    case 0:
      return std::get<0>(permutation);
      break;
    case 1:
      return std::get<1>(permutation);
      break;
    case 2:
      return std::get<2>(permutation);
      break;
    case 3:
      return std::get<3>(permutation);
      break;
    default:
      throw std::invalid_argument("Invalid index");
      break;
  }
}

std::string handle_pizza_city(std::istream &input) {
  std::stringstream result;
  std::size_t n, m, k, counter = 1;
  while (1) {
    input >> n;
    if (n == 0) {
      break;
    }
    input >> m >> k;
    std::vector<Point> pizzerias(k);
    std::vector<std::size_t> pizza_capacities(k);
    for (std::size_t i = 0; i < k; ++i) {
      std::size_t x, y, c;
      input >> x >> y >> c;
      pizzerias[i] = Point(x - 1, y - 1);
      pizza_capacities[i] = c;
    }

    Pizza_City pizza_city(m, n, pizzerias, pizza_capacities);
    result << "Case " << counter << ":\n";

    if (pizza_city.cover_all_city()) {
      for (const auto &permutation : pizza_city.get_correct_permutations()) {
        result << get_permutation_value(permutation, 0) << " "
               << get_permutation_value(permutation, 1) << " "
               << get_permutation_value(permutation, 2) << " "
               << get_permutation_value(permutation, 3) << std::endl;
      }
    }
    result << std::endl;
    ++counter;
  }

  return result.str();
}

bool Pizza_City::add_pizzeria_coverage(const Point &pizzeria,
                                       const num_permutation_t &permutation) {
  bool result = true;
  auto &val = city_field_[pizzeria.y()][pizzeria.x()];

  auto side_addition = [&](int i, int dx, int dy) {
    std::size_t i_permutation = get_permutation_value(permutation, i);
    for (std::size_t j = 1; j <= i_permutation; ++j) {
      bool inside_field_x = dx < 0 ? pizzeria.x() >= -dx * j
                                   : pizzeria.x() + dx * j < field_width_;
      bool inside_field_y = dy < 0 ? pizzeria.y() >= -dy * j
                                   : pizzeria.y() + dy * j < field_height_;
      if (inside_field_x && inside_field_y &&
          city_field_[pizzeria.y() + dy * j][pizzeria.x() + dx * j] == 0) {
        city_field_[pizzeria.y() + dy * j][pizzeria.x() + dx * j] = val;
      } else {
        result = false;
      }
    }
  };

  side_addition(0, 0, -1);
  side_addition(1, 1, 0);
  side_addition(2, 0, 1);
  side_addition(3, -1, 0);

  // previously we could fill the incorrect cells and we need to get rid of it
  if (!result) {
    remove_pizzeria_coverage(pizzeria, permutation);
  }
  return result;
}

void Pizza_City::remove_pizzeria_coverage(
    const Point &pizzeria, const num_permutation_t &permutation) {
  auto side_deletion = [&](int i, int dx, int dy) {
    std::size_t i_permutation = get_permutation_value(permutation, i);
    std::size_t pizzeria_id = city_field_[pizzeria.y()][pizzeria.x()];
    for (std::size_t j = 1; j <= i_permutation; ++j) {
      bool inside_field_x = dx < 0 ? pizzeria.x() >= -dx * j
                                   : pizzeria.x() + dx * j < field_width_;
      bool inside_field_y = dy < 0 ? pizzeria.y() >= -dy * j
                                   : pizzeria.y() + dy * j < field_height_;
      if (inside_field_x && inside_field_y &&
          city_field_[pizzeria.y() + dy * j][pizzeria.x() + dx * j] ==
              pizzeria_id)
        city_field_[pizzeria.y() + dy * j][pizzeria.x() + dx * j] = 0;
    }
  };

  side_deletion(0, 0, -1);
  side_deletion(1, 1, 0);
  side_deletion(2, 0, 1);
  side_deletion(3, -1, 0);
}

bool Pizza_City::backtracking(std::size_t pizzeria_id) {
  const Point &pizzeria = pizza_coords_[pizzeria_id - 1];
  const std::size_t &pizzeria_capacity = pizza_capacities_[pizzeria_id - 1];
  auto constraints = get_expansion_constraints(pizzeria);
  auto all_permutations =
      find_permutations(pizzeria_capacity, pizzeria_capacity, constraints);

  for (const auto &permutation : all_permutations) {
    if (add_pizzeria_coverage(pizzeria, permutation)) {
      correct_permutations_.push_back(permutation);
      if (pizzeria_id == pizza_coords_.size() && is_city_covered_correctly()) {
        return true;
      } else if (pizzeria_id == pizza_coords_.size()) {
        return false;
      }
      if (backtracking(pizzeria_id + 1)) {
        return true;
      }
      correct_permutations_.pop_back();
      remove_pizzeria_coverage(pizzeria, permutation);
    }
  }
  return false;
}

num_permutation_t Pizza_City::get_expansion_constraints(const Point &pizzeria) {
  auto side_expansion = [&](int i, int dx, int dy) {
    std::size_t j = 0;
    bool inside_field_x, inside_field_y;
    do {
      j++;
      inside_field_x = dx < 0 ? pizzeria.x() >= -dx * j
                              : pizzeria.x() + dx * j < field_width_;
      inside_field_y = dy < 0 ? pizzeria.y() >= -dy * j
                              : pizzeria.y() + dy * j < field_height_;
      if (!inside_field_x || !inside_field_y) break;
    } while (!city_field_[pizzeria.y() + dy * j][pizzeria.x() + dx * j]);
    return j;
  };

  auto up = side_expansion(0, 0, -1);
  auto right = side_expansion(1, 1, 0);
  auto down = side_expansion(2, 0, 1);
  auto left = side_expansion(3, -1, 0);
  return std::make_tuple(up, right, down, left);
}

std::vector<Point> Pizza_City::get_possible_expansion_moves(
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

  std::vector<Point> possible_expansion_moves;

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
        possible_expansion_moves.push_back(
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
  return possible_expansion_moves;
}

double Pizza_City::get_distance_to_nearest_busy_point(
    Point current_point, std::size_t pizzeria_id, std::size_t filling_step) {
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
          pizza_capacities_[traverse_id - 1] >= filling_step) {
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

bool Pizza_City::is_pizzeria_point_reachable_to_other_pizzeria(
    const Point &point, std::size_t pizzeria_id) const {
  auto side_traverse = [&](int dx, int dy) {
    std::size_t j = 0;
    bool inside_field_x, inside_field_y;
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
      } else if (current_id != 0 && current_id != pizzeria_id &&
                 pizza_capacities_[current_id - 1] > j &&
                 pizza_coords_[current_id - 1] ==
                     Point(point.x() + dx * j, point.y() + dy * j)) {
        return true;
      }
    }
    return false;
  };

  bool left = side_traverse(-1, 0);
  bool right = side_traverse(1, 0);
  bool up = side_traverse(0, 1);
  bool down = side_traverse(0, -1);

  return side_traverse(1, 0) || side_traverse(-1, 0) || side_traverse(0, 1) ||
         side_traverse(0, -1);
}

void Pizza_City::iterating_coverage() {
  // TODO remove this from here and add where better (as class field in cstr
  // i.e.)
  std::vector<pizza_data_t> pizzerias_and_capacities;
  for (std::size_t i = 0; i < pizza_coords_.size(); i++) {
    pizzerias_and_capacities.push_back(
        {pizza_coords_[i], pizza_capacities_[i]});
  }
  correct_expansion = std::vector<num_permutation_t>(
      pizzerias_and_capacities.size(), {0, 0, 0, 0});
  std::size_t filling_step = 0, busy_points_count = 0;

  // expansion iteration
  while (busy_points_count <
         field_height_ * field_width_ - pizzerias_and_capacities.size()) {
    filling_step++;
    for (std::size_t i = 0; i < pizzerias_and_capacities.size(); i++) {
      pizza_data_t pizzeria_and_capacity = pizzerias_and_capacities[i];
      if (pizzeria_and_capacity.second < filling_step) {
        continue;
      }

      std::vector<Point> possible_moves =
          get_possible_expansion_moves(pizzeria_and_capacity.first);
      std::unordered_map<Point, double, PointHash> possible_moves_distances;

      for (std::size_t j = 0; j < possible_moves.size(); j++) {
        auto distance = get_distance_to_nearest_busy_point(possible_moves[j],
                                                           i + 1, filling_step);
        possible_moves_distances[possible_moves[j]] = distance;
      }

      // // need to take the farest point
      std::stable_sort(possible_moves.begin(), possible_moves.end(),
                       [&](const Point &a, const Point &b) {
                         return possible_moves_distances[a] <
                                possible_moves_distances[b];
                       });

      Point move = possible_moves.back();
      for (auto possible_move : possible_moves) {
        if (!is_pizzeria_point_reachable_to_other_pizzeria(possible_move,
                                                           i + 1)) {
          move = possible_move;
          break;
        }
      }
      city_field_[move.y()][move.x()] = i + 1;
      busy_points_count++;

      int top_increment = move.y() > pizzeria_and_capacity.first.y() ? 1 : 0;
      int right_increment = move.x() > pizzeria_and_capacity.first.x() ? 1 : 0;
      int left_increment = move.x() < pizzeria_and_capacity.first.x() ? 1 : 0;
      int down_increment = move.y() < pizzeria_and_capacity.first.y() ? 1 : 0;

      correct_expansion[i] = {
          std::get<0>(correct_expansion[i]) + top_increment,
          std::get<1>(correct_expansion[i]) + right_increment,
          std::get<2>(correct_expansion[i]) + down_increment,
          std::get<3>(correct_expansion[i]) + left_increment,
      };

#ifdef DEBUG_MODE
      std::cout << "step " << filling_step << " pizzeria " << i + 1 << ": "
                << std::endl;
      for (auto permutation : correct_expansion) {
        std::cout << std::get<0>(permutation) << " " << std::get<1>(permutation)
                  << " " << std::get<2>(permutation) << " "
                  << std::get<3>(permutation) << std::endl;
      }
      std::cout << std::endl;
#endif
    }
#ifdef DEBUG_MODE
    std::cout << "City: " << std::endl;
    for (std::size_t y = field_height_ - 1; y >= 0 && y < field_height_; --y) {
      for (std::size_t x = 0; x < field_width_; x++) {
        std::cout << city_field_[y][x] << " ";
      }
      std::cout << std::endl;
    }
#endif
  }
}

std::string handle_iterating_pizza_city(std::istream &input) {
  std::stringstream result;
  std::size_t n, m, k, counter = 1;
  while (1) {
    input >> n;
    if (n == 0) {
      break;
    }
    input >> m >> k;
    std::vector<Point> pizzerias(k);
    std::vector<std::size_t> pizza_capacities(k);
    for (std::size_t i = 0; i < k; ++i) {
      std::size_t x, y, c;
      input >> x >> y >> c;
      pizzerias[i] = Point(x - 1, y - 1);
      pizza_capacities[i] = c;
    }

    Pizza_City pizza_city(m, n, pizzerias, pizza_capacities);
    result << "Case " << counter << ":\n";

    pizza_city.iterating_coverage();
    for (auto permutation : pizza_city.correct_expansion) {
      result << std::get<0>(permutation) << " " << std::get<1>(permutation)
             << " " << std::get<2>(permutation) << " "
             << std::get<3>(permutation) << std::endl;
    }
    result << std::endl;
    ++counter;
  }

  return result.str();
}

Pizza_City::Pizza_City(std::size_t field_height, std::size_t field_width,
                       const std::vector<Point> &pizza_coords,
                       const std::vector<std::size_t> &pizza_capacities)
    : field_height_(field_height),
      field_width_(field_width),
      pizza_coords_(pizza_coords),
      pizza_capacities_(pizza_capacities) {
  city_field_ = new std::size_t *[field_height_];

  for (std::size_t i = 0; i < field_height_; i++) {
    city_field_[i] = new std::size_t[field_width_];
    for (std::size_t j = 0; j < field_width_; j++) {
      city_field_[i][j] = 0;
    }
  }

  for (std::size_t i = 0; i < pizza_coords_.size(); i++) {
    city_field_[pizza_coords_[i].y()][pizza_coords_[i].x()] = i + 1;
  }
}

bool Pizza_City::cover_all_city() { return backtracking(1); }

bool Pizza_City::is_city_covered_correctly() const {
  for (std::size_t i = 0; i < field_height_; i++) {
    for (std::size_t j = 0; j < field_width_; j++) {
      if (city_field_[i][j] == 0) {
        return false;
      }
    }
  }
  return true;
}

std::size_t **Pizza_City::get_city_field() const { return city_field_; }

const std::vector<num_permutation_t> &Pizza_City::get_correct_permutations()
    const {
  return correct_permutations_;
}

Pizza_City::~Pizza_City() {
  for (std::size_t i = 0; i < field_height_; i++) {
    delete[] city_field_[i];
  }
  delete[] city_field_;
}

}  // namespace Pizza