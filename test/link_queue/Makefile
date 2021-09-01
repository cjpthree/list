INC = -I../../include

CCFLAGS = -D LOCK_TYPE=1
# -Wall -O -g
LDFLAGS :=
# -lubus -lubox -ljson-c -lpthread -lcurl -lblobmsg_json -lcrypto -ldl
EXEC=test

all:$(EXEC)

OBJS=\
	link_queue_test.o

%.o: %.c
	$(CC) $(CCFLAGS) -c $< $(INC)

$(EXEC): $(OBJS)
	$(CC) $(CCFLAGS)  -o $@ $^   $(LDFLAGS)

clean:
	rm -f *.o  $(EXEC)
