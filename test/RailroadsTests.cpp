#include <gtest/gtest.h>

#include <Railroads/Railroads.hpp>

TEST(Railroads, IntegrationTestFromTestCase) {
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