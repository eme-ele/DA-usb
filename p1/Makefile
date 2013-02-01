.PHONY: all clean

CPP = g++

all: gcp1

clean:
	rm -f gcp1 *.o

%.o: %.cpp
	$(CPP) -c $< -o $@

gcp1: graph.o 
