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

/**
 * @brief Handle the RGB game by reading the input and returning the result.
 *
 * @param input The input stream to read from.
 * @return The result of the RGB game as a string.
 */
std::string handle_rgb_game(std::istream &input) {
  // Create a string stream to store the result
  std::stringstream result;

  // Read the number of games from the input stream
  std::size_t n;
  input >> n;

  // Iterate over each game
  std::size_t counter = 1;
  for (std::size_t i = 0; i < n; i++) {
    // Start a new game
    result << "Game " << counter++ << ":" << std::endl;

    // Read the game field from the input stream
    char field[FIELD_HEIGHT][FIELD_WIDTH];
    for (std::size_t j = 0; j < FIELD_HEIGHT; j++) {
      for (std::size_t k = 0; k < FIELD_WIDTH; k++) {
        input >> field[j][k];
      }
    }

    try {
      // Create a RGB_Game object and play it
      RGB_Game game(field);
      game.play();

      // Append the log of the game to the result
      result << game.dumps_log() << std::endl;
    } catch (const std::invalid_argument &e) {
      result << "Invalid input: " << e.what() << std::endl;
    }
  }

  // Return the result as a string
  return result.str();
}

/**
 * @brief Overloaded equality operator for Point objects.
 *
 * This operator checks if two Point objects are equal by comparing their x and
 * y coordinates.
 *
 * @param point The Point object to compare with.
 * @return true if the Point objects have the same x and y coordinates, false
 * otherwise.
 */
bool Point::operator==(const Point &point) const {
  // Check if the x and y coordinates of the two Point objects are equal
  return x_ == point.x_ && y_ == point.y_;
}

bool Point::operator!=(const Point &point) const { return !(*this == point); }

std::size_t Point::x() const { return x_; }

std::size_t Point::y() const { return y_; }

/**
 * @brief Hash function for Point objects.
 *
 * This function combines the hash values of the x and y coordinates of the
 * Point object using the bitwise XOR operator.
 *
 * @param point The Point object to be hashed.
 *
 * @return The hash value of the Point object.
 */
std::size_t PointHash::operator()(const Point &point) const {
  // Get the hash values of the x and y coordinates of the Point object
  std::size_t hash_x = std::hash<std::size_t>()(point.x());
  std::size_t hash_y = std::hash<std::size_t>()(point.y());

  // Combine the hash values using the bitwise XOR operator
  return hash_x ^ hash_y;
}

/**
 * @brief Initializes the Disjoint Set Union (DSU) data structure.
 *
 * This function initializes the parent, rank, and root_to_cluster_size
 * arrays for the DSU data structure. It assigns each element its own parent
 * and rank, and sets the cluster size to 1.
 *
 * @throws None
 */
void DSU::init() {
  // Iterate over each element in the field
  for (std::size_t i = 0; i < FIELD_HEIGHT * FIELD_WIDTH; i++) {
    // Set the parent of each element to itself
    parent[i] = i;
    // Set the rank of each element to 1
    rank[i] = 1;
    // Set the cluster size of each element to 1
    root_to_cluster_size[i] = 1;
  }
}

void DSU::reset() { init(); }

DSU::DSU() { init(); }

/**
 * @brief Finds and returns the root of a given element in the disjoint set.
 *
 * This function uses path compression to find the root of a given element.
 * It recursively traverses the parent array until it reaches the root of the
 * set. Along the way, it updates the parent array to have each element point
 * directly to the root, thereby compressing the path.
 *
 * @param i The index of the element whose root is to be found.
 *
 * @return The index of the root element in the disjoint set.
 */
std::size_t DSU::get_root(std::size_t i) {
  // If the parent of the element is itself, it is the root
  if (parent[i] == i) {
    return i;
  }
  // Otherwise, recursively find the root of the parent and update the parent
  // array to have the element point directly to the root
  return parent[i] = get_root(parent[i]);
}

/**
 * @brief Merges two disjoint sets containing i1 and i2 into a single set.
 *
 * This function merges the two disjoint sets containing i1 and i2 into a
 * single set. It uses the rank of each set to determine which set to merge
 * into the other. If the rank of root1 is less than the rank of root2, root1
 * is merged into root2. If the rank of root2 is less than the rank of root1,
 * root2 is merged into root1. If the ranks are equal, root1 is merged into
 * root2 and the rank of root1 is incremented. The cluster size of the merged
 * set is updated accordingly.
 *
 * @param i1 The index of the first element in the first disjoint set.
 * @param i2 The index of the second element in the second disjoint set.
 */
