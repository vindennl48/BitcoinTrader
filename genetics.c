#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>
#include <thread>
#include "genetics.h"
//#include "definitions.h"
#include "mth.h"

using namespace std;


// ITEM LIST
////////////////////////////////////////////////////////////////////////////////
// AGENT CLASS
////////////////////////////////////////////////////////////////////////////////
//   Agent::agent_id
//   Agent::candles
//   Agent::Agent
//   Agent::get_id
//   Agent::get_str
//   Agent::mutate
//   Agent::get_fitness
//   Agent::get_fitness_thread
////////////////////////////////////////////////////////////////////////////////
// GENETICS CLASS
////////////////////////////////////////////////////////////////////////////////
//   Genetics::Genetics
//   Genetics::run
//   Genetics::fitness
//   Genetics::create_new_generation
//   Genetics::remove_agent
//   Genetics::get_str
//   Genetics::get_time
//   Genetics::best_fit_selection
////////////////////////////////////////////////////////////////////////////////
// FUNCTIONS
////////////////////////////////////////////////////////////////////////////////
//   CANDLES get_candles
////////////////////////////////////////////////////////////////////////////////


// AGENT CLASS
////////////////////////////////////////////////////////////////////////////////
int     Agent::agent_id = 0;
CANDLES Agent::candles  = get_candles(NUM_OF_CANDLES);



Agent::Agent(){

  ID      = get_id();
  fitness = -1;
  dna     = brain::create(NUM_OF_NEURONS);
  bank    = 100.0;
  trade   = { 0, 0, 0, 0 };

};

int Agent::get_id(){

  int new_id = Agent::agent_id;
  agent_id  += 1;

  return new_id;

};

string Agent::get_str(){

  string result;

  result += " - ID: " + to_string(ID);
  result += ", My Fitness: " + mth::dbl_to_s(fitness, 2);

  return result;

};

void Agent::mutate(){

  double chance_amount = MUTATE_CHANCE;

  if( RAND_UNIFORM <= chance_amount){

    int len_dna = dna.size();
    loop(i,dna.size()){
      if( RAND_UNIFORM <= chance_amount){
        neuron::reset_weights(dna.at(i));
      };
    };

  };

};

void Agent::get_fitness(){


  if(fitness == -1){

    fitness = 0;

    int len_candles = candles.size();
    loop(i, len_candles){


      CANDLE *candle = &candles.at(i);
      brain::input(dna, *candle);
      brain::run(dna, 2);
      double output = (brain::output(dna, 1)).at(0);
      
      if(output == 1 && trade.at(IN_TRADE) == 0){
        trade.at(IN_TRADE) = 1;
        trade.at(BUY_AT)   = (*candle).at(CLOSE);
        trade.at(AMOUNT)   = bank;
      }
      else if(output == 0 && trade.at(IN_TRADE) == 1){
        trade.at(IN_TRADE) = 0;
        trade.at(SELL_AT)  = (*candle).at(CLOSE);
        double ratio       = trade.at(SELL_AT)/trade.at(BUY_AT);
        bank               = ratio * trade.at(AMOUNT);
      };

      if(bank < 10){
        break;
      };

    };

    fitness = bank;

  };
    
};



std::thread Agent::get_fitness_thread(){
  return std::thread([=] { get_fitness(); } );
};
////////////////////////////////////////////////////////////////////////////////



// GENETICS CLASS
////////////////////////////////////////////////////////////////////////////////
Genetics::Genetics(int pop, int num_children, double chance){

  population      = pop;
  num_of_children = num_children;
  chance_amount   = chance;

  // Time Vars
  time_start      = mth::mtime();
  time_average    = 0;
  num_of_children = num_of_children;

  loop(i, pop){ agents.push_back(Agent()); };

};



void Genetics::run(){

  print(" - Running Algorithm");

  int start, end;

  int generation = 0;
  while(true){

    start = mth::mtime();
    //print("Start: " << start);
    //PAUSE;

    // print(" - Starting fitness");
  
    fitness();

    system("clear;clear");
    print(" # Generation: " << generation);
    print(get_str());
    // print(" - Creating New Generation");

    create_new_generation();

    end = mth::mtime();

    print(get_time(start,end));

    generation += 1;

  };

};



void Genetics::fitness(){

    int len_agents = agents.size();
    vector<thread> threads;

    loop(i, len_agents){ 
      threads.push_back(
        thread(
          agents.at(i).get_fitness_thread()
        )
      );
    };

    for(auto& t : threads) t.join();

    sort(agents.begin(), agents.end());
    reverse(agents.begin(), agents.end());

};



