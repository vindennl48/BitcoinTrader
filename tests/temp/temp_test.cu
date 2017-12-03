#include <iostream>
#include <math.h>
#include "../../mth.h"

typedef unsigned long int INT;


// block size and amount
#define T     1024
#define B(x)  (x+T-1)/T


// Kernel
////////////////////////////////////////////////////////////////////////////////
__global__
void add(INT *a, INT *b, INT *c, INT size){
  INT index = threadIdx.x + blockDim.x * blockIdx.x;
  if(index < size)
    c[index] = a[index] + b[index];
  //for(INT i=0; i<size; i+=1)
  //  c[i] = a[i] + b[i];
};
////////////////////////////////////////////////////////////////////////////////


int main(){
  INT N = /*elements*/ 2048*2048*10;
  cArray<INT> a(N), b(N), c(N);

  for(INT i=0; i<N; i+=1){
    a.h[i] = i;
    b.h[i] = i;
  }

  INT start = mtime();
  a.send(); b.send();
  add<<<B(N),T>>>(a.d, b.d, c.d, N);
  c.receive();
  INT end = mtime();

  print("\n" << "Elapsed Time: " << (start-end) << "ms" << "\n");


  for(INT i=(N-10); i<N; i+=1)
    std::cout << a.h[i] << ", ";
  std::cout << std::endl;

  for(INT i=(N-10); i<N; i+=1)
    std::cout << c.h[i] << ", ";

  return 0;
}
