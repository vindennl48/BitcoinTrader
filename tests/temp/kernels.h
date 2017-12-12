#ifndef KERNELS_H
#define KERNELS_H

#include "../../mth.h"

cArray<double> sums(Na*Nn);
cArray<double> raw_sums(Na*Np);
cArray<double> reacts(Na*Nn);
cArray<double> weights(Na*Np);

__host__ inline void
send_agents(){
  sums.send(); raw_sums.send();
  reacts.send(); weights.send();
};

__host__ inline void
receive_agents(){
  /*sums.receive();*/ raw_sums.receive();
  /*reacts.receive();*/ /*weights.receive();*/
};

__host__ inline void
set_random_agents(){
  loop(i, Na*Nn){
    sums[i]     = 1;
    raw_sums[i] = 0;
    reacts[i]   = 0;
  };
  loop(i, Na*Np) weights[i]=((double)(rand()%200)-100)/100;
};

__device__ inline void
get_start_points(UINT *itr_rs, UINT *itr_w, const UINT &tidx){
  UINT a,n,i,n2,i2;

  a = tidx/Np; n = (tidx-(a*Np))/Nn; i = (tidx-(a*Np))-(n*Nn);
  *itr_rs = i; *itr_w = (a*Nn)+(i*Nn)+n;

  // a  = tidx/Np; n  = tidx/Nn-a*Nn; i = tidx-(a*Np+n*Nn);
  // n2 = i;       i2 = n;
  // *itr_rs = a*Nn+n2; *itr_w = a*Np+n2*Nn+i2;
};

// KERNELS
////////////////////////////////////////////////////////////////////////////////
__global__ void
kernel_prepare(double *sums, double *reacts){
  volatile __shared__ double s_sums[Nn];
  volatile __shared__ double s_reacts[Nn];

  UINT tidx = blockIdx.x*blockDim.x+threadIdx.x;
  UINT i    = threadIdx.x;
  UINT itr_rs, itr_w;
  get_start_points(&itr_rs, &itr_w, tidx);

  s_sums[i] = sums[itr_rs];
  __syncthreads();

  s_reacts[i] = 1/(1+exp(-s_sums[i]))-.5;
  s_sums[i]   = 0;
  __syncthreads();

  sums[itr_rs]   = s_sums[i];
  reacts[itr_rs] = s_reacts[i];
};

__global__ void
kernel_fire(double *sums, double *raw_sums,
  double *reacts, double *weights)
{
  volatile __shared__ double s_raw_sums[Nn];
  volatile __shared__ double s_reacts[Nn];
  volatile __shared__ double s_weights[Nn];

  UINT tidx = blockIdx.x*blockDim.x+threadIdx.x;
  UINT i    = threadIdx.x;
  UINT itr_rs, itr_w;
  get_start_points(&itr_rs, &itr_w, tidx);
  s_reacts[i]   = reacts[itr_rs];
  s_weights[i]  = weights[itr_w];
  s_raw_sums[i] = 0;
   __syncthreads();

  s_raw_sums[i] = reacts[i] * weights[i];
  __syncthreads();

  raw_sums[tidx] = s_raw_sums[i];
};

__global__ void
kernel_add(double *sums, double *raw_sums){
  /* each block will process 2 neurons at a time */
  volatile __shared__ double sums1[Nn];
  volatile __shared__ double sums2[Nn];
  UINT tidx, itr1, itr2, a, n, i;
  i = threadIdx.x; n = blockIdx.x;
  tidx = blockIdx.x*blockDim.x+threadIdx.x;
  sums1[i] = raw_sums[tidx];
  sums2[i] = raw_sums[tidx+Np*(Na/2)];
  __syncthreads();

  a = (UINT)tidx/Np;
  if(i<Nn/2){
    for(UINT s=blockIdx.x/2; s>0; s>>=1){
      if(i<s)
        sums1[s] += sums1[i+s];
      __syncthreads();
    };
    sums[n] = sums1[0];
  }
  else{
    i -= Nn/2;
    for(UINT s=blockIdx.x/2; s>0; s>>=1){
      if(i<s)
        sums2[s] += sums2[i+s];
      __syncthreads();
    };
    sums[n+Nn*(Na/2)] = sums2[0];
  };
};


#endif
