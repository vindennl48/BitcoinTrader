#ifndef GENETICS_H
#define GENETICS_H

#include "agent_template.h"
#include "../bin/mth.h"


extern const int MAX_THREADS;


class Genetics{
public:

  int       population, generation;
  int       time_start, time_average;
  string    time_status;
  double    battle_chance;
  v(Agent)  agents;


  inline Genetics(int population, double battle_chance)
  : population(population),
    battle_chance(battle_chance),
    time_start(mtime()),
    generation(1){

    agents.reserve(population*2);
    loop(i, population)
      agents.push_back( Agent() );
  };

  inline void run(){
    print(" - Running Algorithm");

    int start, end;

    start = mtime();
    fitness();
    print_status();
    //create_new_generation();
    end = mtime();
    set_time_status(start, end);

    generation++;

  };

  inline void fitness(){
    vector<thread> threads;

    int len_agents = agents.size();
    int j = 0;
    loop(i, len_agents){

      if(j == MAX_THREADS){
        for(auto& t : threads) t.join();
        threads.clear();
        j = 0;
      };

      threads.push_back(thread(
        agents[i].get_fitness_thread()
      ));
      
      j++;
    };
    for(auto& t : threads) t.join();

    sort(agents.begin(), agents.end());
    reverse(agents.begin(), agents.end());

  };

  inline string get_str(){
    stringstream result;

    int len_agents = agents.size();
    if(len_agents >= 10)
      len_agents = 10;
    loop(i, len_agents){
      Agent* agent = &agents[i];
      result << agent->get_agent_str() << endl;
    };

    return result.str();
  };

  inline void print_status(){
    stringstream result;

    CLEAR;
    result << " # Generation: " << generation << endl;
    result << get_str() << time_status;

    print(result.str());
  };

  inline void set_time_status(int start, int end){
    double latest, elapsed, estimated;
    stringstream result;

    latest       = (double) (start-end);
    time_average =    (int) ((time_average + latest)/2);
    elapsed      = (double) (time_start - end);
    estimated    = (double) (time_average*100);

    string a,b,c;
    a = dbl_to_s(   latest/1000, 2);
    b = dbl_to_s(  elapsed/1000, 2);
    c = dbl_to_s(estimated/1000, 2);

    result << '+' << endl;
    result << " Latest Stopwatch: " << a << "sec, ";
    result <<           "Elapsed: " << b << "sec, ";
    result <<        "Est.100Gen: " << c << "sec";
    result << endl << '+' << endl;

    time_status = result.str();
  };

};


#endif
