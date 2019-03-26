OBJS = StateNode.o utils.o NFA.o DFA.o main.o
CC = g++
DEBUG = -g
CFLAGS = -c -fopenmp $(DEBUG)
LFLAGS = $(DEBUG)

psearch : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -fopenmp -o $@

%.o : %.cpp
	$(CC) $(CFLAGS) $^ -std=c++11

clean :
	rm *.o psearch
