#ifndef NEURON_H
#define NEURON_H

#include "mth.h"


struct Neuron{                                         // STRUCT NEURON
private:

  int*        index_id;
  int*        length;
  int*        threshold;
  double*     sum;
  double*     react;

  v(Neuron*)  neighbors;
  v(double*)  weights;

public:
  inline Neuron(int index_id, int num_neurons){        // CONSTRUCTOR
    this->index_id  = new int(index_id);
    this->length    = new int(num_neurons);
    this->threshold = new int(RAND_UNIFORM);
    this->sum       = new double(0);
    this->react     = new double(0);
    this->neighbors.reserve(num_neurons);
    this->weights.reserve(num_neurons);
    loop(i, num_neurons)
      weights.push_back(new double(0));
    reset_weights();
  };

  inline ~Neuron(){                                    // DESTRUCTOR
    delete index_id;
    delete length;
    delete threshold;
    delete sum;
    delete react;

    loop(i, weights.size())
      delete weights.at(i);
    weights.clear();
  };

  inline void set_neighbors(v(Neuron*) neurons){       // set_neighbors()
    for(auto& n : neurons)
      neighbors.push_back(n);
  };

  inline void reset_weights(){                         // reset_weights()
    for(auto* w : weights)
      *w = ((double)(rand()%200)-100)/100;
  };

  inline void reset_weight(int weight_id){             // reset_weight()
    *(weights.at(weight_id)) = ((double)(rand()%200)-100)/100;
  };

  inline void reset_neuron(){                          // reset_neuron()
    *sum = 0; *react = 0;
  };
  inline void add_to_sum(int x){ *sum += x; };         // add_to_sum()

  inline void prepare(){                               // prepare()
    *react = round(1/(1+exp(-*sum)),4);
    *sum   = 0;
  };

//  inline void fire(){                                  // fire()
//    int len_weights = weights->size();
//    if(*react != 0){
//
//      int i = 0;
//      for(auto& w : *weights){
//
//        if(*index_id != i){
//          Neuron* neuron = neighbors->at(i);
//          // neuron->add_to_sum(*react * *w);
//        };
//
//        i++;
//      };
//
//    };
//  };

  inline string get_str(){                             // get_str()
    stringstream result;
    result << "Idx: " << set_length(*index_id,            3) << ", ";
    result <<  "Sz: " << set_length((int)weights.size(),  3) << ", ";
    result <<   "S: " << set_length(dbl_to_s(*sum,  2),   7) << ", ";
    result <<   "R: " << set_length(dbl_to_s(*react,2),   4) << " | ";
    result <<   "W: ";
    loop(i, 5)
      result << set_length(dbl_to_s(*(weights.at(i)), 2), 6) << ", ";
    result << "..";
    return result.str();
  };

  inline string get_data(){                            // get_data()
    stringstream result; 

    result << *index_id << endl;
    for(auto* w : weights)
      result << *w << endl;

    result << "end neuron" << endl;
    return result.str();
  };

};


#endif

