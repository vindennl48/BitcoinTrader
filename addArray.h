/*
 * 
 * This function will add an array with cuda.. somehow..
 *
 * */

#include "mth.h"


template <typename T>
__global__
void cuda_addArray(int N, T *arr){
  int i  = GET_INDEX;

  while(true){
    int half_n = N/2;
    if(index <= half_n){
      int i2 = half_n+index;
      arr[i] += arr[i2];
      N = half_n;
    }
    else{ break; };
    __syncthreads();
  };
};


template <typename T>
inline T
addArray(int N, cArray<T> &arr){

  // Find if array is divisible
  unsigned int remainder = 0;
  for(int i=0; i<64; i+=1){
    unsigned int bit = 1<<i;
    if(bit >= N){
      remainder = bit-N;
      break;
    };
  };


  // Add remainders to last divisible num
  int x = N-remainder;
  for(int i=0; i<remainder; i+=1)
    arr.h[x-1] += arr.h[x+i];

  // Reset N to divisible num
  N -= remainder;

  // Run kernel to add array
  arr.send();
  addArray<<<B(N),1024>>>(N, arr.d);
  arr.receive();

  // Return value
  return arr.h[0];
};