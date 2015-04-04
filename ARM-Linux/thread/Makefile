EXEC = thread
OBJS = thread.o
SRC  = thread.c

CC = arm-linux-gcc
CFLAGS += -O2 -Wall
LDFLAGS +=-lpthread 

all:$(EXEC)

$(EXEC):$(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -vf $(EXEC) *.o *~
