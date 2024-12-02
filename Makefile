CFLAGS = -Wall -Werror -Wpedantic -std=c++20 -O0 -g
CC = g++

OBJECTS = main.o bubbleSort.o selectionSort.o insertionSort.o mergeSort.o merge.o

run-tests: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.cpp insertionSort.o bubbleSort.o mergeSort.o merge.o

bubbleSort.o: bubbleSort.cpp

selectionSort.o: selectionSort.cpp

insertionSort.o: insertionSort.cpp

mergeSort.o: mergeSort.cpp merge.o

merge.o: merge.cpp

clean:
	rm -f run-tests *.o *~
