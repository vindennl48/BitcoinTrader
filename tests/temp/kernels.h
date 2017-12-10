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
  a  = tidx/Np; n  = tidx/Nn-a*Nn; i = tidx-(a*Np+n*Nn);
  n2 = i;       i2 = n;

  *itr_rs = a*Nn+n2; *itr_w = a*Np+n2*Nn+i2;
};

__device__ inline void
get_start_points_add(UINT *itr_1, UINT *itr_2, const UINT &tidx){
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
  UINT tidx, itr;
  tidx = blockIdx.x*blockDim.x+threadIdx.x;

  if(blockIdx.x < gridDim.x/2){
    /*first half of block*/
    itr = tidx;
    sums1[itr] = raw_sums[itr];
    sums1[itr+(gridDim.x/2)*Nn] = raw_sums[itr+(gridDim.x/2)*Nn];
  }
  else{
    /*second half of block*/
    itr = tidx + (gridDim.x/2)*Nn;
    sums2 = raw_sums[itr];
  };
  __syncthreads();

  // get_start_points_add(&itr_1, &itr_2, tidx);

  // volatile __shared__ double s_raw_sums1[Na*Np];
  // volatile __shared__ double s_sums1[Na*Nn];

  // volatile __shared__ double s_raw_sums2[Na*Np];
  // volatile __shared__ double s_sums2[Na*Nn];

};


#endif
