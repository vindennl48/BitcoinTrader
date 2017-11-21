#ifndef BRAIN_H
#define BRAIN_H

#include "neuron.h"
#include "../bin/mth.h"

inline void process_thread( Neuron &neuron ){
  neuron.fire();
};

struct Brain{

  int        length;
  v(Neuron)  neurons;
  v(Neuron*) L_neurons;

  inline Brain(int length)
  : length(length){

    loop(i, length)
      neurons.push_back( Neuron(i, length) );

    for(auto& n : neurons)
      L_neurons.push_back(&n);

    for(auto& n : neurons) 
      n.set_neighbors(L_neurons);
  };

  inline void run(int iter=2){
    loop(j, iter){
      v(thread) threads;

      for(auto& n : neurons)
        n.prepare();
      for(auto& n : neurons)
        threads.push_back(
          thread(process_thread, ref(n))
        );
        //n.fire();

      for(auto& t : threads) t.join();
    };
  };

  inline void input(v(double) &x){
    int len_x = x.size();
    loop(i, len_x)
      neurons[i].sum = x[i];
  };

  inline v(double) output(int num_outputs){
    v(double) result(num_outputs);

    int len = length - 1;
    loop(i, num_outputs){
      Neuron* neuron = &neurons[len-i];
       neuron->prepare();
      result[i] = neuron->react;
    };

    return result;
  };

  inline void reset(){
    for(auto& n : neurons){
      n.sum   = 0;
      n.react = 0;
    };
  };

  inline string get_str(){
    stringstream result;

    result << "--" << endl;

    for(auto& n : neurons)
      result << "| " << n.get_str() << endl;

    result << "--" << endl;

    return result.str();

  };

  inline string get_data(){
    stringstream result;

    result << length << endl;

    for(auto& n : neurons)
      result << n.get_data();

    result << "end brain" << endl;
    return result.str();
  };

  inline void load_data(v(string) data){

    length = stoi(data[0]);
    neurons.clear();

    int len_data = data.size();
    int itr = 1;
    while(data[itr] != "end brain"){

      int x = 0;
      int n_index_id;
      v(double) n_weights;

      while(data[itr] != "end neuron"){
        if(x == 0){
          n_index_id = stoi(data[itr]);
          x++;
        }
        else
          n_weights.push_back(stod(data[itr]));
        itr++;
      };

      // neurons.push_back(
      //   Neuron(n_index_id, length, &neurons, n_weights)
      // );

      itr++;

    };

  };

};




#endif
