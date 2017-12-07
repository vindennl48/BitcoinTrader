#include <iostream>
#include <math.h>
#include <string>
#include "../../mth.h"

typedef unsigned long int UINT;


#undef  THREADS
#define THREADS  (1<<9)   /* 512 */

// KERNELS
////////////////////////////////////////////////////////////////////////////////
__global__
void kernel_prepare(double *sums, double *reacts,
    double *inputs, UINT ITR, UINT N){
  UINT tindex    = blockIdx.x*blockDim.x+threadIdx.x;
  if(tindex<N) sums[tindex] = inputs[ITR*N+tindex];
  reacts[tindex] = 1/(1+exp(-sums[tindex]))-.5;
  sums[tindex]   = 0;
};
__global__
void kernel_fire(double *sums, double *reacts, double *weights){
  volatile __shared__ double sdata[THREADS];
  volatile __shared__ double rdata[THREADS];
  volatile __shared__ double wdata[THREADS];

  UINT tid    = threadIdx.x;  /* opposing neuron */
  //UINT tindex = blockIdx.x*blockDim.x+threadIdx.x;
  UINT neuron = blockIdx.x;   /* this block's neuron */
  rdata[tid]  = reacts[tid];
  wdata[tid]  = weights[blockDim.x*tid+neuron];
  __syncthreads();

  if(tid != neuron)
    sdata[tid] = rdata[tid]*wdata[tid];
  else
    sdata[tid] = 0;
  __syncthreads();

  for(UINT s=blockDim.x/2; s>0; s>>=1){
    if(tid<s)
      sdata[tid]+=sdata[tid+s];
    __syncthreads();
  };

  if(tid==0) sums[blockIdx.x]=sdata[0];
};
__global__
void kernel_get_fit(double *sums, double *fitness, UINT num_neurons){
  UINT result = 1/(1+exp(-sums[num_neurons]))-.5;
  if(result > 0)
    /*buy*/
  else
    /*sell*/
};
////////////////////////////////////////////////////////////////////////////////


void get_candles(int, double*);


#define AGENT_RANDOM  2
struct Agent{
  UINT           agentID;
  UINT           num_neurons;
  double         trade[4];
  cArray<double> sums;
  cArray<double> reacts;
  cArray<double> weights;
  cArray<double> fitness;

  Agent(UINT num_neurons, UINT ID, UINT options=2)
  :sums(num_neurons),reacts(num_neurons),weights(pow(num_neurons,2)),
   num_neurons(num_neurons),fitness(1),agentID(ID)
  {
    if(options == AGENT_RANDOM){

      /* create a random brain */
      loop(i, num_neurons){
        sums.h[i] = 0;
        reacts.h[i] = 0;
      };
      loop(i, pow(num_neurons,2))
        weights.h[i] = ((double)(rand()%200)-100)/100;

      *fitness.h = 100;
    }
  };
  void send(){
    sums.send();
    reacts.send();
    weights.send();
    fitness.send();
  };
  void receive(){
    //sums.receive();
    fitness.receive();
  };
};


// MAIN
////////////////////////////////////////////////////////////////////////////////
int main(){

  const UINT num_neurons = THREADS;
  const UINT num_inputs  = 10000;
  cArray<double> candles(num_inputs);
  get_candles(num_inputs, candles.h);

  Agent agent(num_neurons, 2);

  int start = mtime();

  agent.send(); candles.send();
  loop(i, num_inputs){
    kernel_prepare<<<1,num_neurons>>>(
      agent.sums.d,
      agent.reacts.d,
      candles.d,
      i, 4
    );
    kernel_fire<<<num_neurons,num_neurons>>>(
      agent.sums.d,
      agent.reacts.d,
      agent.weights.d
    );
  };
  agent.receive();

  int end = mtime();

  loop(i, num_neurons)
    print("sum: " << agent.sums.h[i]);

  print("Time Elapsed: " << (start-end) << "ms, " << (double)(20*(start-end))/1000 << "sec/20");

  return 0;
}
////////////////////////////////////////////////////////////////////////////////

inline void
get_candles(int num_candles, double *candles){
  ifstream        if_candles;
  v(double)       candles_temp;
  //cArray<double>  candles(num_candles);

  if_candles.open("candles_raw.txt");
  string STRING;

  while(!if_candles.eof()){
    try{
      getline(if_candles, STRING);
      try{ candles_temp.push_back(atof(STRING.c_str()));} 
      catch(...){ print("Errir converting string to double."); };
      STRING = "";
    }
    catch(...){
      print("candles_raw.txt has an uneven amount of lines!");
    };
  };

  UINT size = candles_temp.size()-2;

  loop(i, num_candles){
    double t = candles_temp[size-i];
    candles[i] = t;
  };
};