void DSU::make_union(std::size_t i1, std::size_t i2) {
  // Get the roots of i1 and i2
  auto root1 = get_root(i1);
  auto root2 = get_root(i2);

  // If the roots are already equal, no merge is needed
  if (root1 == root2) {
    return;
  }

  // Merge the smaller rank set into the larger rank set
  if (rank[root1] < rank[root2]) {
    // Merge root1 into root2
    parent[root1] = root2;
    // Update the cluster size of root2
    root_to_cluster_size[root2] += root_to_cluster_size[root1];
  } else if (rank[root2] < rank[root1]) {
    // Merge root2 into root1
    parent[root2] = root1;
    // Update the cluster size of root1
    root_to_cluster_size[root1] += root_to_cluster_size[root2];
  } else {
    // Merge root1 into root2 and increment the rank of root1
    parent[root1] = root2;
    rank[root1]++;
    // Update the cluster size of root2
    root_to_cluster_size[root2] += root_to_cluster_size[root1];
  }
}

/**
 * @brief Returns the cluster size of the set containing the element at index i.
 *
 * This function uses the root of the set containing the element at index i to
 * look up the cluster size in the root_to_cluster_size map.
 *
 * @param i The index of the element for which the cluster size is to be found.
 *
 * @return The cluster size of the set containing the element at index i.
 */
std::size_t DSU::get_cluster_size(std::size_t i) {
  // Get the root of the set containing the element at index i
  std::size_t root = get_root(i);

  // Look up the cluster size of the set containing the element at index i in
  // the root_to_cluster_size map and return it
  return root_to_cluster_size[root];
}

auto DSU::get_cluster_sizes() const { return root_to_cluster_size; }

/**
 * @brief Checks if a given point is out of the field.
 *
 * This function checks if a given point is out of the field by comparing its
 * x-coordinate and y-coordinate with the width and height of the field
 * respectively. If either of the coordinates is greater than or equal to the
 * corresponding dimension of the field, the function returns true, indicating
 * that the point is out of the field.
 *
 * @param point The point to be checked.
 *
 * @return True if the point is out of the field, false otherwise.
 */
bool RGB_Game::out_of_field(const Point &point) {
  // No need to check for < 0 because std::size_t cannot be negative
  // If the x-coordinate is greater than or equal to the width of the field
  // or the y-coordinate is greater than or equal to the height of the field,
  // the point is out of the field, so return true.
  return (point.x() >= FIELD_WIDTH || point.y() >= FIELD_HEIGHT);
}

/**
 * @brief Clusterizes the game field.
 *
 * This function iterates over each element in the game field and performs
 * clustering based on the union-find algorithm. It checks if the element is
 * non-zero and then checks its neighbors to form clusters. If two adjacent
 * elements are the same color, they are unioned together in the union-find
 * data structure.
 *
 * @throws None
 */
void RGB_Game::clusterize_field() {
  // Iterate over each element in the game field
  for (std::size_t i = 0; i < FIELD_HEIGHT; ++i) {
    for (std::size_t j = 0; j < FIELD_WIDTH; ++j) {
      // Skip if the element is zero
      if (!game_field[i][j]) continue;

      // Define a lambda function to check and union adjacent elements
      auto make_union = [&](std::size_t dx, std::size_t dy) {
        // Get the neighboring element, considering out-of-field boundaries
        char neighbour_elem = out_of_field(Point(j + dx, i + dy))
                                  ? 0
                                  : game_field[i + dy][j + dx];
        // If the neighboring element exists and is the same color, union them
        if (neighbour_elem && neighbour_elem == game_field[i][j]) {
          field_dsu.make_union(i * FIELD_WIDTH + j,
                               (i + dy) * FIELD_WIDTH + j + dx);
        }
      };

      // Check and union adjacent elements in different directions
      make_union(0, -1);  // Up
      make_union(1, 0);   // Right
      make_union(0, 1);   // Down
      make_union(-1, 0);  // Left
    }
  }
}

/**
 * @brief Shifts non-zero elements in each row upwards.
 *
 * This function iterates over each column of the game field and shifts non-zero
 * elements upwards until they reach the top of the row.
 *
 * @param game_field The game field to operate on.
 */
void shift_raw_elems(char (&game_field)[FIELD_HEIGHT][FIELD_WIDTH]) {
  std::size_t counter = 0;

  // The maximum number of shifts is FIELD_HEIGHT because each row can have at
  // most FIELD_HEIGHT non-zero elements.
  while (counter < FIELD_HEIGHT) {
    // Iterate over each column of the game field
    for (std::size_t i = 0; i < FIELD_WIDTH; ++i) {
      // Iterate over each element in the current column from the bottom up
      for (std::size_t j = FIELD_HEIGHT - 1; j > 0; --j) {
        // If the current element is zero and the element below it is non-zero,
        // swap them
        if (!game_field[j][i] && game_field[j - 1][i]) {
          std::swap(game_field[j][i], game_field[j - 1][i]);
        }
      }
    }

    // Increment the counter after each iteration
    ++counter;
  }
}

