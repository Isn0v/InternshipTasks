#include <gtest/gtest.h>

#include <TreasureHunt/TreasureHunt.hpp>
#include <vector>

class TreasureHuntTestSuite : public ::testing::Test {
 protected:
  std::vector<Treasure_Hunt::Wall> walls;

  void SetUp() override {
    using namespace Treasure_Hunt;

    walls = {
        Wall(0, 0, 100, 0),     Wall(0, 0, 0, 100),    Wall(0, 100, 100, 100),
        Wall(100, 0, 100, 100), Wall(20, 0, 37, 100),  Wall(40, 0, 76, 100),
        Wall(85, 0, 0, 75),     Wall(100, 90, 0, 90),  Wall(0, 71, 100, 61),
        Wall(0, 14, 100, 38),   Wall(100, 47, 47, 100)};  // test case from task
  }
  void TearDown() override { walls.clear(); }
};

TEST_F(TreasureHuntTestSuite, CorrectNumberOfWallsFromCenter) {
  EXPECT_EQ(2, Treasure_Hunt::calc_number_of_doors(
                   walls, Treasure_Hunt::Point(54.5, 55.4)));
}

// cannot get to the treasure -> std::numeric_limits<std::size_t>::max()
TEST_F(TreasureHuntTestSuite,
       IncorrectNumberOfWallsFromTriangleTopCorner) {
  EXPECT_EQ(
      std::numeric_limits<std::size_t>::max(),
      Treasure_Hunt::calc_number_of_doors(walls, Treasure_Hunt::Point(70, 87)));
}

TEST_F(TreasureHuntTestSuite, CorrectNumberOfWallsFromAlmostOutside) {
  EXPECT_EQ(1, Treasure_Hunt::calc_number_of_doors(walls,
                                                   Treasure_Hunt::Point(1, 1)));
}

TEST_F(TreasureHuntTestSuite,
       CorrectNumberOfWallsFromTheSmallestBottomTriangle) {
  EXPECT_EQ(2, Treasure_Hunt::calc_number_of_doors(
                   walls, Treasure_Hunt::Point(50, 27)));
}

TEST(TreasureHuntTest, IntergrationTest) {
  std::string input =
      "7\n"
      "20 0 37 100\n"
      "40 0 76 100\n"
      "85 0 0 75\n"
      "100 90 0 90\n"
      "0 71 100 61\n"
      "0 14 100 38\n"
      "100 47 47 100\n"
      "54.5 55.4\n";

  std::string expected = "Number of doors = 2\n";

  EXPECT_EQ(expected, Treasure_Hunt::handle_treasure_hunt(input));
}

TEST(TreasureHuntTest, IncorrectIntergrationTest) {
  std::string input =
      "7\n"
      "20 0 37 100\n"
      "40 0 76 100\n"
      "85 0 0 75\n"
      "100 90 0 90\n"
      "0 71 100 61\n"
      "0 14 100 38\n"
      "100 47 47 100\n"
      "70 87\n";

  std::string expected = "Impossible to get to the given treasure point\n";

  EXPECT_EQ(expected, Treasure_Hunt::handle_treasure_hunt(input));
}