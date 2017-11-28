#include "../../brain.h"


bool test_constructor_destructor(int x){
  print("\nStarted: \"test_constructor_destructor()\"");

  Brain brain(x);
  brain.run(100);
  print(brain.get_str());

  print("Finished: \"test_constructor_destructor()\"\n");
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int main(){
  int num_neurons = 300;

  int start, end;

  start = mtime();
  test_constructor_destructor(num_neurons);
  end = mtime();

  print("Time Elapsed: " << dbl_to_s((double)(start-end)/1000, 2) << "sec");

  return 0;
};

