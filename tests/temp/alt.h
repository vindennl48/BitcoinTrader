#ifndef ALT_H
#define ALT_H

#define host_device __host__ __device__

#define array_operators(x,y) __host__ __device__ \
x operator [] (unsigned long int i) const {      \
  return y[i];                                   \
};                                               \
__host__ __device__                              \
x& operator [] (unsigned long int i){            \
  return y[i];                                   \
};

struct Data{
  double dataframes[Nd][Ndp] = {{0}};
  UINT   counter = -1;
  array_operators(double[Ndp],dataframes);
  __host__
  Data(){
    loop(i,Nd){
      loop(j,Ndp)
        dataframes[i][j] = ((double)(rand()%200)-100);
    };
  };
};

struct Neuron{
  double sum          = 0;
  double react        = 0;
  double raw_sums[Nn] = {0};
  double weights[Nn]  = {0};
  host_device
  Neuron(){ loop(i,Nn) weights[i]=((double)(rand()%200)-100)/100 };
};

struct Brain{
  Neuron neurons[Nn];
  array_operators(Neuron, neurons);
};

struct Trade{
  double in_trade = 0;
  double buy_at   = 0;
  double sell_at  = 0;
  double amount   = 0;
};

struct Agent{
  Brain brain;
  Trade trade;
  double fit_score = 100;
};

struct Agents{
  Agent *h;
  Agent *d;

  //Data *h_data;
  //Data *d_data;

  __host__
  Agents(){
    h = (Agent*)malloc(Na*sizeof(Agent));
    cudaMalloc((void **)&d, Na*sizeof(Agent));
  };

  __host__
  ~Agents(){ free(h); cudaFree(d); };

  host_device
  Agent operator [] (unsigned long int i) const {
    #ifdef __CUDA_ARCH__
      return d[i];
    #else
      return h[i];
    #endif
  };
  host_device
  Agent& operator [] (unsigned long int i){
    #ifdef __CUDA_ARCH__
      return d[i];
    #else
      return h[i];
    #endif
  };

  __host__
  Agent* send(){
    cudaMemcpy(d,h,size,cudaMemcpyHostToDevice);
    return d;
  };

  __host__
  void receive(){
    cudaMemcpy(h,d,size,cudaMemcpyDeviceToHost);
  };

};

// Kernels
////////////////////////////////////////////////////////////////////////////////
#define GET_TIDX UINT tidx = blockIdx.x**blockDim.x+threadIdx.x
#define GET_I    UINT i    = threadIdx.x
#define GET_B    UINT b    = blockIdx.x

__global__ void                                /*kernel_prepare*/
kernel_prepare(Agent *agents){
  GET_B; GET_I;
  Neuron *n = &agents[b].brain[i];
  n->react = (1/(1+exp(-n->sum)))-.5;
  n->sum = 0;
  __syncthreads();
};

__global__ void                                /*kernel_fire*/
kernel_fire(Agent *agents){
  UINT a = (UINT)blockIdx.x/Nn;
  GET_B; GET_I;
  Neuron *n1 = &agents[a].brain[b];
  Neuron *n2 = &agents[a].brain[i];

  if(b!=i)
    n1->raw_sum[i] = n2->react * n2->weight[b];
  else
    n1->raw_sum[i] = 0;
};

__global__ void                                /*kernel_add*/
kernel_add(Agent *agents){
  volatile __shared__ double sums1[Nn];
  volatile __shared__ double sums2[Nn];
  UINT a = (UINT)blockIdx.x/Nn;
  GET_B; GET_I;
  sums1[i] = agents[a].brain[b].weights[i];
  sums2[i] = agents[a].brain[b+(Nn/2)].weights[i];
  __syncthreads();

  if(i<Nn/2){
    for(UINT s=Nn/2; s>0; s>>=1){
      if(i<s)
        sums1[i] += sums1[i+s];
      __syncthreads();
    };
    if(i==0)
      agents[a].brain[b].sum = sums1[0];
  }
  else{
    i -= Nn/2;
    for(UINT s=Nn/2; s>0; s>>=1){
      if(i<s)
        sums2[i] += sums2[i+s];
      __syncthreads();
    };
    if(i==0)
      agents[a].brain[b+(Nn/2)].sum = sums2[0];
  };
};

__global__ void                                /*kernel_set_inputs*/
kernel_set_inputs(Agent *agents, Data *data){
  UINT a = blockIdx.x; GET_I;
  double data_frame[Ndp] = data[data->counter];
  if(i<Ndp) agents[a].brain[i].sum = data[i];
};

__global__ void                                /*kernel_set_fitness*/
kernel_set_fitness(Agent *agents, Data *data){
  UINT a = threadIdx.x;
  if(a<Na){
    Trade  *trade     = &agents[a].trade;
    double *fit_score = &agents[a].fit_score;
    Neuron *n         = &agents[a].brain[Nn-1];

    double result = n->sum;
    result = 1/(1+exp(-result))-.5;

    if(result>0 && trade->in_trade == 0){
      /*buy*/
      trade->in_trade = 1;
      trade->buy_at   = data[data->counter][3];  /*candle close*/
      trade->amount   = *fit_score;
    }
    else if(result<=0 && trade->in_trade == 1){
      /*sell*/
      trade->in_trade = 0;
      trade->sell_at  = data[data->counter][3];  /*candle close*/
      double ratio    = trade->sell_at / trade->buy_at;
      *fit_score      = ratio * trade->amount;
    };
  };
  __syncthreads();
};

// Functions
////////////////////////////////////////////////////////////////////////////////
inline void                                    /*agents_prepare*/
agents_prepare(Agents agents){ kernel_prepare<<<Na,Nn>>>(agents.d); };
inline void                                    /*agents_fire*/
agents_fire(Agents agents){ kernel_fire<<<(Na*Nn),Nn>>>(agents.d); };
inline void                                    /*agents_add*/
agents_add(Agents agents){ kernel_fire<<<(Na*Nn)/2,Nn>>>(agents.d); };
inline void                                    /*agents_set_inputs*/
agents_set_inputs(Agents agents){
  agents.d_data->next();
  kernel_set_inputs<<<Na,Nn>>>(agents.d, agents.d_data);
};
inline void                                    /*agents_set_fitness*/
agents_set_fitness(Agents agents){ kernel_set_fitness<<<1,Nn>>>(agents.d); };
inline void                                    /*agents_run_generation*/
agents_run_generation(Agents agents){
  agents_set_inputs(agents);
  agents_prepare(agents);
  agents_fire(agents);
  agents_add(agents);
  agents_set_fitness(agents);
};
////////////////////////////////////////////////////////////////////////////////


#undef array_operators
#undef host_device
#undef GET_TIDX
#undef GET_I
#undef GET_B


#endif
