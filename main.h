
#ifndef MAIN_H
#define MAIN_H

#include "mth.h"
#include "genetics.h"

extern const int    NUM_OF_CANDLES;
extern const int    NUM_OF_NEURONS;
extern const int    SIZE_OF_POP;
extern const double MUTATE_CHANCE;
extern const double BATTLE_CHANCE;

void check_for_pause(int&);
void run_algorithm(Genetics &ga, mth::string&);


#endif
