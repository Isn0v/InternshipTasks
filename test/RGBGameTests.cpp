#include <gtest/gtest.h>

#include <RGBGame/RGBGame.hpp>
#include <fstream>
#include <sstream>

class RGBGameTest : public ::testing::TestWithParam<int> {};
INSTANTIATE_TEST_SUITE_P(RGBGame, RGBGameTest, ::testing::Range(1, 5));

TEST_P(RGBGameTest, IntegrationTest) {
  int num_test = GetParam();
  std::stringstream ss_in, ss_exp;

  ss_in << CMAKE_PROJECT_SOURCE_DIR << "/test/data/RGBGame/input_" << num_test
        << ".txt";
  ss_exp << CMAKE_PROJECT_SOURCE_DIR << "/test/data/RGBGame/expected_"
         << num_test << ".txt";

  std::ifstream in(ss_in.str());
  std::ifstream exp(ss_exp.str());
  std::ostringstream expected;
  expected << exp.rdbuf();

  if (!exp.is_open() || !in.is_open()) {
    FAIL() << "Failed to open expected output file";
  }

  EXPECT_EQ(RGB_Game::handle_rgb_game(in), expected.str());
}