/**
 * @brief Shifts non-zero elements in each column to the left.
 *
 * This function iterates over each column of the game field and shifts non-zero
 * elements to the left until they reach the leftmost position.
 *
 * @param game_field The game field to operate on.
 */
void shift_column_elems(char (&game_field)[FIELD_HEIGHT][FIELD_WIDTH]) {
  /**
   * @brief Checks if a column is empty.
   *
   * This lambda function checks if a column is empty by iterating over each
   * element in the column and returning false if any element is non-zero.
   *
   * @param i The index of the column.
   * @return true if the column is empty, false otherwise.
   */
  auto column_is_empty = [&](std::size_t i) {
    for (std::size_t j = 0; j < FIELD_HEIGHT; ++j) {
      if (game_field[j][i]) {
        return false;
      }
    }
    return true;
  };

  std::size_t counter = 0;
  // The maximum number of shifts is FIELD_WIDTH because we have FIELD_WIDTH
  // amount of columns.
  while (counter < FIELD_WIDTH) {
    // Iterate over each column from the leftmost to the second-to-last column
    for (std::size_t i = 0; i < FIELD_WIDTH - 1; ++i) {
      // If the current column is empty, shift all elements to the left
      if (column_is_empty(i)) {
        for (std::size_t j = 0; j < FIELD_HEIGHT; ++j) {
          std::swap(game_field[j][i], game_field[j][i + 1]);
        }
      }
    }

    // Increment the counter after each iteration
    ++counter;
  }
}

/**
 * @brief Updates the game field by shifting non-zero elements upwards and
 * shifting non-zero elements to the left.
 */
void RGB_Game::update_field() {
  // Shift non-zero elements upwards in each row
  shift_raw_elems(game_field);

  // Shift non-zero elements to the left in each column
  shift_column_elems(game_field);
}

/**
 * @brief Chooses the best move based on the cluster size of non-zero elements.
 *
 * This function iterates over each column and each row from the bottom to the
 * second-to-top row. If a non-zero element is found, it calculates the cluster
 * size of that element using the DSU and updates the best move if the cluster
 * size is greater than the current maximum cluster size.
 *
 * @return The best move as a Point object, where the x-coordinate represents
 * the column index and the y-coordinate represents the row index.
 */
Point RGB_Game::choose_move() {
  // Initialize the best move to (-1, 0) to indicate that no move has been found
  // overflow because of std::size_t
  Point best_move = Point(-1, 0);

  // Initialize the maximum cluster size to 2, which is the minimum cluster size
  std::size_t max_cluster_size = 2;

  // Iterate over each column and each row from the bottom to the second-to-top
  // row this iteration is according to the rules of choosing move from the task
  for (std::size_t j = 0; j < FIELD_WIDTH; ++j) {
    for (std::size_t i = FIELD_HEIGHT - 1; i > 0; --i) {
      // If a non-zero element is found
      if (game_field[i][j]) {
        // Calculate the cluster size of the element using the DSU
        std::size_t cluster_size =
            field_dsu.get_cluster_size(i * FIELD_WIDTH + j);

        // If the cluster size is greater than the current maximum cluster size
        if (cluster_size > max_cluster_size) {
          // Update the maximum cluster size and the best move
          max_cluster_size = cluster_size;
          best_move = Point(j, i);
        }
      }
    }
  }

  // Return the best move
  return best_move;
}

/**
 * @brief Constructs a new RGB_Game object.
 *
 * This constructor initializes the game field with the given field data and
 * performs the clustering operation on the game field.
 *
 * @param field The game field data.
 * @throws std::invalid_argument If any element in the field is not a valid
 *         character.
 */
RGB_Game::RGB_Game(char (&field)[FIELD_HEIGHT][FIELD_WIDTH]) {
  // Iterate over each element in the field and populate the game field
  for (std::size_t i = 0; i < FIELD_HEIGHT; i++) {
    for (std::size_t j = 0; j < FIELD_WIDTH; j++) {
      // Check if the current element is a valid character
      if (!chars_allowed[field[i][j]]) {
        throw std::invalid_argument("Invalid char in field");
      }
      // Populate the game field with the current element
      game_field[i][j] = field[i][j];
    }
  }

  // Initialize the DSU object
  field_dsu = DSU();
  // Perform the clustering operation on the game field
  clusterize_field();
}

