// first cuda program

#include <iostream>
#include <math.h>

__global__
void add(int N, double *x, double*y)
{
  for (int i = 0; i < N; i++)
    y[i] = x[i] + y[i];
}

int main()
{
  int N = 1<<20;
  double a = 5;
  double *x, *y;

  cudaMallocManaged(&x, N*sizeof(double));
  cudaMallocManaged(&y, N*sizeof(double));

  for (int i = 0; i < N; i++)
  {
    x[i] = 1.0;
    y[i] = 2.0;
  }

  x[0] = &a;

  add<<<1, 1>>>(N, x, y);
  cudaDeviceSynchronize();

  double maxError = 0.0;
  for (int i = 0; i < N; i++)
    maxError = fmax(maxError, fabs(y[i]-3.0));
  std::cout << "Max error: " << maxError << std::endl;

  cudaFree(x);
  cudaFree(y);

  return 0;
}
