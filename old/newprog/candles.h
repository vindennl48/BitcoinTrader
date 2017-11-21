#ifndef CANDLES_H
#define CANDLES_H

#include "./bin/mth.h"




struct Candle{
  double high, low, open, close;
  inline Candle(double high, double low, double open, double close)
  : high(high),
    low(low),
    open(open),
    close(close){};

  inline v(double) get_vector(){
    v(double) result = {high, low, open, close};
    return result;
  };
};
typedef vector<Candle>  Candles;


extern const int NUM_CANDLES;
extern Candles   CANDLES;


inline Candles load_candles(){

  Candles v_candles;

  ifstream     candles_raw;
  print("If program freezes here.. make sure you included a candles_raw.txt file.");
  candles_raw.open("candles_raw.txt");
  string high, low, open, close;
  string::size_type sz;

  while(!candles_raw.eof()){
    try{
      getline(candles_raw, high);
      getline(candles_raw, low);
      getline(candles_raw, open);
      getline(candles_raw, close);
      v_candles.push_back(
        Candle( 
          stod(high, &sz), stod(low  , &sz),
          stod(open, &sz), stod(close, &sz) 
        )
      );
    } catch(...){ getline(candles_raw, close); };
  };

  return v_candles;

};


inline void set_candles(Candles &data_set){
  Candles result;

  int start_pos = rand()%(data_set.size()-NUM_CANDLES);
  // int start_pos = data_set.size() - 1 - NUM_CANDLES;

  result.insert(
    result.end(),
    data_set.begin() + start_pos,
    data_set.begin() + start_pos + NUM_CANDLES
  );

  CANDLES = result;

};



#endif