auto RGB_Game::get_field() { return game_field; }

/**
 * @brief Plays the game.
 *
 * This function implements the main game loop. It repeatedly calls the
 * choose_move() function to determine the next move, and then calls the
 * make_move() function to make the move. It keeps track of the move count
 * and updates the game state after each move. The game loop continues until
 * the choose_move() function returns a Point object with x=-1.
 *
 * After the game loop exits, the function calculates the final score and
 * writes it to the game log.
 */
void RGB_Game::play() {
  // Initialize the move count
  std::size_t move_count = 1;

  // Game loop
  while (1) {
    // Choose the next move
    Point move = choose_move();

    // Check if the game is over
    if (move == Point(-1, 0)) {
      break;
    }

    // Make the move and update the game state
    make_move(move_count++, move);
    update();
  }

  // Calculate the final score
  std::size_t n_balls = count_balls_on_field();
  total_score = (n_balls == 0) ? total_score + 1000 : total_score;

  // Write the final score to the game log
  game_log << "Final score " << total_score << " with " << n_balls
           << " balls remaining\n";
}

std::string RGB_Game::dumps_log() { return game_log.str(); }

/**
 * @brief Makes a move at the given point and updates the game state.
 *
 * @param move_count The number of moves made so far.
 * @param point The point where the move is made.
 *
 * This function performs the following steps:
 * 1. Finds the color of the ball at the given point.
 * 2. Iterates over the entire game field and checks if the ball at the
 *    current position is part of the same connected component as the ball at
 *    the given point.
 * 3. If a ball is part of the same connected component, it is marked as erased
 *    and the number of erased balls is incremented.
 * 4. Calculates the number of acquired points by subtracting 2 from the total
 *    number of erased balls and then squaring the result.
 * 5. Updates the total score by adding the number of acquired points.
 * 6. Writes a log message to the game log indicating the move that was made
 *    and the number of points gained.
 */
void RGB_Game::make_move(std::size_t move_count, const Point &point) {
  // Initialize the number of erased balls to 0
  std::size_t n_erased = 0;

  // Get the color of the ball at the given point
  char color = game_field[point.y()][point.x()];

  // Iterate over the entire game field
  for (std::size_t i = 0; i < FIELD_HEIGHT; i++) {
    for (std::size_t j = 0; j < FIELD_WIDTH; j++) {
      // Check if the ball at the current position is part of the same
      // connected component as the ball at the given point
      if (field_dsu.get_root(i * FIELD_WIDTH + j) ==
          field_dsu.get_root(point.y() * FIELD_WIDTH + point.x())) {
        // Mark the ball as erased and increment the number of erased balls
        game_field[i][j] = 0;
        n_erased++;
      }
    }
  }

  // Calculate the number of acquired points
  std::size_t acquired_points = (n_erased - 2) * (n_erased - 2);

  // Update the total score by adding the number of acquired points
  total_score += acquired_points;

  // Write a log message to the game log indicating the move that was made and
  // the number of points gained
  // [FIELD_HEIGHT - point.y()] and [point.x() + 1] because of the
  // relation of the coordinate basises in the algorithm and in the task
  game_log << "Move " << move_count << " at (" << FIELD_HEIGHT - point.y()
           << ", " << point.x() + 1 << ")" << ": removed " << n_erased
           << " balls of color " << color << ", got " << acquired_points
           << " points\n";
}

/**
 * @brief Updates the game field and resets the Disjoint Set Union.
 *
 * This function updates the game field by calling the `update_field` method and
 * then resets the Disjoint Set Union by calling the `reset` method of the
 * `field_dsu` object. It also clusterizes the field by calling the
 * `clusterize_field` method.
 *
 * @throws None
 */
void RGB_Game::update() {
  // Update the game field
  update_field();

  // Reset the Disjoint Set Union
  field_dsu.reset();

  // Clusterize the game field
  clusterize_field();
}

/**
 * @brief Counts the number of balls on the game field.
 *
 * This function iterates over each cell of the game field and counts the
 * number of cells that have a non-zero value. It returns the total count of
 * balls on the field.
 *
 * @return The total number of balls on the game field.
 */
std::size_t RGB_Game::count_balls_on_field() {
  std::size_t count = 0;  // Initialize the counter to 0

  // Iterate over each cell of the game field
  for (std::size_t i = 0; i < FIELD_HEIGHT; i++) {
    for (std::size_t j = 0; j < FIELD_WIDTH; j++) {
      // If the current cell has a non-zero value, increment the counter
      if (game_field[i][j]) {
        count++;
      }
    }
  }

  // Return the total count of balls on the field
  return count;
}

}  // namespace RGB_Game