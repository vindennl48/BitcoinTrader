#ifndef AGENT_H
#define AGENT_H

#include "./genetic_algorithm/agent_template.h"
#include "./neural_network/brain.h"
#include "./candles.h"
#include "./bin/mth.h"


extern const int     NUM_NEURONS;
extern const int     NUM_CANDLES;
extern const double  MUTATE_CHANCE;
extern Candles       CANDLES;


struct Trade{
  double in_trade = 0;
  double buy_at   = 0;
  double amount   = 0;
  double sell_at  = 0;
};


struct Agent : public AgentTemplate, public Brain{

  Trade trade;

  inline Agent()
  : AgentTemplate(),
    Brain(NUM_NEURONS){

    fitness = 100.00;
  };

  inline void mutate(){
    if( RAND_UNIFORM <= MUTATE_CHANCE){
      loop(i, length){
        if( RAND_UNIFORM <= MUTATE_CHANCE )
          neurons[i].reset_weight( rand()%length );
      };
    };
  };

  inline void get_fitness(){
    for(auto& candle : CANDLES){
      v(double) v_candle = candle.get_vector();
      input(v_candle);
      run();
      double result = output(1)[0];

      if(result == 1 && trade.in_trade == 0){
        trade.in_trade  = 1;
        trade.buy_at    = candle.close;
        trade.amount    = fitness;
      }
      else if(result == 0 && trade.in_trade == 1){
        trade.in_trade  = 0;
        trade.sell_at   = candle.close;
        double ratio    = trade.sell_at / trade.buy_at;
        fitness         = ratio * trade.amount;
      };
    };

    reset();

  };

  inline bool operator < (const Agent &obj) const {
    return (fitness < obj.fitness); };

  inline thread get_fitness_thread(){
    return std::thread([=] { get_fitness(); } );
  };

};



#endif

