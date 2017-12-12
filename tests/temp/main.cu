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
  loop(j, 10){
    print("Hey");
    loop(i, 10) nprint(dbl_to_s(sums[i],2) << ", ");
    loop(i, 10000){
      kernel_prepare<<<Na,Nn>>>(sums.d, reacts.d);
      kernel_fire<<<(Na*Nn),Nn>>>(sums.d, raw_sums.d, reacts.d, weights.d);
      kernel_add<<<(Na*Nn)/2,Nn>>>(sums.d, raw_sums.d);
    };
    receive_agents();
  };

  int end = mtime();

  print("\nElapsed Time: " << (start-end) << "ms");


  return 0;
}
