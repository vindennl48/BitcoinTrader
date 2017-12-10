#ifndef KERNELS_H
#define KERNELS_H

#include "../../mth.h"


#ifndef Na
#define Na  10      /*num agents*/
#endif

#ifndef Nn
#define Nn  (1<<7)  /*num neurons per agent*/
#endif

#ifndef Np
#define Np  (Nn*Nn) /*num processes per agent*/
#endif


// KERNELS
////////////////////////////////////////////////////////////////////////////////
__global__
void kernel_prepare(
  Agent *agents
  /*double *sums, double *reacts, double *candles, UINT ITR, UINT N*/
){
  UINT tidx = Idxer::idx(blockIdx.x,threadIdx.x,blockDim.x);
  UINT aid  = blockIdx.x;
  UINT nID  = threadIdx.x;
  // if(tindex<N) sums[tindex] = candles[ITR*N+tindex];
  double sum = agents[aid].neurons[nID].sum[0];
  agents[aid].neurons[nID].react[0] = 1/(1+exp(-sum))-.5;
  agents[aid].neurons[nID].sum[0] = 0;
};


/* Agents
  *  - Agent          (UINT)i/n
  *    - Neurons      i-x*n;
  *      - Neuron
  *        - sum
  *        - react
  *        - Weights
  *          - Weight
  *          - Weight
  *      - Neuron
  *        - sum
  *        - react
  *        - Weights
  *          - Weight
  *          - Weight
  *  - Agent          (UINT)i/n
  *    - Neurons      i-x*n;
  *      - Neuron
  *        - sum
  *        - react
  *        - Weights
  *          - Weight
  *          - Weight
  *      - Neuron
  *        - sum
  *        - react
  *        - Weights
  *          - Weight
  *          - Weight
  * */
__global__ void
kernel_fire(Agent *agents, double *sb/*sums block*/){
  volatile __shared__ double reacts[Nn];
  volatile __shared__ double weights[Nn];
  volatile __shared__ double sums[Nn];

  UINT a  /*agent*/  = (UINT)blockIdx.x/Nn
  UINT n1 /*neuron*/ = a*Nn+threadIdx.x;

  UINT i    = threadIdx.x;
  UINT tidx = Idxer::idx(blockIdx.x,threadIdx.x,blockDim.x);
  UINT aid  = (UINT)tidx/Np;
       tidx = tidx-aid*Np;
       tidx = Idxer::col(tidx,Np);
  UINT n2ID = i;
  UINT nID  = tidx-n2ID*Nn;

  reacts[i]  = agents[aid].neurons[n2ID].react[0];
  weights[i] = agents[aid].neurons[n2ID].weights[nID];
  __syncthreads();

  if(nID!=n2ID)
    sums[i] = reacts[i] * weights[i];
  else
    sums[i] = 0;
  __syncthreads();

  sb[tidx] = sums[i];
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
