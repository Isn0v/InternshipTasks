#pragma once
#include <cstddef>
#include <unordered_map>
#include <iostream>
#include <sstream>

#define FIELD_WIDTH 15
#define FIELD_HEIGHT 10

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

public:
  DSU();
  std::size_t get_root(std::size_t i);
  void make_union(std::size_t i1, std::size_t i2);
};

class RGB_Game {
private:
  char game_field[FIELD_HEIGHT][FIELD_WIDTH];
  std::size_t total_score = 0;
  std::unordered_map<Point, std::size_t, PointHash> elem_to_cluster_size;
  std::stringstream game_log;
  DSU field_dsu;

  bool out_of_field(const Point& point);
  bool can_move();

  void clusterize_field();
  void union_chars(bool *visited, char elem1, const Point& point1,
                   char elem2, const Point& point2);
  void update_field();
  Point choose_move();
  void update();
  void make_move(std::size_t move_count, const Point& point);

  std::size_t count_balls_on_field();

public:
  RGB_Game(char **field);
  auto get_field();

  void play();
};