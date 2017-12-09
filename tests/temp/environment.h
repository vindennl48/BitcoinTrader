#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H


#include "../../mth.h"
#include "candles.h"
#include "agent.h"


struct Environment{
  const UINT num_neurons = THREADS;
  const UINT num_candles = CANDLES;
  UINT agentID = 0;
  v(Agent) agents;
  cArray<double> candles;

  Environment(UINT N)
    :candles(num_candles*4)
  {
    get_candles(num_candles, &candles);
    candles.send();
    init_random(N);
  };
  inline void
  init_random(UINT N){
    loop(i, N){
      agents.push_back(
        Agent(agentID,num_neurons,&candles,num_candles)
      );
      agentID+=1;
    };
    loop(i, N)
      agents[i].send();
  };
  inline void
  run(){
    // int start = mtime();
    int size_agents = agents.size();
    loop(i, size_agents)
      agents[i].get_fitness();
    // int end = mtime();
    // return (start-end);
  };
  inline void
  receive(){
    int size_agents = agents.size();
    loop(i, size_agents)
      agents[i].receive();
  };
  inline void
  Print(){
    receive();
    int size_agents = agents.size();
    loop(i, size_agents)
      print("Agent " << i << " fitness: " << agents[i].fitness[0]);
  };
};


#endif
