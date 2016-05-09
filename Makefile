
all: main.cpp quadtree.cpp
	g++ -o main main.cpp quadtree.cpp

clean:
	rm *.o *~ quadtree
