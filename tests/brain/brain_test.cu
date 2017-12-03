#include "../../brain.h"
#include "../../addArray.h"


void test_constructor_destructor(int x){
  print("\nStarted: \"test_constructor_destructor()\"");

  Brain brain(x);
  brain.run(2000);

  print("Finished: \"test_constructor_destructor()\"\n");
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int main(){

  int N = 5000;
  cArray<int> a(N);
  loop(i, N)
    a.h[i] = i;

  int result = addArray<int>(N, a);
  print("Result: " << result);


  // int num_neurons = 300;

  // int start, end;

  // Brain brain(num_neurons);
  // start = mtime();
  // //test_constructor_destructor(num_neurons);
  // brain.run(2000);
  // end = mtime();

  // print("Time Elapsed: " << (start-end) << "ms");

  // return 0;
};

