#ifndef KERNELS_H
#define KERNELS_H

#include "../../mth.h"

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

  sums[i]   = s_sums[i];
  reacts[i] = s_reacts[i];
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
  __syncthreads();

  s_reacts[i]   = reacts[itr_rs];
  // s_weights[i]  = weights[itr_w];
  // s_raw_sums[i] = reacts[i] * weights[i];
  s_raw_sums[i] = tidx;
  __syncthreads();

  raw_sums[i] = s_raw_sums[i];



  // if(tidx != itr_rs)
  //   s_raw_sums[i] = s_reacts[i] * weights[i];
  // __syncthreads();

  // UINT a  /*agent*/  = (UINT)blockIdx.x/Nn;
  // UINT n1 /*neuron*/ = a*Nn+blockIdx.x;
  // UINT n2 /*neuron2*/= threadIdx.x;
  // UINT i    = threadIdx.x;
  // UINT tidx = blockIdx.x*blockDim.x+threadIdx.x;

  // reacts[i]  = agents[a].neurons[n2].react[0];
  // weights[i] = agents[a].neurons[n2].weights[n1];
  // __syncthreads();

  // if(n1!=n2)
  //   sums[i] = reacts[i] * weights[i];
  // else
  //   sums[i] = 0;
  // __syncthreads();

  // sb[tidx] = sums[i];
};
////////////////////////////////////////////////////////////////////////////////






// __global__
// void kernel_prepare(double *sums, double *reacts,
//     double *candles, UINT ITR, UINT N){
//   UINT tindex    = blockIdx.x*blockDim.x+threadIdx.x;
//   if(tindex<N) sums[tindex] = candles[ITR*N+tindex];
//   reacts[tindex] = 1/(1+exp(-sums[tindex]))-.5;
//   sums[tindex]   = 0;
// };
// 
// 
// __global__
// void kernel_fire(double *sums, double *reacts, double *weights){
//   volatile __shared__ double sdata[THREADS];
//   volatile __shared__ double rdata[THREADS];
//   volatile __shared__ double wdata[THREADS];
// 
//   UINT tid    = threadIdx.x;  /* opposing neuron */
//   //UINT tindex = blockIdx.x*blockDim.x+threadIdx.x;
//   UINT neuron = blockIdx.x;   /* this block's neuron */
//   rdata[tid]  = reacts[tid];
//   wdata[tid]  = weights[blockDim.x*tid+neuron];
//   __syncthreads();
// 
//   if(tid != neuron)
//     sdata[tid] = rdata[tid]*wdata[tid];
//   else
//     sdata[tid] = 0;
//   __syncthreads();
// 
//   for(UINT s=blockDim.x/2; s>0; s>>=1){
//     if(tid<s)
//       sdata[tid]+=sdata[tid+s];
//     __syncthreads();
//   };
// 
//   if(tid==0) sums[blockIdx.x]=sdata[0];
// };
// 
// 
// __global__
// void kernel_get_fit(double *sums, double *fitness, double *trade, double *candles,
//   UINT num_neurons, UINT ITR, UINT N)
// {
//   double result = 1/(1+exp(-sums[num_neurons-1]))-.5;
//   if(result > 0 && trade[IN_TRADE] == 0){
//     /*buy*/
//     trade[IN_TRADE] = 1;
//     trade[BUY_AT]   = candles[ITR*N+CLOSE];
//     trade[AMOUNT]   = *fitness;
//   }else if(result <=0 && trade[IN_TRADE] == 1){
//     /*sell*/
//     trade[IN_TRADE] = 0;
//     trade[SELL_AT]  = candles[ITR*N+CLOSE];
//     double ratio    = trade[SELL_AT]/trade[BUY_AT];
//     *fitness        = ratio*trade[AMOUNT];
//   };
// };
////////////////////////////////////////////////////////////////////////////////



#endif