void Genetics::create_new_generation(){

  AGENTS result, offspring, successors;
  
  successors.push_back(agents.at(0));

  loop(i,(int)(population/(num_of_children+2))){

    AGENT parent1, parent2, child1, child2;
    int split;

    parent1 = best_fit_selection();
    parent2 = best_fit_selection();

    successors.push_back(parent1);
    successors.push_back(parent2);

    loop(j,(num_of_children/2)){

      if(parent1.dna.size() > parent2.dna.size()){
        split = rand()%parent2.dna.size();
      }
      else{
        split = rand()%parent1.dna.size();
      };

      child1 = Agent();
      child1.dna.clear();

      child1.dna.insert(
        child1.dna.end(),
        parent1.dna.begin(),
        parent1.dna.begin() + split
      );
      child1.dna.insert(
        child1.dna.end(),
        parent2.dna.begin() + split,
        parent2.dna.end()
      );

      child2 = Agent();
      child2.dna.clear();

      child2.dna.insert(
        child2.dna.end(),
        parent2.dna.begin(),
        parent2.dna.begin() + split
      );
      child2.dna.insert(
        child2.dna.end(),
        parent1.dna.begin() + split,
        parent1.dna.end()
      );

      offspring.push_back(child1);
      offspring.push_back(child2);

    };

  };

  int len_offspring = offspring.size();
  loop(i,len_offspring){ offspring.at(i).mutate(); };

  result.insert(
    result.end(),
    successors.begin(),
    successors.end()
  );
  result.insert(
    result.end(),
    offspring.begin(),
    offspring.end()
  );

  agents.clear();
  agents.insert(
    agents.end(),
    result.begin(),
    result.end()
  );

};



void Genetics::remove_agent(AGENT &remove){

  int len_agents = agents.size();
  loop(i, len_agents){

    AGENT *agent = &agents.at(i);

    if ((*agent).ID == remove.ID){
      agents.erase(agents.begin()+i);
      break;
    };

  };

};



string Genetics::get_str(){

  int len_agents;
  string result;

  len_agents = agents.size();
  loop(i,10){
    AGENT *agent = &agents.at(i);
    result += (*agent).get_str() + "\n";
  };

  return result;

};



string Genetics::get_time(int start, int end){

  print("Start: " << start << ", End: " << end );

  int latest, elapsed, estimated;
  string result;

  latest       = start-end;
  time_average = (time_average + latest)/2;
  elapsed      = (time_start - end);
  estimated    = (time_average*100);
  
  string a,b,c;
  a = mth::dbl_to_s(   ((double)latest/1000),2);
  b = mth::dbl_to_s(  ((double)elapsed/1000),2);
  c = mth::dbl_to_s(((double)estimated/1000),2);

  print("A: " << a << ", B: " << b <<  ", C: " << c);

  result += " +\n";
  result += " Latest Stopwatch: " + a + "sec, ";
  result +=           "Elapsed: " + b + "sec, ";
  result +=        "Est.100Gen: " + c + "sec";
  result += "\n +\n";

  return result;

};




Agent Genetics::best_fit_selection(){

  AGENT a1, a2;
  int   winner;


  a1 = agents.at( rand()%agents.size() );
  if(agents.size() == 1){ return a1; };

  a2 = agents.at( rand()%agents.size() );
  while( a1.ID == a2.ID ){
    a2 = agents.at( rand()%agents.size() );
  };

  if(a1.fitness > a2.fitness){
    if(RAND_UNIFORM <= 1-chance_amount){
      winner = 1;
    }
    else{
      winner = 2;
    };
  }
  else if(a1.fitness == a2.fitness){
    if(RAND_UNIFORM <= 0.5){
      winner = 1;
    }
    else{
      winner = 2;
    };
  }
  else{
    if(RAND_UNIFORM <= chance_amount){
      winner = 1;
    }
    else{
      winner = 2;
    };
  };

  if(winner == 1){
    remove_agent(a2);
    return a1;
  }
  else{
    remove_agent(a1);
    return a2;
  };

};
////////////////////////////////////////////////////////////////////////////////


// FUNCTIONS
////////////////////////////////////////////////////////////////////////////////
CANDLES get_candles(int num_candles){

  ifstream                 candles_raw;
  vector<double>           candles_str;
  vector< vector<double> > result_pre, result;

  candles_raw.open("candles_raw.txt");
  string            STRING;
  string::size_type sz;

  // loop(j,1000){
  while(!candles_raw.eof()){
    try{

    bool flag = true;

    loop(i,4){
      getline(candles_raw, STRING);
      try{ candles_str.push_back(stod(STRING, &sz)); } 
      catch(...){ flag = false; };
      STRING = "";
    };
    if(flag){ result_pre.push_back(candles_str); };

    }
    catch(...){
      print("candles_raw.txt has an uneven amount of lines!");
    };

    candles_str.clear();
  };

  result.insert(
    result.end(),
    result_pre.begin() + (result_pre.size()-num_candles),
    result_pre.end()
  );

  return result;

};
////////////////////////////////////////////////////////////////////////////////
