#ifndef BRAIN_H
#define BRAIN_H

#include "mth.h"
#include "neuron.h"
#include "threadpool.h"


__global__
void process_thread(int N, double* r, double* w, double* s){
  int index = blockIdx.x * blockDim.x + threadIdx.x;
  int stride = blockDim.x * gridDim.x;
  for(int i = index; i<N; i += stride){
    *s = *r * *w;
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
    loop(k, iterations){
      // prepare
      for(auto& n : neurons)
        n.prepare();

      // fire
      /* here, we have to copy each 'react' and 'Weight_n' for each
       * thread.  Then we can send them to their respective cuda cores.*/
      int num_processes = (num_neurons * (num_neurons - 1));
      double *r, *w, *s;

      cudaMallocManaged(&r, num_processes*sizeof(double));
      cudaMallocManaged(&w, num_processes*sizeof(double));
      cudaMallocManaged(&s, num_processes*sizeof(double));

      //v(v(double*)) processes;
      //processes.reserve(num_processes);

      int j = 0;
      loop(i, num_neurons){
        for(auto& n : neurons){
          if(n.index_id != i){
            r[i] = (n.react);
            w[i] = (n.weights[i]);
            s[i] = (neurons[i].sum);
            //processes.push_back({
            //  new double(n.react),
            //  new double(n.weights[i]),
            //  &(neurons[i].sum)
            //});
          };
        };
      };

      process_thread<<<1, 256>>>(num_processes, r, w, s);
      cudaDeviceSynchronize();
      cudaFree(r);
      cudaFree(w);
      cudaFree(s);

      // for(auto& proc : processes)
      //   process_thread(proc[0], proc[1], proc[2]);

      // for(auto& proc : processes){
      //   delete proc[0];
      //   delete proc[1];
      // };

    };
  };

  inline void 
  input(v(double) &x){
    int len_x = x.size();
    loop(i, len_x)
      neurons[i].sum = x[i];
  };

  inline v(double) 
  output(int num_outputs){
    v(double) result(num_outputs);

    int len = num_neurons - 1;
    loop(i, num_outputs){
      Neuron* neuron = &neurons[len-i];
      neuron->prepare();
      result[i] = neuron->react;
    };

    return result;
  };

  inline void
  reset(){
    for(auto& n : neurons){
      n.sum   = 0;
      n.react = 0;
    };
  };

  inline 
  string get_str(){
    stringstream result;
    result << "--" << endl;

    for(auto& n : neurons)
      result << "| " << n.get_str() << endl;

    result << "--" << endl;
    return result.str();
  };

  inline string get_data(){
    stringstream result;
    result << num_neurons << endl;

    for(auto& n : neurons)
      result << n.get_data();
    result << "end brain" << endl;

    return result.str();
  };

  inline void load_data(v(string) data){

    num_neurons = stoi(data[0]);
    neurons.clear();

    int len_data = data.size();
    int itr = 1;
    while(data[itr] != "end brain"){

      int        x = 0;
      int        n_index_id;
      v(double)  n_weights;

      while(data[itr] != "end neuron"){
        if(x == 0){
          n_index_id = stoi(data[itr]);
          x++;
        }
        else
          n_weights.push_back(
            stod(data[itr])
          );
        itr++;
      };

      neurons.push_back(
        Neuron(n_index_id, num_neurons, n_weights)
      );

      itr++;

    };

  };

};



#endif
