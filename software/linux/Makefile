PROG = tiny85_hid_pwm

TARGET = $(PROG)
CC = gcc
STRIP = strip
CFLAGS = -Wall -O2 -DOS_LINUX
LIBS = -lusb

OBJS = $(PROG).o hid.o

all: clean $(TARGET)

$(PROG): $(OBJS)
	$(CC) -o $(PROG) $(OBJS) $(LIBS)
	$(STRIP) $(PROG)

hid.o: hid.c hid.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o $(PROG)
	rm -rf tmp


