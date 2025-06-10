####################################
# CS:APP T2 SB
# Makefile that builds peqcomp
####################################

CC = gcc
CFLAGS = -Wall -O0

# Build the buffer bomb
peqcomp: peqcomp.c TestePeqcomp.c peqcomp.h
	$(CC) $(CFLAGS) -fno-stack-protector -Wa,--execstack -o peqcomp peqcomp.c TestePeqcomp.c 

# Use this when you want to recompile
clean:
	-rm -f *.o peqcomp *~

