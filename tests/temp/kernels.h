#ifndef KERNELS_H
#define KERNELS_H

#include "../../mth.h"

cArray<double> sums(Na*Nn);
cArray<double> raw_sums(Na*Np);
cArray<double> reacts(Na*Nn);
cArray<double> weights(Na*Np);
cArray<double> data(Nd*Ndp);
cArray<double> fit_scores(Na);
cArray<double> trades(Na*4)


__host__ inline void
send_agents(){
  sums.send(); raw_sums.send();
  reacts.send(); weights.send();
  data.send();
};

__host__ inline void
receive_agents(){
  sums.receive(); 
  //reacts.receive(); 
  //raw_sums.receive();
  //weights.receive();
};

__host__ inline void
set_random_agents(){
  loop(i, Na*Nn){
    sums[i]     = 0;
    reacts[i]   = 0;
  };
  loop(i, Na*Np){
    raw_sums[i] = 0;
    weights[i]  = ((double)(rand()%200)-100)/100;
  };
  loop(i, Na*4) trades[i]     = 0;
  loop(i, Na)   fit_scores[i] = 100;
};

__host__ inline void
set_random_data(){
  loop(i, Nd*Ndp)
    data[i] = ((double)(rand()%200)-100);
};

__host__ __device__ inline void
get_start_points(UINT *itr_rs, UINT *itr_w, const UINT &tidx){
  UINT a,n,i;
  a = (UINT)tidx/Np; 
  n = (UINT)(tidx-(a*Np))/Nn;
  i = (tidx-(a*Np))-(n*Nn);
  *itr_rs = i;
  *itr_w  = (a*Nn)+(i*Nn)+n;
};

// KERNELS
////////////////////////////////////////////////////////////////////////////////
__global__ void
kernel_prepare(double *sums, double *reacts){
  UINT tidx = blockIdx.x*blockDim.x+threadIdx.x;

  reacts[tidx] = 1/(1+exp(-sums[tidx]))-.5;
  sums[tidx]   = 0;
  __syncthreads();
};

__global__ void
kernel_fire(double *sums, double *raw_sums,
  double *reacts, double *weights)
{

  UINT tidx = blockIdx.x*blockDim.x+threadIdx.x;
  UINT itr_rs, itr_w;
  get_start_points(&itr_rs, &itr_w, tidx);

  if(itr_w != tidx)
    raw_sums[tidx] = reacts[itr_rs] * weights[itr_w];
  else
    raw_sums[tidx] = 0;

//  volatile __shared__ double s_reacts[Nn];
//  volatile __shared__ double s_weights[Nn];
//  volatile __shared__ double s_raw_sums[Nn];
//
//  UINT tidx = blockIdx.x*blockDim.x+threadIdx.x;
//  UINT i    = threadIdx.x;
//  UINT itr_rs, itr_w;
//  get_start_points(&itr_rs, &itr_w, tidx);
//  s_reacts[i]   = reacts[itr_rs];
//  s_weights[i]  = weights[itr_w];
//  s_raw_sums[i] = 0;
//   __syncthreads();
//
//  if(itr_w != tidx)
//    s_raw_sums[i] = s_reacts[i] * s_weights[i];
//  else
//    s_raw_sums[i] = 0;
//  __syncthreads();
//
//  raw_sums[tidx] = s_raw_sums[i];
};


__global__ void
kernel_add(double *sums, double *raw_sums){
  /* each block will process 2 neurons at a time */
  volatile __shared__ double sums1[Nn];
  volatile __shared__ double sums2[Nn];
  UINT tidx, n, i;
  i = threadIdx.x; n = blockIdx.x;
  tidx = blockIdx.x*blockDim.x+threadIdx.x;
  sums1[i] = raw_sums[tidx];
  sums2[i] = raw_sums[tidx+Np*(Na/2)];
  __syncthreads();

  if(i<Nn/2){
    for(UINT s=Nn/2; s>0; s>>=1){
      if(i<s)
        sums1[i] += sums1[i+s];
      __syncthreads();
    };
    if(i==0)
      sums[n] = sums1[0];
  }
  else{
    i -= Nn/2;
    for(UINT s=blockDim.x/2; s>0; s>>=1){
      if(i<s)
        sums2[i] += sums2[i+s];
      __syncthreads();
    };
    if(i==0)
      sums[n+Nn*(Na/2)] = sums2[0];
  };
};
__global__ void
kernel_set_inputs(double *sums, double *data, int d_i){
  UINT i = threadIdx.x;
  UINT tidx = blockIdx.x*blockDim.x+threadIdx.x;
  if(i<Ndp) sums[tidx] = data[d_i+i];
};
__global__ void
kernel_set_fitness(double *sums, double *trades, 
  double *fit_scores, double *data, double d_i)
{
  UINT i = threadIdx.x; /*agent number*/
  if(i<Na){
    double result = sums[(Nn*i)+(Nn-1)];
    result = 1/(1+exp(-result))-.5;

    /*t1=in_trade, t2=buy_at, t3=sell_at, t4=amount*/
    UINT t1,t2,t3,t4;
    t1 = i*4;  /*i agents times 4 trade variabales*/
    t2 = t1+1; t3 = t2+1; t4 = t3+1;

    if(result>0 && trades[t1] == 0){
      /*buy*/
      trades[t1] = 1;
      trades[t2] = data[d_i+/*candle-close i*/];
      trades[t4] = fit_scores[i];
    }
    else if(result<=0 && trades[t1] == 1){
      /*sell*/
      trades[t1]    = 0;
      trades[t3]    = data[d_i+/*candle-close i*/];
      double ratio  = trades[t3] / trades[t2];
      fit_scores[i] = ratio * trades[t4];
    };
  };
  __syncthreads();
};


// Functions
////////////////////////////////////////////////////////////////////////////////
inline void
agents_prepare(){ kernel_prepare<<<Na,Nn>>>(sums.d, reacts.d); };
inline void
agents_fire(){ kernel_fire<<<(Na*Nn),Nn>>>(
  sums.d, raw_sums.d, reacts.d, weights.d); };
inline void
agents_add(){ kernel_add<<<(Na*Nn)/2,Nn>>>(sums.d, raw_sums.d); };

int data_i=0;
inline void
agents_set_inputs(){
  kernel_set_inputs<<<Na,Nn>>>(sums.d, data.d, data_i);
  data_i += Ndp;
  if(data_i >= Ndp*Nd)
    data_i = 0;
};

inline void
agents_set_fitness(){ 
  kernel_set_fitness<<<1,Nn>>>(sums.d, trades.d, fit_scores.d,
    data.d, (data_i-Ndp)); 
};

inline void
agents_run_generation(){ 
  agents_set_inputs();
  agents_prepare();
  agents_fire();
  agents_add();
  agents_set_fitness();
};

////////////////////////////////////////////////////////////////////////////////


#endif
