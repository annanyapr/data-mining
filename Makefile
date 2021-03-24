make:
	g++ -std=c++11 -pthread -g readgml/readgml.c comparision.cpp -o comp
makeDelete:
	g++ -std=c++11 -pthread -g readgml/readgml.c comparison_deletion.cpp -o compDeletion
clean:
	rm comp intermediate.txt