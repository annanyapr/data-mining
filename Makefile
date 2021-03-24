make:
	g++ -std=c++11 -pthread -g readgml/readgml.c comparision.cpp -o comp
makeDelete:
	g++ -std=c++11 -pthread -g readgml/readgml.c comparison_deletion.cpp -o compDeletion
makecompscan:
	g++ -std=c++11 -pthread -g readgml/readgml.c comparison_scan.cpp -o compscan
makecompadd:
	g++ -std=c++11 -pthread -g readgml/readgml.c comparison_add.cpp -o compadd
makecompdel:
	g++ -std=c++11 -pthread -g readgml/readgml.c comparison_del.cpp -o compdel
clean:
	rm comp intermediate.txt