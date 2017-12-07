#ifndef AGENT_H
#define AGENT_H


// Candle Definitions
#define HIGH          0
#define LOW           1
#define OPEN          2
#define CLOSE         3

// Trade Definitions
#define IN_TRADE      0
#define BUY_AT        1
#define SELL_AT       2
#define AMOUNT        3

#define AGENT_RANDOM  2


#include "../../mth.h"
#include "Kernels.h"


struct Agent{
  UINT           agentID;
  UINT           num_neurons;
  UINT           num_candles;
  cArray<double> trade;
  cArray<double> sums;
  cArray<double> reacts;
  cArray<double> weights;
  cArray<double> fitness;
  cArray<double> *candles;

  inline 
  Agent(UINT ID, UINT num_neurons, cArray<double> *in_candles, UINT num_candles, UINT options=2)
    :sums   (cArray<double>(num_neurons)),
     reacts (cArray<double>(num_neurons)),
     weights(cArray<double>(pow(num_neurons,2))),
     fitness(cArray<double>(1)),
     trade  (cArray<double>(4)),
     num_neurons(num_neurons),
     num_candles(num_candles),
     agentID(ID)
  {
    candles = in_candles;
    if(options == AGENT_RANDOM){

      /* create a random brain */
      loop(i, num_neurons){
        sums.h[i]   = 0;
        reacts.h[i] = 0;
      };
      loop(i, pow(num_neurons,2))
        weights.h[i] = ((double)(rand()%200)-100)/100;

      *fitness.h = 100.00;
      loop(i, 4)
        trade.h[i] = 0;
    }
  };

  inline void 
  send(){
    sums.send();
    reacts.send();
    weights.send();
    trade.send();
    fitness.send();
  };

  inline void 
  receive(){
    //sums.receive();
    fitness.receive();
  };

  inline void
  get_fitness(){
    bool sw = false;
    loop(i, num_candles){
      kernel_prepare<<<1,num_neurons>>>(
        sums.d,
        reacts.d,
        candles->d,
        i, 4
      );
      kernel_fire<<<num_neurons,num_neurons>>>(
        sums.d,
        reacts.d,
        weights.d
      );
      kernel_get_fit<<<1,1>>>(
        sums.d,
        fitness.d,
        trade.d,
        candles->d,
        num_neurons,
        i, 4
      );
      fitness.receive();
      trade.receive();
      nprint("Candle Close: " << candles->h[i*4+CLOSE] << " | ");
      print("fitness: " << *fitness.h << ", " << trade.h[0] << ", " << trade.h[1] 
        << ", "  << trade.h[2] << ", "  << trade.h[3]);
      if(sw)
        PAUSE;
      if(*fitness.h <=0){
        print("Function lost all money");
        sw = true;
      };
    };
  };

  // inline
  // Agent(const Agent &source)
  //   :sums   (cArray<double>(source.num_neurons)),
  //    reacts (cArray<double>(source.num_neurons)),
  //    weights(cArray<double>(source.weights)),
  //    fitness(cArray<double>(1)),
  //    trade  (cArray<double>(4)),
  //    candles(source.candles),
  //    num_neurons(source.num_neurons),
  //    num_candles(source.num_candles),
  //    agentID(source.agentID)
  // {
  //   loop(i, 4)
  //     trade.h[i] = 0;
  //   loop(i, num_neurons){
  //     sums.h[i]   = 0;
  //     reacts.h[i] = 0;
  //   };
  //   *fitness.h = 100;
  // };

};



#endif
