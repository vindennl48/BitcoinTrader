

// GLOBAL VARS
#define THREADS   256


#include "../../mth.h"
#include "candles.h"
#include "agent.h"

int main(){

  PAUSE;

  const UINT num_neurons = THREADS;
  const UINT num_candles = 10000;
  UINT agentID = 0;

  cArray<double> candles(num_candles*4);
  get_candles(num_candles, &candles);

  Agent agent1(agentID, num_neurons, &candles, num_candles);
  agentID+=1;
  Agent agent2(agentID, num_neurons, &candles, num_candles);
  agentID+=1;

  int start = mtime();

  agent1.send(); candles.send();
  agent2.send();
  agent1.get_fitness();
  agent2.get_fitness();
  agent1.receive();
  agent2.receive();

  int end = mtime();

  print("Agent1 fitness: " << *(agent1.fitness.h));
  print("Agent2 fitness: " << *(agent2.fitness.h) << "\n\n");

  print("Time Elapsed: " << (start-end) << "ms, " << (double)(20*(start-end))/1000 << "sec/20");

  return 0;
}
