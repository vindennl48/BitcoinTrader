#include "main.h"
#include "definitions.h"
#include "genetics.h"

using namespace std;


// GLOBALS
////////////////////////////////////////////////////////////////////////////////
const int    NUM_OF_CANDLES = 1000;
const int    NUM_OF_NEURONS = 150;
const int    SIZE_OF_POP    = 20;
const double MUTATE_CHANCE  = 0.6;
const double BATTLE_CHANCE  = 0.05;
////////////////////////////////////////////////////////////////////////////////


int main(){
  print("Starting!");

  Genetics ga(SIZE_OF_POP, 4, BATTLE_CHANCE);
  ga.run();

  print("Finished!");
  return 0;
};



