OBJS = main.o
CC = g++
DEBUG = -g
CFLAGS = -c -pthread $(DEBUG)
LFLAGS = $(DEBUG)

mr : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -pthread -o $@

%.o : %.cpp
	$(CC) $(CFLAGS) $^ -std=c++11

clean :
	rm *.o mr
