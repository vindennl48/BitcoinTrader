#ifndef AGENT_TEMPLATE_H
#define AGENT_TEMPLATE_H

#include "../bin/mth.h"


struct AgentTemplate{

  static int agent_id;

  int    ID;
  double fitness;
  // v(T)*  dna;

  inline AgentTemplate()
  : ID(get_id()),
    fitness(0){};

  inline int get_id(){
    agent_id++;
    return agent_id;
  };

  inline string get_agent_str(){
    stringstream result;
    result << " - ID: " << set_length(to_string(ID), 6) << "  |  My Fitness: ";
    result << set_length(dbl_to_s(fitness, 2), 8);
    return result.str();
  };

};
struct Agent;


#endif
