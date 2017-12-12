#define USING_CUDA
#include "../../mth.h"

// GLOBAL VARS
#define Na  20       /*num agents*/
#define Nn  (1<<7)   /*128: num neurons per agent*/
#define Np  (Nn*Nn)  /*num processes per agent*/


#include "kernels.h"

int main(){
  set_random_agents();

  int start = mtime();

  send_agents();
  loop(i, 20000){
    kernel_prepare<<<Na,Nn>>>(sums.d, reacts.d);
    kernel_fire<<<(Na*Nn),Nn>>>(sums.d, raw_sums.d, reacts.d, weights.d);
    kernel_add<<<(Na*Nn),Nn>>>(sums.d, raw_sums.d);
  };
  receive_agents();

  int end = mtime();
  print("Elapsed Time: " << (start-end) << "ms");

  loop(i, Nn) nprint(dbl_to_s(sums[i],2) << ", ");

  return 0;
}
