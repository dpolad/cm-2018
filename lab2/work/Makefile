CC = gcc

CFLAGS = -Wall -mmmx -msse -msse2 -mssse3 -msse4 -mavx -march=native -fopt-info-vec -flto

# Utility routines

all: lab2.o 

lab2.o: lab2.c 
	$(CC) $(CFLAGS) -I /home/davide/libpfc/include -O0 $< -o 0_$@ -Wl,-L/home/davide/libpfc/build/src,-lpfc
	$(CC) $(CFLAGS) -I /home/davide/libpfc/include -O1 $< -o 1_$@ -Wl,-L/home/davide/libpfc/build/src,-lpfc 
	$(CC) $(CFLAGS) -I /home/davide/libpfc/include -O2 $< -o 2_$@ -Wl,-L/home/davide/libpfc/build/src,-lpfc 
	$(CC) $(CFLAGS) -I /home/davide/libpfc/include -O3 $< -o 3_$@ -Wl,-L/home/davide/libpfc/build/src,-lpfc 

dump: lab2.o
	objdump -D lab2.o

clean:
	rm -f *.o 

