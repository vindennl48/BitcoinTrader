

// GLOBAL VARS
#define THREADS   256


#include "../../mth.h"
#include "candles.h"
#include "agent.h"

int main(){

  const UINT num_neurons = THREADS;
  const UINT num_candles = 10000;

  cArray<double> candles(num_candles*4);
  get_candles(num_candles, &candles);

  Agent agent(/*ID*/0, num_neurons, &candles, num_candles);

  int start = mtime();

  agent.send(); candles.send();
  agent.get_fitness();
  agent.receive();

  int end = mtime();

  print("fitness: " << *(agent.fitness.h) << "\n\n");

  print("Time Elapsed: " << (start-end) << "ms, " << (double)(20*(start-end))/1000 << "sec/20");

  return 0;
}
