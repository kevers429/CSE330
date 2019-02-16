OBJS = StateNode.o utils.o NFA.o DFA.o main.o
CC = g++
DEBUG = -g
CFLAGS = -c $(DEBUG)
LFLAGS = $(DEBUG)

search : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $@

%.o : %.cpp
	$(CC) $(CFLAGS) $^ -std=c++11

clean :
	rm *.o search
