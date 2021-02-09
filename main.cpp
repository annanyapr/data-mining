#include<bits/stdc++.h>
#include"scan.h"
#include "readgml/readgml.h"
using namespace std;

int main(int argc, char* argv[])
{
    // hard coded graph reading
    if (strcmp(argv[1], "--EXAMPLE")==0)
    {
        // Generate the graph
        graph *G = new graph();
        G->addVertex(0, "0");
        G->addVertex(1, "1");
        G->addVertex(2, "2");
        G->addVertex(3, "3");
        G->addVertex(4, "4");
        G->addVertex(5, "5");
        G->addVertex(6, "6");
        G->addVertex(7, "7");
        G->addVertex(8, "8");
        G->addVertex(9, "9");
        G->addVertex(10, "10");
        G->addVertex(11, "11");
        G->addVertex(12, "12");
        G->addVertex(13, "13");

        G->addEdge(13,"13",9,"9");
        G->addEdge(10,"10",9,"9");
        G->addEdge(8,"8",9,"9");
        G->addEdge(8,"8",7,"7");
        G->addEdge(8,"8",12,"12");
        G->addEdge(12,"12",9,"9");
        G->addEdge(12,"12",7,"7");
        G->addEdge(12,"12",10,"10");
        G->addEdge(12,"12",11,"11");
        G->addEdge(10,"10",11,"11");
        G->addEdge(7,"7",11,"11");
        G->addEdge(7,"7",6,"6");
        G->addEdge(11,"11",6,"6");
        G->addEdge(10,"10",6,"6");
        G->addEdge(3,"3",6,"6");
        G->addEdge(4,"4",6,"6");
        G->addEdge(0,"0",6,"6");
        G->addEdge(3,"3",4,"4");
        G->addEdge(0,"0",4,"4");
        G->addEdge(3,"3",5,"5");
        G->addEdge(4,"4",5,"5");
        G->addEdge(0,"0",5,"5");
        G->addEdge(3,"3",2,"2");
        G->addEdge(1,"1",0,"0");
        G->addEdge(1,"1",5,"5");
        G->addEdge(2,"2",5,"5");
        G->addEdge(2,"2",1,"1");

        G->printGraph();

        // create clusters and generates hubs and outliers
        scan *S = new scan(0.7, 2, G);
        S->execute();

        G->printClusters();
    }

    // via GML 
    if (strcmp(argv[1], "--GML")==0)
    {
        NETWORK* N = new NETWORK();
        FILE *F;

        F = fopen (argv[2], "r");
        if (F == NULL) perror ("Error opening file");
        else
        {
            int r = read_network(N, F);
            graph* G = new graph();
            G->numOfNodes = N->nvertices;   // Number of vertices in graph

            // First add all vertices then add edges
            for(int i=0;i<N->nvertices;i++)
            {
                G->addVertex(N->vertex[i].id, N->vertex[i].label);  // Add new vertex to graph with id and name
            }

            int nEdges = 0;
            for(int i=0;i<N->nvertices;i++)
            {
                for(int j=0;j<N->vertex[i].degree;j++)
                {
                    nEdges++;
                    // Find label of target edge
                    // N->vertex[N->vertex[i].edge[j].target].label
                    // Can't do this as ids may not start from 0
                    
                    // Adding directed edges as GML contains a->b and b->a both
                    G->addDirectedEdge(N->vertex[i].id, N->vertex[i].label, N->vertex[i].edge[j].target, find_label(N->vertex[i].edge[j].target, N));
                }
            }
            G->numofEdges = nEdges;
            G->printGraph();

            // create clusters and generates hubs and outliers
            scan *S = new scan(0.7, 2, G);
            S->execute();
            G->printClusters();
        }
    
    }
}