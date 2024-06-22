#include "RGBGame.hpp"
#include <algorithm>
#include <thread>
#include <vector>

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

DSU::DSU() {
  for (std::size_t i = 0; i < FIELD_HEIGHT * FIELD_WIDTH; i++) {
    parent[i] = i;
    rank[i] = 1;
  }
}

std::size_t DSU::get_root(std::size_t i) {
  if (parent[i] == i) {
    return i;
  }
  return parent[i] = get_root(parent[i]);
}

void DSU::make_union(std::size_t i1, std::size_t i2) {
  auto root1 = get_root(i1);
  auto root2 = get_root(i2);
  if (root1 == root2) {
    return;
  }
  if (rank[root1] > rank[root2]) {
    std::swap(root1, root2);
  }
  parent[root1] = root2;
  if (rank[root1] == rank[root2]) {
    rank[root2]++;
  }
}

bool RGB_Game::out_of_field(const Point &point) {
  return (point.x() >= FIELD_WIDTH || point.y() >= FIELD_HEIGHT ||
          point.x() < 0 || point.y() < 0);
}

bool RGB_Game::can_move() {
  // TODO implement
  throw std::runtime_error("Not implemented");
}

void RGB_Game::clusterize_field() {
  bool visited[FIELD_HEIGHT][FIELD_WIDTH];
  std::fill_n(&visited[0][0], FIELD_HEIGHT * FIELD_WIDTH, false);

  for (std::size_t i = 0; i < FIELD_HEIGHT; i++) {
    for (std::size_t j = 0; j < FIELD_WIDTH; j++) {
      // TODO check incorrect chars in field
      if (visited[i][j]) {
        continue;
      }
      char left_elem = (!out_of_field(Point(j - 1, i)) && !visited[i][j - 1])
                           ? game_field[i][j - 1]
                           : ' ';
      char right_elem = (!out_of_field(Point(j + 1, i)) && !visited[i][j + 1])
                            ? game_field[i][j + 1]
                            : ' ';
      char top_elem = (!out_of_field(Point(j, i - 1)) && !visited[i - 1][j])
                          ? game_field[i - 1][j]
                          : ' ';
      char bottom_elem = (!out_of_field(Point(j, i + 1)) && !visited[i + 1][j])
                             ? game_field[i + 1][j]
                             : ' ';

      union_chars(&visited[i][j], game_field[i][j], Point(j, i), left_elem,
                  Point(j - 1, i));
      union_chars(&visited[i][j], game_field[i][j], Point(j, i), right_elem,
                  Point(j + 1, i));
      union_chars(&visited[i][j], game_field[i][j], Point(j, i), top_elem,
                  Point(j, i - 1));
      union_chars(&visited[i][j], game_field[i][j], Point(j, i), bottom_elem,
                  Point(j, i + 1));
    }
  }
}

void RGB_Game::union_chars(bool *visited, char elem1, const Point &point1,
                           char elem2, const Point &point2) {
  std::size_t dx = point1.x() < point2.x() ? point2.x() - point1.x()
                                           : point1.x() - point2.x();
  std::size_t dy = point1.y() < point2.y() ? point2.y() - point1.y()
                                           : point1.y() - point2.y();
  if (elem1 && elem1 == elem2 && dx + dy == 1) {
    field_dsu.make_union(point1.y() * FIELD_WIDTH + point1.x(),
                         point2.y() * FIELD_WIDTH + point2.x());
    visited[point1.y() * FIELD_WIDTH + point1.x()] = true;
    elem_to_cluster_size[point1]++;
    visited[point2.y() * FIELD_WIDTH + point2.x()] = true;
    elem_to_cluster_size[point2]++;
  }
}

