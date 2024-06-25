// #include <DigitalLab/DigitalLab.hpp>
// #include <TreasureHunt/TreasureHunt.hpp>
#include <RGBGame/RGBGame.hpp>

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <vector>

int main() {
  char field1[FIELD_HEIGHT][FIELD_WIDTH] = {
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

  char field2[FIELD_HEIGHT][FIELD_WIDTH] = {
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

  char field3[FIELD_HEIGHT][FIELD_WIDTH] = {
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

  RGB_Game game(field3);
  game.play();
  std::cout << game.dumps_log();

  return 0;
}