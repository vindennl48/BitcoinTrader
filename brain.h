#ifndef BRAIN_H
#define BRAIN_H

#include <vector>
#include <string>


#define BRAIN std::vector< std::vector<double> >


////////////////////////////////////////////////////////////////////////////////
namespace brain {

  BRAIN create(int num_neurons);

  std::string get_str(BRAIN &b);

  void run(BRAIN &b, int iterations=2);

  void input(BRAIN &b, std::vector<double> &x);

  std::vector<double> output(BRAIN &b, int num_outputs);

  void reset(BRAIN&);

}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Neuron index for settings
#define NEURON std::vector<double>
#define INDEX_ID 0
#define SUM      1
#define REACT    2

namespace neuron {

  NEURON create(double index_id, int length);

  void reset_weights(NEURON &n);

  void fire(NEURON &n, BRAIN &b);

  void prepare(NEURON &n);

  std::string get_str(NEURON &n);

};
////////////////////////////////////////////////////////////////////////////////



#endif
