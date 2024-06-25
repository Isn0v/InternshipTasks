#pragma once
#include <cstddef>
#include <iostream>
#include <sstream>
#include <unordered_map>

#define FIELD_WIDTH 15
#define FIELD_HEIGHT 10

namespace RGB_Game {

class Point {
 private:
  std::size_t x_, y_;

 public:
  std::size_t x() const;
  std::size_t y() const;

  Point() = default;
  Point(std::size_t x, std::size_t y);
  Point(const Point &point);
  Point &operator=(const Point &point);

  bool operator==(const Point &point) const;
  bool operator!=(const Point &point) const;

  friend std::ostream &operator<<(std::ostream &os, const Point &wall);
};

struct PointHash {
  std::size_t operator()(const Point &point) const;
};

class DSU {
 private:
  std::size_t parent[FIELD_HEIGHT * FIELD_WIDTH];
  std::size_t rank[FIELD_HEIGHT * FIELD_WIDTH];
  std::unordered_map<std::size_t, std::size_t> root_to_cluster_size;

  void init();

 public:
  DSU();
  void reset();
  void make_union(std::size_t i1, std::size_t i2);

  std::size_t get_root(std::size_t i);
  std::size_t get_cluster_size(std::size_t i);

  auto get_cluster_sizes() const;

  friend class RGB_Game;
};

class RGB_Game {
 private:
  DSU field_dsu;
  char game_field[FIELD_HEIGHT][FIELD_WIDTH];
  std::size_t total_score = 0;
  std::stringstream game_log;

  bool out_of_field(const Point &point);
  std::size_t count_balls_on_field();
  Point choose_move();

  void clusterize_field();
  void update_field();
  void update();
  void make_move(std::size_t move_count, const Point &point);

 public:
  RGB_Game(char (&field)[FIELD_HEIGHT][FIELD_WIDTH]);
  auto get_field();

  void play();
  std::string dumps_log();
};
}  // namespace RGB_Game