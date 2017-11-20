#include "./agent.h"
#include "./genetic_algorithm/genetics.h"
#include "candles.h"
#include "./bin/mth.h"



// SETTINGS
////////////////////////////////////////////////////////////////////////////////
const int     NUM_NEURONS    = 200;
const int     POPULATION     = 20;
const double  MUTATE_CHANCE  = 0.6;
const double  BATTLE_CHANCE  = 0.05;

const int     MAX_THREADS    = 10;
const int     NUM_CANDLES    = 1000;
Candles       CANDLES;
////////////////////////////////////////////////////////////////////////////////


int main(){
  Candles data_set = load_candles();

  Genetics ga(POPULATION, BATTLE_CHANCE);

  loop(i, 10){
    set_candles(data_set);
    ga.run();
  };

  return 0;
};
