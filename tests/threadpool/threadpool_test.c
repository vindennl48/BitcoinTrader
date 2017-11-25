#include "../../threadpool.h"

typedef int*              vType;
typedef ThreadPool<vType> TPool;

void threadproc(vType x){
  *x += 45;
};

////////////////////////////////////////////////////////////////////////////////

bool test_threadpool(){
  print("\nStarted: \"test_process_thread_join()\"");
  TPool p(threadproc);

  v(vType) queue;
  loop(i, 10)
    queue.push_back(new int(i));
  p.add_to_queue(queue);
  p.join();

  int len_queue = queue.size();
  loop(i, len_queue)
    nprint(*queue[i] << ", ");
  nprint("\n");

  loop(i, queue.size())
    delete queue[i];

  print("Finished: \"test_process_thread_join()\"\n");
};




////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int main(){

  test_threadpool();

  return 0;
};

