#include <gtest/gtest.h>

#include <RGBGame/RGBGame.hpp>
#include <string>

TEST(RGBGameTests, RGBGameWithNoPatterns) {
  char field[FIELD_HEIGHT][FIELD_WIDTH] = {
      {'R', 'G', 'G', 'B', 'B', 'G', 'G', 'R', 'B', 'R', 'R', 'G', 'G', 'B',
       'G'},
      {'R', 'B', 'G', 'R', 'B', 'G', 'R', 'B', 'G', 'R', 'B', 'G', 'R', 'B',
       'G'},
      {'R', 'R', 'R', 'R', 'G', 'B', 'B', 'B', 'R', 'G', 'G', 'R', 'B', 'B',
       'B'},
      {'G', 'G', 'R', 'G', 'B', 'G', 'G', 'B', 'R', 'R', 'G', 'G', 'G', 'B',
       'G'},
      {'G', 'B', 'G', 'G', 'R', 'R', 'R', 'R', 'R', 'B', 'G', 'G', 'R', 'R',
       'R'},
      {'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B',
       'B'},
      {'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B',
       'B'},
      {'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R',
       'R'},
      {'R', 'R', 'R', 'R', 'R', 'G', 'G', 'G', 'G', 'R', 'R', 'R', 'R', 'R',
       'R'},
      {'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G',
       'G'}};

  std::string result =
      "Move 1 at (4, 1): removed 32 balls of color B, got 900 points\n"
      "Move 2 at (2, 1): removed 39 balls of color R, got 1369 points\n"
      "Move 3 at (1, 1): removed 37 balls of color G, got 1225 points\n"
      "Move 4 at (3, 4): removed 11 balls of color B, got 81 points\n"
      "Move 5 at (1, 1): removed 8 balls of color R, got 36 points\n"
      "Move 6 at (2, 1): removed 6 balls of color G, got 16 points\n"
      "Move 7 at (1, 6): removed 6 balls of color B, got 16 points\n"
      "Move 8 at (1, 2): removed 5 balls of color R, got 9 points\n"
      "Move 9 at (1, 2): removed 5 balls of color G, got 9 points\n"
      "Final score 3661 with 1 balls remaining\n";

  RGB_Game::RGB_Game game(field);
  game.play();

  EXPECT_EQ(game.dumps_log(), result);
}

TEST(RGBGameTests, RGBGameWithPairOfEqualRows) {
  char field[FIELD_HEIGHT][FIELD_WIDTH] = {
      {'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R',
       'R'},
      {'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R',
       'R'},
      {'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G',
       'G'},
      {'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G',
       'G'},
      {'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B',
       'B'},
      {'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B',
       'B'},
      {'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R',
       'R'},
      {'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R',
       'R'},
      {'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G',
       'G'},
      {'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G',
       'G'}};

  std::string result =
      "Move 1 at (1, 1): removed 30 balls of color G, got 784 points\n"
      "Move 2 at (1, 1): removed 30 balls of color R, got 784 points\n"
      "Move 3 at (1, 1): removed 30 balls of color B, got 784 points\n"
      "Move 4 at (1, 1): removed 30 balls of color G, got 784 points\n"
      "Move 5 at (1, 1): removed 30 balls of color R, got 784 points\n"
      "Final score 4920 with 0 balls remaining\n";

  RGB_Game::RGB_Game game(field);
  game.play();

  EXPECT_EQ(game.dumps_log(), result);
}

TEST(RGBGameTests, RGBGameWithNoClusters) {
  char field[FIELD_HEIGHT][FIELD_WIDTH] = {
      {'R', 'B', 'G', 'R', 'B', 'G', 'R', 'B', 'G', 'R', 'B', 'G', 'R', 'B',
       'G'},
      {'B', 'G', 'R', 'B', 'G', 'R', 'B', 'G', 'R', 'B', 'G', 'R', 'B', 'G',
       'R'},
      {'G', 'R', 'B', 'G', 'R', 'B', 'G', 'R', 'B', 'G', 'R', 'B', 'G', 'R',
       'B'},
      {'R', 'B', 'G', 'R', 'B', 'G', 'R', 'B', 'G', 'R', 'B', 'G', 'R', 'B',
       'G'},
      {'B', 'G', 'R', 'B', 'G', 'R', 'B', 'G', 'R', 'B', 'G', 'R', 'B', 'G',
       'R'},
      {'G', 'R', 'B', 'G', 'R', 'B', 'G', 'R', 'B', 'G', 'R', 'B', 'G', 'R',
       'B'},
      {'R', 'B', 'G', 'R', 'B', 'G', 'R', 'B', 'G', 'R', 'B', 'G', 'R', 'B',
       'G'},
      {'B', 'G', 'R', 'B', 'G', 'R', 'B', 'G', 'R', 'B', 'G', 'R', 'B', 'G',
       'R'},
      {'G', 'R', 'B', 'G', 'R', 'B', 'G', 'R', 'B', 'G', 'R', 'B', 'G', 'R',
       'B'},
      {'R', 'B', 'G', 'R', 'B', 'G', 'R', 'B', 'G', 'R', 'B', 'G', 'R', 'B',
       'G'}};

  std::string result = "Final score 0 with 150 balls remaining\n";

  RGB_Game::RGB_Game game(field);
  game.play();

  EXPECT_EQ(game.dumps_log(), result);
}

