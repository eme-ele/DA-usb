.PHONY: all clean

CPP = g++

all: precision

clean:
	rm -f precision *.o

%.o: %.cpp
	$(CPP) -c $< -o $@

precision: distancias.o
