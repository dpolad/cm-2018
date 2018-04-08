CC = gcc

CFLAGS = -Wall -mmmx -msse -msse2 -mssse3 -msse4 -mavx -march=native -fopenmp

# Utility routines

all: lab2.o lab2_omp.o

lab2.o: lab2.c 
	$(CC) $(CFLAGS) -O0 $< -o 0_$@ 
	$(CC) $(CFLAGS) -O1 $< -o 1_$@ 
	$(CC) $(CFLAGS) -O2 $< -o 2_$@ 
	$(CC) $(CFLAGS) -O3 $< -o 3_$@ 

lab2_omp.o: lab2_omp.c 
	$(CC) $(CFLAGS) -DOMP -O0 $< -o 0_$@ 
	$(CC) $(CFLAGS) -DOMP -O1 $< -o 1_$@ 
	$(CC) $(CFLAGS) -DOMP -O2 $< -o 2_$@ 
	$(CC) $(CFLAGS) -DOMP -O3 $< -o 3_$@ 

dump: lab2.o
	objdump -D lab2.o

clean:
	rm -f *.o 

