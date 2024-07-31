#include <gtest/gtest.h>

#include <TreasureHunt/TreasureHunt.hpp>
#include <vector>
#include <sstream>
#include <fstream>

class TreasureHuntTest : public ::testing::TestWithParam<int> {};
INSTANTIATE_TEST_SUITE_P(TreasureHunt, TreasureHuntTest, ::testing::Range(1, 12));

TEST_P(TreasureHuntTest, IntegrationTest) {
  int num_test = GetParam();
  std::stringstream ss_in, ss_exp;

  ss_in << CMAKE_PROJECT_SOURCE_DIR << "/test/data/TreasureHunt/input_" << num_test
        << ".txt";
  ss_exp << CMAKE_PROJECT_SOURCE_DIR << "/test/data/TreasureHunt/expected_"
         << num_test << ".txt";

  std::ifstream in(ss_in.str());
  std::ifstream exp(ss_exp.str());
  std::ostringstream expected;
  expected << exp.rdbuf();

  if (!exp.is_open() || !in.is_open()) {
    FAIL() << "Failed to open expected output file";
  }

  EXPECT_EQ(Treasure_Hunt::handle_treasure_hunt(in), expected.str());
}