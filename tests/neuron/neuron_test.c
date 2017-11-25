#include "../../neuron.h"


// CONSTRUCTOR
bool test_constructor(int x){
  print("\nStarted: \"test_constructor()\"");
  Neuron neuron(0, x);
  print("Finished: \"test_constructor()\"\n");
};

// set_neighbors()
bool test_set_neighbors(int x){
  print("\nStarted: \"test_set_neighbors()\"");
  v(Neuron*) neurons;
  loop(i, x)
    neurons.push_back(new Neuron(i, x));

  for(auto& n : neurons)
    n->set_neighbors(neurons);

  print("Finished: \"test_set_neighbors()\"\n");
};

// reset_weight_s_()
bool test_reset_weight_s_(int x){
  print("\nStarted: \"test_reset_weights()\"");

  v(Neuron*) neurons;
  loop(i, x)
    neurons.push_back(new Neuron(i, x));

  for(auto& n : neurons)
    n->set_neighbors(neurons);

  for(auto& n : neurons){
    n->reset_weights();
    n->reset_weight(0);
  };


  print("Finished: \"test_reset_weights()\"\n");
};

bool test_prepare(int x){
  print("\nStarted: \"test_prepare()\"");

  Neuron neuron(0, x);
  neuron.prepare();

  print("Finished: \"test_prepare()\"\n");
};

bool test_get_str(int x){
  print("\nStarted: \"test_get_str()\"");

  Neuron neuron(0, x);
  print(neuron.get_str());

  print("Finished: \"test_get_str()\"\n");
};

bool test_get_data(int x){
  print("\nStarted: \"test_get_data()\"");

  Neuron neuron(0, x);
  print(neuron.get_data());

  print("Finished: \"test_get_data()\"\n");
};

bool test_reset_neuron_and_add_to_sum(int x){
  print("\nStarted: \"test_reset_neuron()\"");

  Neuron neuron(0, x);
  neuron.add_to_sum(5);
  neuron.reset_neuron();
  print(neuron.get_str());

  print("Finished: \"test_reset_neuron()\"\n");
};







////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int main(){
  int num_neurons = 35;

  test_constructor(num_neurons);
  test_set_neighbors(num_neurons);
  test_reset_weight_s_(num_neurons);
  test_prepare(num_neurons);
  test_get_str(num_neurons);
  test_get_data(num_neurons);
  test_reset_neuron_and_add_to_sum(num_neurons);

  return 0;
};

