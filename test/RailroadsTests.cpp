#include <gtest/gtest.h>

#include <Railroads/Railroads.hpp>
#include <sstream>
#include <fstream>

class RailroadsTest : public ::testing::TestWithParam<int> {};
INSTANTIATE_TEST_SUITE_P(Railroads, RailroadsTest, ::testing::Range(1, 10));

// TODO add more tests if needed
TEST_P(RailroadsTest, IntegrationTest) {
  int num_test = GetParam();
  std::stringstream ss_in, ss_exp;

  ss_in << CMAKE_PROJECT_SOURCE_DIR << "/test/data/Railroads/input_"
        << num_test << ".txt";
  ss_exp << CMAKE_PROJECT_SOURCE_DIR << "/test/data/Railroads/expected_"
         << num_test << ".txt";

  std::ifstream in(ss_in.str());
  std::ifstream exp(ss_exp.str());
  std::ostringstream expected;
  expected << exp.rdbuf();

  if (!exp.is_open() || !in.is_open()) {
    FAIL() << "Failed to open expected output file";
  }

  EXPECT_EQ(Railroads::handle_coaches(in), expected.str());
}