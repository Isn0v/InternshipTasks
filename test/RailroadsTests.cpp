#include <gtest/gtest.h>

#include <Railroads/Railroads.hpp>

TEST(Railroads, TestCaseAscendingSequence) {
  std::size_t arr[] = {1, 2, 3, 4, 5};
  EXPECT_TRUE(Railroads::can_marshal_coaches(arr, 5));
}

TEST(Railroads, TestCaseFromTask1) {
  std::size_t arr[] = {5, 4, 1, 2, 3};
  EXPECT_FALSE(Railroads::can_marshal_coaches(arr, 5));
}

TEST(Railroads, TestCaseFromTask2) {
  std::size_t arr[] = {6, 5, 4, 3, 2, 1};
  EXPECT_TRUE(Railroads::can_marshal_coaches(arr, 6));
}

TEST(Railroads, IntegrationTest) {
  std::string inp =
      "5\n"
      "1 2 3 4 5\n"
      "5 4 1 2 3\n"
      "0\n"
      "6\n"
      "6 5 4 3 2 1\n"
      "0\n"
      "0\n";

  std::string expected =
      "\n"
      "YES\n"
      "NO\n"
      "\n"
      "YES\n"
      "\n";
  std::string result = Railroads::handle_coaches(inp);
  EXPECT_EQ(result, expected);
}