#include <TreasureHunt/TreasureHunt.hpp>
#include <gtest/gtest.h>
#include <vector>

class CalcNumberOfWallsTest : public ::testing::Test {
protected:
  std::vector<Wall> walls;

  void SetUp() override {
    walls = {
        Wall(0, 0, 100, 0),     Wall(0, 0, 0, 100),    Wall(0, 100, 100, 100),
        Wall(100, 0, 100, 100), Wall(20, 0, 37, 100),  Wall(40, 0, 76, 100),
        Wall(85, 0, 0, 75),     Wall(100, 90, 0, 90),  Wall(0, 71, 100, 61),
        Wall(0, 14, 100, 38),   Wall(100, 47, 47, 100)}; // test case from task
  }
  void TearDown() override { walls.clear(); }
};

TEST_F(CalcNumberOfWallsTest, CorrectNumberOfWallsFromCenter) {
  EXPECT_EQ(2, calc_number_of_walls(walls, Point(54.5, 55.4)));
}

// cannot get to the treasure -> std::numeric_limits<std::size_t>::max()
TEST_F(CalcNumberOfWallsTest, InCorrectNumberOfWallsFromTriangleTopCorner) {
  EXPECT_EQ(std::numeric_limits<std::size_t>::max(),
            calc_number_of_walls(walls, Point(70, 87)));
}

TEST_F(CalcNumberOfWallsTest, CorrectNumberOfWallsFromAlmostOutside) {
  EXPECT_EQ(1, calc_number_of_walls(walls, Point(1, 1)));
}

TEST_F(CalcNumberOfWallsTest,
       CorrectNumberOfWallsFromTheSmallestBottomTriangle) {
  EXPECT_EQ(2, calc_number_of_walls(walls, Point(50, 27)));
}

// TODO add more diffcult test cases probably