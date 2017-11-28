#ifndef NEURON_H
#define NEURON_H

#include "mth.h"


struct Neuron{                                         // STRUCT NEURON
public:

  int         index_id;
  double      sum;
  double      react;
  double      num_neurons;
  v(double)   weights;

  inline Neuron(int index_id, int num_neurons)
  : index_id(index_id), num_neurons(num_neurons)
  {                                                    // CONSTRUCTOR
    sum   = 0;
    react = 0;
    weights.reserve(num_neurons);
    loop(i, num_neurons)
      weights.push_back(0);
    reset_weights();
  };

  inline Neuron(int index_id, int num_neurons, v(double) weights)
  : index_id(index_id), num_neurons(num_neurons), weights(weights)
  {                                                    // CONSTRUCTOR
    sum   = 0;
    react = 0;
  };

  inline ~Neuron(){};                                  // DESTRUCTOR

  inline void reset_weights(){                         // reset_weights()
    for(auto& w : weights)
      w = get_random_weight();
  };

  inline void reset_weight(int weight_id){             // reset_weight()
    weights[weight_id] = get_random_weight();
  };

  inline double get_random_weight(){                   // get_random_weight()
    return ((double)(rand()%200)-100)/100;
  };

  inline void reset_neuron(){                          // reset_neuron()
    sum   = 0;
    react = 0;
  };

  inline void add_to_sum(double x){                    // add_to_sum()
    sum += x;
  };

  inline void prepare(){                               // prepare()
    react = round(1/(1+exp(sum*-1)),4);
    sum = 0;
  };

  inline string get_str(){                             // get_str()
    stringstream result;
    result << "Idx: " << set_length(index_id,             3) << ", ";
    result <<  "Sz: " << set_length((int)weights.size(),  3) << ", ";
    result <<   "S: " << set_length(dbl_to_s(sum,   2),   7) << ", ";
    result <<   "R: " << set_length(dbl_to_s(react, 2),   4) << " | ";
    result <<   "W: ";
    loop(i, 5)
      result << set_length(weights[i], 6) << ", ";
    result << "..";
    return result.str();
  };

  inline string get_data(){                            // get_data()
    stringstream result; 

    result << index_id << endl;
    for(auto& w : weights)
      result << w << endl;

    result << "end neuron" << endl;
    return result.str();
  };

};


#endif