TEST(RGBGameTests, IntegrationTest) {
  std::string inp =
      "3\n"
      "RGGBBGGRBRRGGBG\n"
      "RBGRBGRBGRBGRBG\n"
      "RRRRGBBBRGGRBBB\n"
      "GGRGBGGBRRGGGBG\n"
      "GBGGRRRRRBGGRRR\n"
      "BBBBBBBBBBBBBBB\n"
      "BBBBBBBBBBBBBBB\n"
      "RRRRRRRRRRRRRRR\n"
      "RRRRRRGGGGRRRRR\n"
      "GGGGGGGGGGGGGGG\n"
      "\n"
      "RRRRRRRRRRRRRRR\n"
      "RRRRRRRRRRRRRRR\n"
      "GGGGGGGGGGGGGGG\n"
      "GGGGGGGGGGGGGGG\n"
      "BBBBBBBBBBBBBBB\n"
      "BBBBBBBBBBBBBBB\n"
      "RRRRRRRRRRRRRRR\n"
      "RRRRRRRRRRRRRRR\n"
      "GGGGGGGGGGGGGGG\n"
      "GGGGGGGGGGGGGGG\n"
      "\n"
      "RBGRBGRBGRBGRBG\n"
      "BGRBGRBGRBGRBGR\n"
      "GRBGRBGRBGRBGRB\n"
      "RBGRBGRBGRBGRBG\n"
      "BGRBGRBGRBGRBGR\n"
      "GRBGRBGRBGRBGRB\n"
      "RBGRBGRBGRBGRBG\n"
      "BGRBGRBGRBGRBGR\n"
      "GRBGRBGRBGRBGRB\n"
      "RBGRBGRBGRBGRBG\n"
      "\n";

  std::string expected =
      "Game 1:\n"
      "Move 1 at (4, 1): removed 32 balls of color B, got 900 points\n"
      "Move 2 at (2, 1): removed 39 balls of color R, got 1369 points\n"
      "Move 3 at (1, 1): removed 37 balls of color G, got 1225 points\n"
      "Move 4 at (3, 4): removed 11 balls of color B, got 81 points\n"
      "Move 5 at (1, 1): removed 8 balls of color R, got 36 points\n"
      "Move 6 at (2, 1): removed 6 balls of color G, got 16 points\n"
      "Move 7 at (1, 6): removed 6 balls of color B, got 16 points\n"
      "Move 8 at (1, 2): removed 5 balls of color R, got 9 points\n"
      "Move 9 at (1, 2): removed 5 balls of color G, got 9 points\n"
      "Final score 3661 with 1 balls remaining\n"
      "\n"
      "Game 2:\n"
      "Move 1 at (1, 1): removed 30 balls of color G, got 784 points\n"
      "Move 2 at (1, 1): removed 30 balls of color R, got 784 points\n"
      "Move 3 at (1, 1): removed 30 balls of color B, got 784 points\n"
      "Move 4 at (1, 1): removed 30 balls of color G, got 784 points\n"
      "Move 5 at (1, 1): removed 30 balls of color R, got 784 points\n"
      "Final score 4920 with 0 balls remaining\n"
      "\n"
      "Game 3:\n"
      "Final score 0 with 150 balls remaining\n"
      "\n";

  EXPECT_EQ(RGB_Game::handle_rgb_game(inp), expected);
}

