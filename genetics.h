#ifndef GENETICS_H
#define GENETICS_H

#include <vector>
#include <thread>
#include "brain.h"


// GLOBALS
////////////////////////////////////////////////////////////////////////////////
extern const int    NUM_OF_CANDLES;
extern const int    NUM_OF_NEURONS;
extern const double MUTATE_CHANCE;
////////////////////////////////////////////////////////////////////////////////



#define CANDLES  std::vector< std::vector<double> > 
#define CANDLE   std::vector<double>
#define HIGH     0
#define LOW      1
#define OPEN     2
#define CLOSE    3

#define TRADE    std::vector<double>
#define IN_TRADE 0
#define BUY_AT   1
#define AMOUNT   2
#define SELL_AT  3

#define AGENTS  std::vector<Agent>
#define AGENT   Agent

class Agent{

  static int agent_id;
  static CANDLES candles;

public:
  int    ID;
  double fitness, bank;
  BRAIN  dna;
  TRADE  trade;

  Agent();
  int    get_id();
  void   mutate();
  void   get_fitness();

  std::thread get_fitness_thread();
  std::string get_str();

//  inline Agent& operator = (const Agent &obj) {};
  inline bool   operator < (const Agent &obj) const {
    return (fitness < obj.fitness);
  };

};



class Genetics{

public:
  int     population, num_of_children;
  double  chance_amount;
  int time_start, time_average;
  AGENTS agents;

  Genetics(int, int, double);
  void  run();
  void  create_new_generation();
  Agent best_fit_selection();
  void  fitness();
  void  remove_agent(AGENT &);

  std::string get_time(int, int);
  std::string get_str();

  std::string print_output();

};



CANDLES get_candles(int);



#endif
