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

// TEST(Railroads, TestCaseAscendingSequence) {
//   std::size_t arr[] = {1, 2, 3, 4, 5};
//   // we marshal {5, 4, 3, 2, 1} -> {1, 2, 3, 4, 5}
//  //  here always (n - 1) * n / 2 inversions (n = 5) which is 10
//   EXPECT_TRUE(Railroads::can_marshal_coaches(arr, 5));
// }

// TEST(Railroads, TestCaseFromTask1) {
//   std::size_t arr[] = {5, 4, 1, 2, 3};
//   EXPECT_FALSE(Railroads::can_marshal_coaches(arr, 5));
// }

// TEST(Railroads, TestCaseFromTask2) {
//   std::size_t arr[] = {6, 5, 4, 3, 2, 1};
  
//   // we marshal {6, 5, 4, 3, 2, 1} -> {6, 5, 4, 3, 2, 1} so 0 inversions
//   EXPECT_TRUE(Railroads::can_marshal_coaches(arr, 6));
// }

// TEST(Railroads, TestCaseMarshalTrainWith1Coach) {
//   std::size_t arr[] = {1};
//   EXPECT_TRUE(Railroads::can_marshal_coaches(arr, 1));
// }

// TEST(Railroads, TestCaseImpossibleOrder) {
//   std::size_t arr[] = {2, 1, 3};
//   // we marshal {3, 2, 1} -> {2, 1, 3}  (here are 2 inversions, so it's ok)
//   EXPECT_TRUE(Railroads::can_marshal_coaches(arr, 3));
// }

// TEST(Railroads, TestCaseMarshalTrainWith1000CoachesInAscendingSequence) {
//   std::size_t arr[1000];
//   for (std::size_t i = 0; i < 1000; i++) {
//     arr[i] = i + 1;
//   }
//   EXPECT_TRUE(Railroads::can_marshal_coaches(arr, 1000));
// }

// TEST(Railroads, TestCaseMarshalTrainWithAlternatingPattern) {
//   std::size_t arr[] = {2, 1, 4, 3, 6, 5};
//   // we marshal {6, 5, 4, 3, 2, 1} -> {2, 1, 4, 3, 6, 5} (here are 6 inversions)
//   EXPECT_TRUE(Railroads::can_marshal_coaches(arr, 6));
// }

// TEST(Railroads, TestCaseMarshalTrainWith999CoachesInAscendingSequence) {
//   std::size_t arr[999];
//   for (std::size_t i = 0; i < 999; i++) {
//     arr[i] = i + 1;
//   }
//   // for odd number of coaches amount of inversions should be odd (for ascending
//   // sequence)
//   EXPECT_FALSE(Railroads::can_marshal_coaches(arr, 999));
// }

// TEST(Railroads, IntegrationTest) {
//   std::string inp =
//       "5\n"
//       "1 2 3 4 5\n"
//       "5 4 1 2 3\n"
//       "0\n"
//       "6\n"
//       "6 5 4 3 2 1\n"
//       "0\n"
//       "0\n";

//   std::string expected =
//       "\n"
//       "YES\n"
//       "NO\n"
//       "\n"
//       "YES\n"
//       "\n";
//   std::string result = Railroads::handle_coaches(inp);
//   EXPECT_EQ(result, expected);
// }


