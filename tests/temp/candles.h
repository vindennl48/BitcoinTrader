#ifndef CANDLES_H
#define CANDLES_H

#include "../../mth.h"


inline void
get_candles(int num_candles, cArray<double> *candles){
  v(string) candles_file = get_file("candles_raw.txt");

  UINT size  = candles_file.size();
  UINT units = num_candles*4;
  UINT j     = size-units;

  for(int i=0; i<units; i+=1){
    double t = atof(candles_file[j].c_str());
    candles->h[i] = t;
    j += 1;
  };
  candles->size = units*sizeof(double);
};



#endif
