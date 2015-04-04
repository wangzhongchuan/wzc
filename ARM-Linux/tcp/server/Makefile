EXEC = server
OBJS = server.o
SRC  = server.c

CC = gcc
CFLAGS += -O2 -Wall
LDFLAGS +=-lpthread 

all:$(EXEC)

$(EXEC):$(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -vf $(EXEC) *.o *~
