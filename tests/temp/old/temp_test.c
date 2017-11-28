#include "../../mth.h"

#define NTHREADS   3


inline void
process_thread(CV &cv, v(int*) &queue){
  int len_queue = queue.size();
  print("Size: " << len_queue);
  loop(i, len_queue){
    loop(j, 100)
      *queue[i] += 1;
  };
  queue.clear();
};


template <typename T>
inline bool multithread(v(T) *data);

int main(){

  v(int)  data;
  v(int*) queue;
  int len_data = 10000000;

  loop(i, len_data)
    data.push_back(i);

  int start, end;
  start = mtime();
  multithread<int>(&data);
  end = mtime();

  /////////////////////////////////////

  // for(auto& i : data)
  //   nprint(i << ", ");

  print("\n\nelapsed time: " << ((double)(start-end)/1000));

  return 0;
};


template <typename T>
inline bool multithread(v(T) *data){
  v(v(T*)) q(NTHREADS);

  int h = 0;
  int len_data = data->size();

  loop(i, len_data){
    q[h].push_back(&data->at(i));
    h += 1;
    if(h >= NTHREADS)
      h = 0;
  };

  CV cv;
  v(thread) threads;
  loop(i, NTHREADS){
    threads.push_back(
      thread( process_thread, ref(cv), ref(q[i]) )
    );
  };
  for(auto& t : threads) t.join();
};
