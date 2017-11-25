#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "mth.h"


//////////////////////////
#define NTHREADS    4
//////////////////////////
#ifndef NTHREADS
extern const int NTHREADS;
#endif
//////////////////////////


// Struct TStats
////////////////////////////////////////////////////////////////////////////////
template <typename T>
struct TStats{
private:
  Mutex    *m, *m_running, *m_kill, *m_init, *m_queue;
  CV       *cv;
  bool     *init, *running, *kill;
public:
  v(T)   q;
  inline
  TStats(CV* new_cv){                                  // CONSTRUCTOR
    m         = new Mutex();
    m_running = new Mutex();
    m_kill    = new Mutex();
    m_init    = new Mutex();
    m_queue   = new Mutex();
    cv        = new_cv;
    init      = new bool(false);
    running   = new bool(false);
    kill      = new bool(false);
  };
  inline
  ~TStats(){                                           // DESTRUCTOR
    delete m;
    delete m_running;
    delete m_kill;
    delete m_init;
    delete m_queue;
    delete init;
    delete running;
    delete kill;
  };
  inline T
  pop(){                                               // pop()
    try{
      lock_guard<Mutex> lock(*m_queue);
      T result = q.back();
      q.pop_back();
      return result;
    } 
    catch(...){ print("Nothing left in Queue!"); };
  };
  inline void
  add_to_queue(T x){                                   // add_to_queue()  
    lock_guard<Mutex> lock(*m_queue);
    q.push_back(x);
  };
  inline bool
  get_queue(){
    lock_guard<Mutex> lock(*m_queue);
    return q.empty();
  };
  inline bool 
  get_init(){                                          // get_init()
    lock_guard<Mutex> lock(*m_init);
    return *init;
  };
  inline void 
  set_init(bool x){                                    // set_init()
    lock_guard<Mutex> lock(*m_init);
    *init = x;
  };
  inline bool 
  get_running(){                                       // get_running()
    lock_guard<Mutex> lock(*m_running);
    return *running;
  };
  inline void 
  set_running(bool x){                                 // set_running()
    lock_guard<Mutex> lock(*m_running);
    *running = x;
  };
  inline bool 
  get_kill(){                                          // get_kill()
    lock_guard<Mutex> lock(*m_kill);
    return *kill;
  };
  inline void 
  set_kill(bool x){                                    // set_kill()
    lock_guard<Mutex> lock(*m_kill);
    *kill = x;
  };
  inline void 
  wait(){                                              // wait()
    unique_lock<Mutex> lock(*m);
    cv->wait(lock, [this]{ return (!q.empty() || get_kill()); });
  };
};
////////////////////////////////////////////////////////////////////////////////


// Class ThreadPool
////////////////////////////////////////////////////////////////////////////////
template <typename T>
class ThreadPool{                                      // THREADPOOL
  TStats<T>*    main_stats;
  CV*           cv_main;
  int*          q_sort;
  v(thread)     pool;
  v(TStats<T>*) pool_stats;
  void(*proc)(T);

public:
  inline
  ThreadPool(void(*new_proc)(T)){                      // CONSTRUCTOR
    main_stats = new TStats<T>(cv_main);
    cv_main    = new CV();
    q_sort     = new int(0);
    proc       = new_proc;

    //pool.reserve(NTHREADS);
    loop(i, NTHREADS)
      pool_stats.push_back(new TStats<T>(cv_main));

    loop(i, NTHREADS){
      pool.push_back(thread(
        /*f()*/   process_thread, 
        /*args*/  pool_stats[i], proc, i
      ));
    };

    // wait for threads to initialize
    for(auto* t : pool_stats)
      while(!t->get_init());
  };

  inline
  ~ThreadPool(){                                       // DESTRUCTOR
    kill_threads();

    /* TStats* */    delete main_stats;
    /* CV*     */    delete cv_main;

    /* v(thread)  */ pool.clear();

    /* v(TStats*)    pool_stats */ 
      for(auto* s : pool_stats)
        delete s;
      pool_stats.clear();
  };

  inline void
  add_to_queue(T x){                                   // add_to_queue()

    pool_stats[*q_sort]->add_to_queue(x);
    cv_main->notify_all();

    *q_sort++;
    if(*q_sort > NTHREADS)
      *q_sort = 0;
  };

  inline void
  add_to_queue(v(T) x){                                // add_to_queue()

    int len_x = x.size();
    loop(i, len_x){
      pool_stats[*q_sort]->add_to_queue(x[i]);

      *q_sort += 1;
      if(*q_sort >= NTHREADS)
        *q_sort = 0;
    };
    cv_main->notify_all();
  };

  inline void
  join(){                                              // join()
    for(auto* t : pool_stats)
      while(t->get_running());
  };

  inline void
  kill_threads(){                                      // kill_threads()
    join();
    for(auto* t : pool_stats) t->set_kill(true);
    cv_main->notify_all();
    for(auto& t : pool)       t.join();
    print("threads killed");
  };

  inline static void
  process_thread(TStats<T> *s, 
  void(*proc)(T), int id){                             // process_thread()
    s->set_init(true);

    while(true){
      s->wait();
      if(s->get_kill())
        break;

      s->set_running(true);
      while( !s->get_queue() )
        (*proc)(s->pop());
      s->set_running(false);
    };
    //print("thread " << id << " killed...");
  };
};










#endif
