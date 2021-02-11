To compile program:
`$make`

To remove main and intermediate.txt file:
`$make clean`

To give GML file as input:
`./main --GML filepath epsilon mu`

E.g.: `./main --GML polbooks/polbooks.gml 0.41 2`


To give adjacency matrix as input from txt file
`./main --MATRIX filepath epsilon mu`

E.g.: `./main --MATRIX football/football.txt 0.7 2`
