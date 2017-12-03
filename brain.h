#ifndef BRAIN_H
#define BRAIN_H

#include "mth.h"
#include "neuron.h"
#include "threadpool.h"


// block size and amount
#define T     1024
#define B(x)  (x+T-1)/T


__global__
void prepare_neurons(int N, double *sum, double *react){
  int index = blockIdx.x * blockDim.x + threadIdx.x;
  if(index < N){
    react[index] = round(1/(1+exp(sum[index]*-1)),4);
    sum[index] = 0;
  };
};


__global__
void fire_neurons(int N, double *weights, double *sum, double *react){
  int index = blockIdx.x * blockDim.x + threadIdx.x;
  loop(i, N){
    if(i != index && index < N)
      sum[index] += weights[(1+i)*index] * react[i];
    __syncthreads();
  };
};


class Brain{
public:
  int               num_neurons;
  v(Neuron)         neurons;

  inline
  Brain(int num_neurons)
  : num_neurons(num_neurons)
  {
    loop(i, num_neurons)
      neurons.push_back(Neuron(i, num_neurons));
  };
  inline
  ~Brain(){};

  inline void
  run(int iterations=2){
      for(auto& n : neurons)
        n.prepare();

      // fire
      /* here, we have to copy each 'react' and 'Weight_n' for each
       * thread.  Then we can send them to their respective cuda cores.*/
      int N = num_neurons;
      cArray<double> weights(N*(N-1)), sum(N), react(N);

      int itr = 0;
      for(auto& n : neurons){
        sum.h[itr] = n.sum;
        react.h[itr] = n.react;
        loop(i, N-1)
          weights.h[(1+itr)*i] = n.weights[i];
        itr += 1;
      };

      weights.send(); sum.send(); react.send();

      loop(j, iterations){
        prepare_neurons<<<B(N),T>>>(N, sum.d, react.d);
        fire_neurons<<<B(N),T>>>(N, weights.d, sum.d, react.d);
      };

      weights.receive(); sum.receive(); react.receive();

      itr = 0;
      for(auto& n : neurons){
        n.sum = sum.h[itr];
        n.react = react.h[itr];
        loop(i, N-1)
          n.weights[i] = weights.h[(1+itr)*i];
        itr += 1;
      };

    //};
  };

  //inline void 
  //input(v(double) &x){
  //  int len_x = x.size();
  //  loop(i, len_x)
  //    neurons[i].sum = x[i];
  //};

  //inline v(double) 
  //output(int num_outputs){
  //  v(double) result(num_outputs);

  //  int len = num_neurons - 1;
  //  loop(i, num_outputs){
  //    Neuron* neuron = &neurons[len-i];
  //    neuron->prepare();
  //    result[i] = neuron->react;
  //  };

  //  return result;
  //};

  //inline void
  //reset(){
  //  for(auto& n : neurons){
  //    n.sum   = 0;
  //    n.react = 0;
  //  };
  //};

  //inline 
  //string get_str(){
  //  stringstream result;
  //  result << "--" << endl;

  //  for(auto& n : neurons)
  //    result << "| " << n.get_str() << endl;

  //  result << "--" << endl;
  //  return result.str();
  //};

  //inline string get_data(){
  //  stringstream result;
  //  result << num_neurons << endl;

  //  for(auto& n : neurons)
  //    result << n.get_data();
  //  result << "end brain" << endl;

  //  return result.str();
  //};

  //inline void load_data(v(string) data){

  //  num_neurons = stoi(data[0]);
  //  neurons.clear();

  //  int len_data = data.size();
  //  int itr = 1;
  //  while(data[itr] != "end brain"){

  //    int        x = 0;
  //    int        n_index_id;
  //    v(double)  n_weights;

  //    while(data[itr] != "end neuron"){
  //      if(x == 0){
  //        n_index_id = stoi(data[itr]);
  //        x++;
  //      }
  //      else
  //        n_weights.push_back(
  //          stod(data[itr])
  //        );
  //      itr++;
  //    };

  //    neurons.push_back(
  //      Neuron(n_index_id, num_neurons, n_weights)
  //    );

  //    itr++;

  //  };

  //};

};



#endif
