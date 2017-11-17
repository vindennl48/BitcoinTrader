// my personal testing script
#include "mth.h"
using namespace mth;


int main(){

  auto ms = time();
  loop(i, 500000000){
    int a = 5235 * 1224;
    int b = 5235 * 1224;
    int c = 5235 * 1224;
    int d = 5235 * 1224;
  };
  auto da = time();

  print(ms);
  print(da);
  double ans = (ms-da) / 1000;
  print("Difference: " << ans << "sec");

};
