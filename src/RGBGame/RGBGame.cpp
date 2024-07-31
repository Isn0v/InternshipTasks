#include "RGBGame.hpp"

#include <algorithm>
#include <sstream>
#include <unordered_set>
#include <vector>

namespace RGB_Game {

std::unordered_map<char, bool> chars_allowed{
    {'R', true}, {'G', true}, {'B', true}};

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

std::string handle_rgb_game(std::istream &input) {
  std::stringstream result;

  std::size_t n, counter = 1;
  input >> n;
  for (std::size_t i = 0; i < n; i++) {
    result << "Game " << counter++ << ":" << std::endl;
    char field[FIELD_HEIGHT][FIELD_WIDTH];
    for (std::size_t j = 0; j < FIELD_HEIGHT; j++) {
      for (std::size_t k = 0; k < FIELD_WIDTH; k++) {
        input >> field[j][k];
      }
    }

    RGB_Game game(field);
    game.play();
    result << game.dumps_log() << std::endl;
  }

  return result.str();
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

void DSU::init() {
  for (std::size_t i = 0; i < FIELD_HEIGHT * FIELD_WIDTH; i++) {
    parent[i] = i;
    rank[i] = 1;
    root_to_cluster_size[i] = 1;
  }
}

void DSU::reset() { init(); }

DSU::DSU() { init(); }

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
  if (rank[root1] < rank[root2]) {
    parent[root1] = root2;
    root_to_cluster_size[root2] += root_to_cluster_size[root1];
  } else if (rank[root2] < rank[root1]) {
    parent[root2] = root1;
    root_to_cluster_size[root1] += root_to_cluster_size[root2];
  } else {
    parent[root1] = root2;
    rank[root1]++;
    root_to_cluster_size[root2] += root_to_cluster_size[root1];
  }
}

std::size_t DSU::get_cluster_size(std::size_t i) {
  return root_to_cluster_size[get_root(i)];
}

auto DSU::get_cluster_sizes() const { return root_to_cluster_size; }

bool RGB_Game::out_of_field(const Point &point) {
  // cuz of std::size_t we don't need to check < 0
  return (point.x() >= FIELD_WIDTH || point.y() >= FIELD_HEIGHT);
}

void RGB_Game::clusterize_field() {
  for (std::size_t i = 0; i < FIELD_HEIGHT; ++i) {
    for (std::size_t j = 0; j < FIELD_WIDTH; ++j) {
      if (!game_field[i][j]) continue;

      auto make_union = [&](std::size_t dx, std::size_t dy) {
        char neighbour_elem = out_of_field(Point(j + dx, i + dy))
                                  ? 0
                                  : game_field[i + dy][j + dx];
        if (neighbour_elem && neighbour_elem == game_field[i][j]) {
          field_dsu.make_union(i * FIELD_WIDTH + j,
                               (i + dy) * FIELD_WIDTH + j + dx);
        }
      };

      make_union(0, -1);
      make_union(1, 0);
      make_union(0, 1);
      make_union(-1, 0);
    }
  }
}

void shift_raw_elems(char (&game_field)[FIELD_HEIGHT][FIELD_WIDTH]) {
  std::size_t counter = 0;
  // amount of shifts can't be more than FIELD_HEIGHT because we have
  // FIELD_HEIGHT amount of rows
  while (counter < FIELD_HEIGHT) {
    for (std::size_t i = 0; i < FIELD_WIDTH; ++i) {
      for (std::size_t j = FIELD_HEIGHT - 1; j > 0; --j) {
        if (!game_field[j][i] && game_field[j - 1][i]) {
          std::swap(game_field[j][i], game_field[j - 1][i]);
        }
      }
    }
    ++counter;
  }
}

void shift_column_elems(char (&game_field)[FIELD_HEIGHT][FIELD_WIDTH]) {
  auto column_is_empty = [&](std::size_t i) {
    for (std::size_t j = 0; j < FIELD_HEIGHT; ++j) {
      if (game_field[j][i]) {
        return false;
      }
    }
    return true;
  };
  std::size_t counter = 0;
  // amount of shifts can't be more than FIELD_WIDTH because we have FIELD_WIDTH
  // amount of columns
  while (counter < FIELD_WIDTH) {
    for (std::size_t i = 0; i < FIELD_WIDTH - 1; ++i) {
      if (column_is_empty(i)) {
        for (std::size_t j = 0; j < FIELD_HEIGHT; ++j) {
          std::swap(game_field[j][i], game_field[j][i + 1]);
        }
      }
    }
    ++counter;
  }
}

void RGB_Game::update_field() {
  shift_raw_elems(game_field);
  shift_column_elems(game_field);
}

Point RGB_Game::choose_move() {
  // overflow because of std::size_t
  Point best_move = Point(-1, 0);
  std::size_t max_cluster_size = 2;
  // this iteration is according to the rules of choosing move from the task
  for (std::size_t j = 0; j < FIELD_WIDTH; ++j) {
    for (std::size_t i = FIELD_HEIGHT - 1; i > 0; --i) {
      if (game_field[i][j]) {
        std::size_t cluster_size =
            field_dsu.get_cluster_size(i * FIELD_WIDTH + j);
        if (cluster_size > max_cluster_size) {
          max_cluster_size = cluster_size;
          best_move = Point(j, i);
        }
      }
    }
  }
  return best_move;
}

RGB_Game::RGB_Game(char (&field)[FIELD_HEIGHT][FIELD_WIDTH]) {
  field_dsu = DSU();
  for (std::size_t i = 0; i < FIELD_HEIGHT; i++) {
    for (std::size_t j = 0; j < FIELD_WIDTH; j++) {
      if (!chars_allowed[field[i][j]]) {
        throw std::invalid_argument("Invalid char in field");
      }
      game_field[i][j] = field[i][j];
    }
  }
  clusterize_field();
}

auto RGB_Game::get_field() { return game_field; }

void RGB_Game::play() {
  std::size_t move_count = 1;
  while (1) {
    Point move = choose_move();
    if (move == Point(-1, 0)) {
      break;
    }
    make_move(move_count++, move);
    update();
  }
  std::size_t n_balls = count_balls_on_field();
  total_score = (n_balls == 0) ? total_score + 1000 : total_score;

  game_log << "Final score " << total_score << " with " << n_balls
           << " balls remaining\n";
}

std::string RGB_Game::dumps_log() { return game_log.str(); }

void RGB_Game::make_move(std::size_t move_count, const Point &point) {
  std::size_t n_erased = 0;
  char color = game_field[point.y()][point.x()];
  for (std::size_t i = 0; i < FIELD_HEIGHT; i++) {
    for (std::size_t j = 0; j < FIELD_WIDTH; j++) {
      if (field_dsu.get_root(i * FIELD_WIDTH + j) ==
          field_dsu.get_root(point.y() * FIELD_WIDTH + point.x())) {
        game_field[i][j] = 0;
        n_erased++;
      }
    }
  }
  std::size_t acquired_points = (n_erased - 2) * (n_erased - 2);
  total_score += acquired_points;

  // [FIELD_HEIGHT - point.y()] and [point.x() + 1] because of the
  // relation of the coordinate basises in the algorithm and in the task
  game_log << "Move " << move_count << " at (" << FIELD_HEIGHT - point.y()
           << ", " << point.x() + 1 << ")" << ": removed " << n_erased
           << " balls of color " << color << ", got " << acquired_points
           << " points\n";
}

void RGB_Game::update() {
  update_field();
  field_dsu.reset();
  clusterize_field();
}

std::size_t RGB_Game::count_balls_on_field() {
  std::size_t count = 0;
  for (std::size_t i = 0; i < FIELD_HEIGHT; i++) {
    for (std::size_t j = 0; j < FIELD_WIDTH; j++) {
      if (game_field[i][j]) {
        count++;
      }
    }
  }
  return count;
}

}  // namespace RGB_Game