// first cuda program

#include <iostream>
#include <math.h>

__global__
void add(int *a, int *b, int *c){
  c[threadIdx.x] = a[threadIdx.x] + b[threadIdx.x];
}

#define N 512

int main(){
  int *a, *b, *c;
  int *d_a, *d_b, *d_c;
  int size = N*sizeof(int);

  cudaMalloc((void **)&d_a, size);
  cudaMalloc((void **)&d_b, size);
  cudaMalloc((void **)&d_c, size);

  a = (int *)malloc(size);
  b = (int *)malloc(size);

  for(int i=0; i<N; i+=1){
    a[i] = i;
    b[i] = i;
  }

  cudaMemcpy(d_a, &a, size, cudaMemcpyHostToDevice);
  cudaMemcpy(d_b, &b, size, cudaMemcpyHostToDevice);

  add<<<1,N>>>(d_a, d_b, d_c);

  cudaMemcpy(c, d_c, size, cudaMemcpyDeviceToHost);

  free(a);
  free(b);
  free(c);

  cudaFree(d_a); cudaFree(d_b); cudaFree(d_c);


  return 0;
}