void RGB_Game::update_field() {
  auto thread_raw_elems_shift = [&](std::size_t i) {
    std::size_t first_non_empty_index = -1, first_empty_index = -1; // overflow
    do {
      for (std::size_t j = FIELD_HEIGHT - 1; j > 0; --j) {
        if (game_field[j][i] == ' ') {
          std::swap(game_field[j][i], game_field[j - 1][i]);
        }
      }
      // TODO avoid code duplication
      for (std::size_t j = 0; j < FIELD_HEIGHT; ++j) {
        if (game_field[j][i] != ' ') {
          first_non_empty_index = j;
          break;
        }
      }
      for (std::size_t j = 0; j < FIELD_HEIGHT; ++j) {
        if (game_field[j][i] == ' ') {
          first_empty_index = j;
          break;
        }
      }
    } while (first_non_empty_index != first_empty_index + 1);
  };

  std::vector<std::thread> threads;
  for (std::size_t i = 0; i < FIELD_WIDTH; ++i) {
    threads.emplace_back(thread_raw_elems_shift, i);
  }
  for (auto &thread : threads) {
    thread.join();
  }
  threads.clear();
  // shift of raws ended
  // TODO divide into two parts
  // shift of columns started
  auto column_is_empty = [&](std::size_t i) {
    for (std::size_t j = 0; j < FIELD_HEIGHT; ++j) {
      if (game_field[j][i] != ' ') {
        return false;
      }
    }
    return true;
  };

  std::size_t first_empty_column_index = -1, first_non_empty_column_index = -1;
  do {
    for (std::size_t i = 0; i < FIELD_WIDTH - 1; ++i) {
      if (column_is_empty(i)) {
        for (std::size_t j = 0; j < FIELD_HEIGHT; ++j) {
          std::swap(game_field[j][i], game_field[j][i + 1]);
        }
      }
    }
    // TODO avoid code duplication
    for (std::size_t i = FIELD_WIDTH - 1; i > 0; --i) {
      if (column_is_empty(i)) {
        first_empty_column_index = i;
        break;
      }
    }
    for (std::size_t i = FIELD_WIDTH - 1; i > 0; --i) {
      if (!column_is_empty(i)) {
        first_non_empty_column_index = i;
        break;
      }
    }

  } while (first_empty_column_index != first_non_empty_column_index + 1);
}

Point RGB_Game::choose_move() {
  std::size_t max_cluster_size = 0;
  Point best_move = Point(-1, -1);
  for (std::size_t i = FIELD_HEIGHT; i >= 0; --i) {
    for (std::size_t j = 0; j < FIELD_WIDTH; ++j) {
      if (game_field[i][j] != ' ') {
        std::size_t cluster_size = elem_to_cluster_size[Point(j, i)];
        if (cluster_size > max_cluster_size) {
          max_cluster_size = cluster_size;
          best_move = Point(j, i);
        }
      }
    }
  }
  return best_move;
}

RGB_Game::RGB_Game(char **field) {
  field_dsu = DSU();
  for (std::size_t i = 0; i < FIELD_HEIGHT; i++) {
    for (std::size_t j = 0; j < FIELD_WIDTH; j++) {
      game_field[i][j] = field[i][j];
      elem_to_cluster_size[Point(j, i)] = 1;
    }
  }
  clusterize_field();
}

auto RGB_Game::get_field() { return game_field; }

void RGB_Game::play() {
  std::size_t move_count = 0;
  while (can_move()) {
    Point move = choose_move();
    make_move(move_count++, move);
  }
  game_log << "Final score " << total_score << "with " << count_balls_on_field() << " balls remaining\n";
}

void RGB_Game::make_move(std::size_t move_count, const Point &point) {
  std::size_t n_erased = 0;
  char color = game_field[point.y()][point.x()];
  for (std::size_t i = 0; i < FIELD_HEIGHT; i++) {
    for (std::size_t j = 0; j < FIELD_WIDTH; j++) {
      if (field_dsu.get_root(i * FIELD_WIDTH + j) ==
          field_dsu.get_root(point.y() * FIELD_WIDTH + point.x())) {
        game_field[i][j] = ' ';
        elem_to_cluster_size.erase(Point(j, i));
        n_erased++;
      }
    }
  }
  std::size_t acquired_points = (n_erased - 2) * (n_erased - 2);
  total_score += acquired_points;
  game_log << "Move " << move_count << " at (" << point.x() << ", " << point.y()
           << ")" << ": removed" << n_erased << " balls of color " << color
           << ", got " << acquired_points << " points\n";
  update();
}

void RGB_Game::update() {
  update_field();
  clusterize_field();
}

std::size_t RGB_Game::count_balls_on_field() { 
  // TODO implement
  throw std::runtime_error("Not implemented");
}
