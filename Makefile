make:
	g++ -g readgml/readgml.c comparision.cpp -o comp
clean:
	rm comp intermediate.txt