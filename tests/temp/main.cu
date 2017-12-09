

// GLOBAL VARS
#define THREADS   (1<<7)  /* 7=128, 8=256, 9=512, 10=1024 */
#define CANDLES   2000


#include "../../mth.h"
#include "environment.h"

int main(){

  Environment e(10);
  int start = mtime();
  loop(i, 100)
    e.run();
  int end = mtime();
  e.Print();

  int time = (start-end);

  print("Elapsed Time: " << time << "ms");

  // const UINT num_neurons = THREADS;
  // const UINT num_candles = 10000;
  // UINT agentID = 0;
  // v(Agent) agents;

  // cArray<double> candles(num_candles*4);
  // get_candles(num_candles, &candles);
  // candles.send();

  // int start = mtime();
  // loop(i, 10){
  //   agents.push_back(Agent(agentID, num_neurons, &candles, num_candles));
  //   agentID+=1;
  //   agents.back().send();
  //   agents.back().get_fitness();
  //   agents.back().receive();
  //   print("Agent " << i << " fitness: " << agents.back().fitness[0]);
  // };
  // int end = mtime();

  // loop(i, 10){
  //   print("Agent " << i << " fitness: " << agents[i].fitness[0]);
  // };

  // print("Time Elapsed: " << (start-end) << "ms, " << (double)(100*(start-end))/1000/60 << "min/100gen");

  return 0;
}
