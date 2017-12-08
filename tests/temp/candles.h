#ifndef CANDLES_H
#define CANDLES_H



#include "../../mth.h"



inline void
get_candles(int num_candles, cArray<double> *candles){
  ifstream        if_candles;
  v(double)       candles_temp;
  //cArray<double>  candles(num_candles);

  if_candles.open("candles_raw.txt");
  string STRING;

  while(!if_candles.eof()){
    loop(i, 4){
      try{
        getline(if_candles, STRING);
        try{ 
          double conv = atof(STRING.c_str());
          if(conv > 1)
            candles_temp.push_back(conv); 
        } 
        catch(...){ print("Errir converting string to double."); };
        STRING = "";
      }
      catch(...){
        print("candles_raw.txt has an uneven amount of lines!");
      };
    };
  };

  UINT size  = candles_temp.size();
  UINT units = num_candles*4;
  UINT j     = size-units;

  for(int i=0; i<units; i+=1){
    double t = candles_temp[j];
    //t = atof(dbl_to_s(t, 2).c_str());
    candles->h[i] = t;
    j += 1;
  };
  candles->size = units*sizeof(double);
};



#endif
