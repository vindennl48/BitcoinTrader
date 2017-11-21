#include <iostream>
#include <sstream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <string>
#include <algorithm>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;


#ifndef MTH_H
#define MTH_H


//  DEFINITIONS
#define loop(x,y)  for(int x = 0; x < y; x++)
#define print(x)   std::cout << x << std::endl
#define nprint(x)  std::cout << x
#define round(x,y) double(int(x*(pow(10,y))))/(pow(10,y))
#define RAND_UNIFORM ((double)(rand()%100)/100)
#define CLEAR system("clear;clear;")
#define PAUSE {                            \
  int x123;                                \
  std::cout<<"Press Enter To Continue..."; \
  std::cin.ignore();                       \
}
#define v(x)          std::vector< x >
//  END DEFINITIONS


#define CHRONO std::chrono
inline int mtime(){
  auto now = CHRONO::system_clock::now();
  auto ms = CHRONO::time_point_cast<CHRONO::milliseconds>(now).time_since_epoch().count();
  ms = ms*-1;
  return ms;
};


inline string dbl_to_s(double num, int precision){
  string result;
  
  stringstream stream;
  stream << fixed << setprecision(precision) << num;
  result = stream.str();

  return result;
};

inline string uppercase(string &x){
  transform(
    x.begin(), x.end(), x.begin(), ::toupper
  );
  return x;
};

////////////////////////////////////////////////////////////////////////////////
inline string set_length_raw(string x, int length){
  //print("String: " << x << ", Size: " << x.size());
  //PAUSE;
  if(x.size() >= length)
    return x;

  stringstream result;

  int spaces = length - x.size();
  loop(i, spaces)
    result << " ";
  result << x;

  return result.str();
};

inline string set_length(int y, int length){
  stringstream w;
  w << y;
  string x = w.str();
  return set_length_raw(x, length);
};

inline string set_length(double y, int length){
  stringstream w;
  w << y;
  string x = w.str();
  return set_length_raw(x, length);
};

inline string set_length(string x, int length){
  return set_length_raw(x, length);
};
////////////////////////////////////////////////////////////////////////////////


#endif
