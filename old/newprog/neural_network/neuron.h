#ifndef NEURON_H
#define NEURON_H

#include "../bin/mth.h"


struct Neuron{
private:
  mutex  *m;

public:
  int    index_id;
  int    length;
  double sum;
  double react;

  v(Neuron*) neighbors;
  v(double)  weights;

  inline Neuron(int index_id, int length)
  : index_id(index_id),
    sum(0),
    react(0),
    length(length),
    weights(length),
    m(new mutex()){

    reset_weights();

  };

  // fix this later
  inline Neuron(int index_id, int length, v(double) weights)
  : index_id(index_id),
    sum(0),
    react(0),
    length(length),
    weights(weights){};

  inline void set_neighbors(v(Neuron*) neurons){
    neighbors = neurons;
  };

  inline void reset_weights(){
    for (auto& w : weights)
      w = ((double)(rand()%200)-100)/100;
  };

  inline void reset_weight(int weight_id){
    weights[weight_id] = ((double)(rand()%200)-100)/100;
  };

  inline void prepare(){
    react = round(1/(1+exp(-sum)),4);
    sum   = 0;
  };

  inline void fire(){
    int len_weights = weights.size();
    if(react != 0){

      int i = 0;
      for(auto& w : weights){

        if(index_id != i){
          Neuron* neuron = neighbors.at(i);
          // double* sum    = &neuron->sum;
          // *sum += react * w;
          neuron->add_to_sum(react * w);
        };

        i++;
      };

    };
  };

  inline void add_to_sum(double value){
    //lock_guard<mutex> lock(m);
    sum += value;
  };

  inline string get_str(){
    stringstream result;
    result << "Idx: " << set_length(index_id,            3) << ", ";
    result <<  "Sz: " << set_length((int)weights.size(), 3) << ", ";
    result <<   "S: " << set_length(dbl_to_s(sum,  2),   7) << ", ";
    result <<   "R: " << set_length(dbl_to_s(react,2),   4) << " | ";
    result <<   "W: ";
    loop(i, 5)
      result << set_length(dbl_to_s(weights[i], 2), 6) << ", ";
    result << "..";
    return result.str();
  };

  inline string get_data(){
    stringstream result; 

    result << index_id << endl;
    for(auto& w : weights)
      result << w << endl;

    result << "end neuron" << endl;
    return result.str();
  };

};


#endif
