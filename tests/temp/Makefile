CXXFLAGS= -std=c++11 -O0
NVCCFLAGS= -std=c++11 -O0
NAME= main

all: $(NAME).cu
	nvcc $(NAME).cu -o $(NAME) $(NVCCFLAGS)

clean:
	rm $(NAME)
