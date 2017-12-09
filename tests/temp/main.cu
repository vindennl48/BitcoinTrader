#include "../../mth.h"

// GLOBAL VARS
//#define THREADS   (1<<7)  /* 7=128, 8=256, 9=512, 10=1024 */
//#define CANDLES   2000
#define Na  2        /*num agents*/
#define Nn  (1<<7)   /*num neurons per agent*/
#define Np  (Nn*Nn)  /*num processes per agent*/


struct Neuron{
  cArray<double> sum;
  cArray<double> react;
  cArray<double> weights;
  Neuron():sum(1),react(1),weights(Nn){
    sum[0]   = 1;
    react[0] = 0;
    loop(i, Nn)
      weights[i] = ((double)(rand()%200)-100)/100;
  };
};

struct Agent{
  cArray<Neuron> neurons;
  Agent():neurons(Nn){
    loop(i, Nn)
      neurons[i] = Neuron();
  };
};

struct SB{
  cArray< cArray<double> > neurons;
  SB():neurons(Nn){
    loop(i, Nn){
      neurons[i] = cArray<double>(Nn);
      loop(j, Nn)
        neurons[i][j] = 0;
    };
  };
}; /*sums block*/


#include "kernels.h"
// #include "environment.h"

int main(){

  cArray<SB> sb_all(Na);
  loop(i, Na)
    sb_all[i] = SB();

  cArray<Agent> agents(Na);
  loop(i, Na)
    agents[i] = Agent();

  // agents.send();
  // kernel_prepare<<<Na,Nn>>>(agents.d);
  // kernel_fire<<<(Nn*Na),Nn>>>(agents.d, sb_all.send());
  // sb_all.receive();

  // agents.receive();
  loop(i, Na){
    loop(j, 10)
      nprint(agents[i].neurons[j].sum[0] << ", ");
    print("\n");
    loop(j, 10)
      nprint(agents[i].neurons[j].react[0] << ", ");
    print("\n");
  };

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
