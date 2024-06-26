#include "Pizza.hpp"

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

std::vector<num_permutation_t> find_permutations(std::size_t capacity,
                                                 std::size_t target) {
  std::vector<num_permutation_t> result;
  for (std::size_t i = 0; i <= capacity; ++i) {
    for (std::size_t j = 0; j <= capacity; ++j) {
      for (std::size_t k = 0; k <= capacity; ++k) {
        for (std::size_t l = 0; l <= capacity; ++l) {
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

std::string handle_pizza_city(const std::string &input) {
  std::stringstream result;
  std::istringstream ss(input);
  std::size_t n, m, k, counter = 1;
  while (1) {
    ss >> n;
    if (n == 0) {
      break;
    }
    ss >> m >> k;
    std::vector<Point> pizzerias(k);
    std::vector<std::size_t> pizza_capacities(k);
    for (std::size_t i = 0; i < k; ++i) {
      std::size_t x, y, c;
      ss >> x >> y >> c;
      pizzerias[i] = Point(x - 1, y - 1);
      pizza_capacities[i] = c;
    }

    result << "Case " << counter << ":\n";
    Pizza_City pizza_city(m, n, pizzerias, pizza_capacities);
    if (pizza_city.cover_all_city()) {
      for (const auto &permutation : pizza_city.get_correct_permutations()) {
        result << get_permutation_value(permutation, 0) << " "
               << get_permutation_value(permutation, 1) << " "
               << get_permutation_value(permutation, 2) << " "
               << get_permutation_value(permutation, 3) << std::endl;
      }
    } else {
      result << "Impossible to cover city" << std::endl;
    }
    result << std::endl;
    ++counter;
  }

  return result.str();
}

void dump_permutation(const num_permutation_t &permutation) {
  std::cout << "(" << std::get<0>(permutation) << ", "
            << std::get<1>(permutation) << ", " << std::get<2>(permutation)
            << ", " << std::get<3>(permutation) << ")" << std::endl;
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
      auto &val_dx_dy =
          city_field_[pizzeria.y() + dy * j][pizzeria.x() + dx * j];
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
  auto all_permutations =
      find_permutations(pizzeria_capacity, pizzeria_capacity);

  for (const auto &permutation : all_permutations) {
    if (add_pizzeria_coverage(pizzeria, permutation)) {
      correct_permutations_.push_back(permutation);
      if (pizzeria_id == pizza_coords_.size()) {
        return true;
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

bool Pizza_City::is_city_covered() const { 
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