TEST(RGBGameTests, IntegrationTestWithAllEdgeCases) {
  std::string inp =
      "5\n"
      "RGGGGGGGGGGGGGG\n"
      "GGGGGGGGGGGGGGG\n"
      "GGGGGGGGGGGGGGG\n"
      "GGGGGGGGGGGGGGG\n"
      "GGGGGGGGGGGGGGG\n"
      "GGGGGGGGGGGGGGG\n"
      "GGGGGGGGGGGGGGG\n"
      "GGGGGGGGGGGGGGG\n"
      "GGGGGGGGGGGGGGG\n"
      "GGGGGGGGGGGGGGG\n"
      "\n"
      "GGGGGGGGGGGGGGG\n"
      "GGGGGGGGGGGGGGG\n"
      "GGGGGGGGGGGGGGG\n"
      "GGGGGGGGGGGGGGG\n"
      "GGGGGGGGGGGGGGG\n"
      "GGGGGGGGGGGGGGG\n"
      "GGGGGGGGGGGGGGG\n"
      "GGGGGGGGGGGGGGG\n"
      "GGGGGGGGGGGGGGG\n"
      "GGGGGGGGGGGGGGG\n"
      "\n"
      "RRRGGGGBGGGGRRR\n"
      "RRRGGGGBGGGGRRR\n"
      "RRRGGGGBGGGGRRR\n"
      "RRRGGGGBGGGGRRR\n"
      "RRRGGGGBGGGGRRR\n"
      "GGGGGGGBGGGGGGG\n"
      "GGGGGGGBGGGGGGG\n"
      "GGGGGGGBGGGGGGG\n"
      "GGGGGGGBGGGGGGG\n"
      "GGGGGGGBGGGGGGG\n"
      "\n"
      "RRRRRRRBGGGGGGG\n"
      "RRRRRRRBGGGGGGG\n"
      "RRRRRRRBGGGGGGG\n"
      "RRRRRRRBGGGGGGG\n"
      "RRRRRRRBGGGGGGG\n"
      "GGGGGGGBRRRRRRR\n"
      "GGGGGGGBRRRRRRR\n"
      "GGGGGGGBRRRRRRR\n"
      "GGGGGGGBRRRRRRR\n"
      "GGGGGGGBRRRRRRR\n"
      "\n"
      "GGGGGGGBGGGGGGG\n"
      "GGGGGGGBGGGGGGG\n"
      "GGGGGGGBGGGGGGG\n"
      "GGGGGGGBGGGGGGG\n"
      "BBBBBBBBGGGGGGG\n"
      "BBBBBBBBGGGGGGG\n"
      "RRRRRRRRGGGGGGG\n"
      "RRRRRRRRGGGGGGG\n"
      "RRRRRRRRGGGGGGG\n"
      "RRRRRRRRGGGGGGG\n";

  std::string expected =
      "Game 1:\n"
      "Move 1 at (1, 1): removed 149 balls of color G, got 21609 points\n"
      "Final score 21609 with 1 balls remaining\n"
      "\n"
      "Game 2:\n"
      "Move 1 at (1, 1): removed 150 balls of color G, got 21904 points\n"
      "Final score 22904 with 0 balls remaining\n"
      "\n"
      "Game 3:\n"
      "Move 1 at (1, 1): removed 55 balls of color G, got 2809 points\n"
      "Move 2 at (1, 5): removed 55 balls of color G, got 2809 points\n"
      "Move 3 at (1, 1): removed 15 balls of color R, got 169 points\n"
      "Move 4 at (1, 2): removed 15 balls of color R, got 169 points\n"
      "Move 5 at (1, 1): removed 10 balls of color B, got 64 points\n"
      "Final score 7020 with 0 balls remaining\n"
      "\n"
      "Game 4:\n"
      "Move 1 at (1, 1): removed 35 balls of color G, got 1089 points\n"
      "Move 2 at (1, 1): removed 35 balls of color R, got 1089 points\n"
      "Move 3 at (1, 2): removed 35 balls of color R, got 1089 points\n"
      "Move 4 at (1, 2): removed 35 balls of color G, got 1089 points\n"
      "Move 5 at (1, 1): removed 10 balls of color B, got 64 points\n"
      "Final score 5420 with 0 balls remaining\n"
      "\n"
      "Game 5:\n"
      "Move 1 at (1, 9): removed 70 balls of color G, got 4624 points\n"
      "Move 2 at (1, 1): removed 32 balls of color R, got 900 points\n"
      "Move 3 at (3, 1): removed 28 balls of color G, got 676 points\n"
      "Move 4 at (1, 1): removed 20 balls of color B, got 324 points\n"
      "Final score 7524 with 0 balls remaining\n"
      "\n";

  EXPECT_EQ(RGB_Game::handle_rgb_game(inp), expected);
}