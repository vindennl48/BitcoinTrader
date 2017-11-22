
/*  
 * brain:
 *   neuron 1:
 *     add to neuron 2
 *     add to neuron 3
 *     add to neuron 4
 *   neuron 2:
 *     add to neuron 1
 *     add to neuron 3
 *     add to neuron 4
 *   neuron 3:
 *     add to neuron 1
 *     add to neuron 2
 *     add to neuron 4
 *   neuron 4:
 *     add to neuron 1
 *     add to neuron 2
 *     add to neuron 3
 *
 * 1 neuron at a time -> threadpool
 *   - calculations get separated into n number of queues for n
 *     number of threads.
 *   - threads process their queue.
 * repeat with rest of neurons.
 *
 * 
 * */

#ifndef NEURON_H
#define NEURON_H

#include "mth.h"


//////////////////////////
#define NTHREADS    4
//////////////////////////
#ifndef NTHREADS
extern const int NTHREADS;
#endif
//////////////////////////


struct Neuron{                                         // STRUCT NEURON
private:

  Mutex       *m;
  int*        index_id;
  static int* length;
  double*     sum;
  double*     react;

  v(Neuron*)* neighbors;
  v(double*)* weights;

  // ThreadPool                                        // THREADPOOL
  //////////////////////
  static v(thread*) pool;
  static v(v(int*)) queue;

  inline static
  void initialize_threadpool(int length){              // create_threads
    pool.reserve(NTHREADS);
    queue.reserve(NTHREADS);

    loop(i, NTHREADS){
      pool.push_back(new thread(
        /*place thread here*/
      ));
      queue[i].reserve(length);
      loop(j, length) queue[i][j] = new int(-1);
    };

  };
  inline static
  void destroy_threadpool(){                           // destroy_threads
    for(auto& t : pool) t->join();
    loop(i, NTHREADS){
      delete pool[i];
      loop(j, *length) delete queue[i][j];
    };
  };
  //////////////////////

public:
  inline Neuron(int index_id, int length){             // CONSTRUCTOR
    this->m         = new Mutex();
    this->index_id  = new int(index_id);
    Neuron::length  = new int(length);
    this->sum       = new double(0);
    this->react     = new double(0);
    this->neighbors = new v(Neuron*);
    this->weights   = new v(double*);

    this->neighbors->reserve(length);
    this->weights->reserve(length);

    initialize_threadpool(length);
  };

  inline ~Neuron(){                                    // DESTRUCTOR
    delete m;
    delete index_id;
    delete length;
    delete sum;
    delete react;
    delete neighbors;

    loop(i, weights->size())
      delete weights->at(i);
    weights->clear();
    delete weights;
    destroy_threadpool();
  };

  inline void set_neighbors(v(Neuron*) neurons){      // set_neighbors()
    for(auto& n : neurons)
      neighbors->push_back(n);
  };

  inline void reset_weights(){                         // reset_weights()
    for(auto& w : *weights)
      *w = ((double)(rand()%200)-100)/100;
  };

  inline void reset_weight(int weight_id){             // reset_weight()
    *(weights->at(weight_id)) = ((double)(rand()%200)-100)/100;
  };

  inline void prepare(){                               // prepare()
    *react = round(1/(1+exp(-*sum)),4);
    *sum   = 0;
  };

  inline void fire(){                                  // fire()
    int len_weights = weights->size();
    if(*react != 0){

      int i = 0;
      for(auto& w : *weights){

        if(*index_id != i){
          Neuron* neuron = neighbors->at(i);
          neuron->add_to_sum(*react * *w);
        };

        i++;
      };

    };
  };

  inline void add_to_sum(double value){                // add_to_sum()
    lock_guard<Mutex> lock(*m);
    *sum += value;
  };

  inline string get_str(){                             // get_str()
    stringstream result;
    result << "Idx: " << set_length(*index_id,            3) << ", ";
    result <<  "Sz: " << set_length((int)weights->size(), 3) << ", ";
    result <<   "S: " << set_length(dbl_to_s(*sum,  2),   7) << ", ";
    result <<   "R: " << set_length(dbl_to_s(*react,2),   4) << " | ";
    result <<   "W: ";
    loop(i, 5)
      result << set_length(dbl_to_s(*(weights->at(i)), 2), 6) << ", ";
    result << "..";
    return result.str();
  };

  inline string get_data(){                            // get_data()
    stringstream result; 

    result << *index_id << endl;
    for(auto& w : *weights)
      result << w << endl;

    result << "end neuron" << endl;
    return result.str();
  };

};


#endif


int main(){
  return 0;
};

