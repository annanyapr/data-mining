#include<bits/stdc++.h>
#include"scan.h"
using namespace std;

int main()
{
    // vertex* v1 = new vertex(1, "1");
    // vertex* v2 = new vertex(2, "2");
    // vertex* v3 = new vertex(3, "3");
    // vertex* v4 = new vertex(4, "4");
    // vertex* v5 = new vertex(5, "5");
    // vertex* v6 = new vertex(6, "6");
    // vertex* v7 = new vertex(7, "7");
    // vertex* v8 = new vertex(8, "8");
    // vertex* v9 = new vertex(9, "9");
    // vertex* v10 = new vertex(10, "10");
    // vertex* v11 = new vertex(11, "11");
    // vertex* v12 = new vertex(12, "12");
    // vertex* v13 = new vertex(13, "13");

    graph *G = new graph();
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
    // scan *S = new scan(0.65, 2, G);
    // S->execute();

    // G->printClusters();
    
}