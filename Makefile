make:
	g++ -std=c++11 -pthread -g readgml/readgml.c comparision.cpp -o comp
clean:
	rm comp intermediate.txt