CC = g++
CFLAGS = -Wall -g
OBJECTS = CPU.o \
	Instructions.o \
	Memory.o \
	main.o

all: sevcsikc

sevcsikc: $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

%.o : %.cc
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJECTS) sevcsikc html latex

doc:
	doxygen

