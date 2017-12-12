#define USING_CUDA
#include "../../mth.h"

// GLOBAL VARS
#define Na   20       /*num agents*/
#define Nn   (1<<7)   /*128: num neurons per agent*/
#define Np   (Nn*Nn)  /*num processes per agent*/
#define Nd   10000    /*num dataobj*/
#define Ndp  6        /*num datapoints in each dataobj*/


#include "kernels.h"

int main(){
  set_random_agents();
  set_random_data();

  int start = mtime();

  send_agents();
  loop(i, 100)
    loop(i, Nd) agents_run_generation();
  receive_agents();

  loop(i, Na*Nn) nprint(dbl_to_s(sums[i],2) << ", ");

  int end = mtime();

  print("\nElapsed Time: " << (start-end) << "ms");


  return 0;
}
