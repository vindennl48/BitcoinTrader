#define USING_CUDA
#include "../../mth.h"

// GLOBAL VARS
#define Na  20       /*num agents*/
#define Nn  (1<<7)   /*128: num neurons per agent*/
#define Np  (Nn*Nn)  /*num processes per agent*/


cArray<double> sums(Na*Nn);
cArray<double> raw_sums(Na*Np);
cArray<double> reacts(Na*Nn);
cArray<double> weights(Na*Np);

__host__ inline void
send_agents(){
  sums.send(); raw_sums.send();
  reacts.send(); weights.send();
};

__host__ inline void
receive_agents(){
  sums.receive(); raw_sums.receive();
  reacts.receive(); weights.receive();
};

__device__ inline void
get_start_points(UINT *itr_rs, UINT *itr_w, const UINT &tidx){
  UINT a  = (UINT)tidx/Np; UINT n = tidx-a*Nn;
  *itr_rs = a*Nn+n;        *itr_w = a*Np+n;
};

__host__ inline void
set_random_agents(){
  loop(i, Na*Nn){
    sums[i]     = 1;
    raw_sums[i] = 0;
    reacts[i]   = 0;
  };
  loop(i, Na*Np)
    weights[i]=((double)(rand()%200)-100)/100;
};

#include "kernels.h"

int main(){
  set_random_agents();
  send_agents();

  loop(i, Nn) nprint(sums[i] << ", ");
  print("\n");

  kernel_prepare<<<Na,Nn>>>(sums.d, reacts.d);
  receive_agents();

  loop(i, Nn) nprint(sums[i] << ", ");
  print("\n");

  // kernel_fire<<<(Nn*Na),Nn>>>(agents.d, raw_sums.d);
  // raw_sums.receive();

  // loop(i, Na){
  //   loop(j, Np){
  //     nprint(sb_all[i].neurons[i][j] << ", ");
  //     PAUSE;
  //   };
  //   print("--------------------------------------------------------");
  // };


////////////////////////////////////////////////////////////////////////////////
  // Environment e(10);
  // int start = mtime();
  // loop(i, 100)
  //   e.run();
  // int end = mtime();
  // e.Print();

  // int time = (start-end);

  // print("Elapsed Time: " << time << "ms");
////////////////////////////////////////////////////////////////////////////////

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
