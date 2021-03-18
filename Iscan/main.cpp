#include<bits/stdc++.h>
#include"iscan.h"
#include "../readgml/readgml.h"

using namespace std;

int main(int argc, char* argv[])
{
    // Input taken from GML 
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

            // First add all vertices then add edges
            for(int i=0;i<N->nvertices;i++)
            {
                // Add new vertex to graph with id and name
                G->addVertex(N->vertex[i].id, N->vertex[i].label); 
            }

            int nEdges = 0;
            for(int i=0;i<N->nvertices;i++)
            {
                for(int j=0;j<N->vertex[i].degree;j++)
                {
                    nEdges++;
                
                    // Adding directed edges as Network contains a->b and b->a both
                    G->addDirectedEdge(N->vertex[i].id, N->vertex[i].edge[j].target);
                }
            }
            G->numofEdges = nEdges/2;
            G->printGraph();

            
            if(stof(argv[3])>1 || stof(argv[3])<=0){cout<<"Epsilon value should be between 0 and 1"<<endl;exit(0);}
            if(stoi(argv[4])<=0){cout<<"Mu value should be greater than 0"<<endl;exit(0);}

            // create clusters and generates hubs and outliers
            iscan *IS = new iscan(stof(argv[3]), stoi(argv[4]), G);
            IS->executeSCAN();
            G->printClusters();
            cout<<"Vertices information"<<endl;
            G->printVertices();

            cout<<"Initial clustering done."<<endl;
            cout<<"Number of edges to add/delete:"<<endl;
            
            int numupdates, update, src, dest;
            cin>>numupdates;
            for(int i=0;i<numupdates;i++)
            {
                // Check if vertex exists or not and same for edge
                cin>>update>>src>>dest;
                IS->updateEdge(src, dest, update);   
                // G->printVertices();
            }

            G->printClusters();

        }
    
    }

    // Input taken from txt file as adjacency matrix
    if (strcmp(argv[1], "--MATRIX") == 0)
    {        
        ifstream F(argv[2]);
        if (!F) perror ("Error opening file");
        else
        {
            graph* G = new graph();
            string line;
            int nvertices = 0;

            // Count number of lines as number of vertices present in graphs
            while(getline(F, line)){nvertices++;}
            for(int i=0;i<nvertices;i++)
            {
                G->addVertex(i, "");
            }

            // Point file pointer to beginning of file
            F.clear();
            F.seekg(0);

            int temp;
            int nEdges = 0;
            for(int i=0;i<nvertices;i++)
            {
                for(int j=0;j<nvertices;j++)
                {
                    F>>temp;
                    if(temp==1)
                    {
                        nEdges++;
                        // If 1 present in matrix then add edge in graph
                        G->addDirectedEdge(i, j);
                    }
                }
            }
            G->numofEdges = nEdges/2;
            G->printGraph();

            // create clusters and generates hubs and outliers
            if(stof(argv[3])>1 || stof(argv[3])<=0){cout<<"Epsilon value should be between 0 and 1"<<endl;exit(0);}
            if(stoi(argv[4])<=0){cout<<"Mu value should be greater than 0"<<endl;exit(0);}
            
            iscan *IS = new iscan(stof(argv[3]), stoi(argv[4]), G);
            IS->executeSCAN();
            cout<<"SCAN Results:"<<endl;
            G->printClusters();


            cout<<"Vertices information"<<endl;
            G->printVertices();

            cout<<"Initial clustering done."<<endl;
            cout<<"Number of edges to add/delete:"<<endl;
            
            int numupdates, update, src, dest;
            cin>>numupdates;
            for(int i=0;i<numupdates;i++)
            {
                // Check if vertex exists or not and same for edge
                cin>>update>>src>>dest;
                IS->updateEdge(src, dest, update);   
                // G->printVertices();
            }

            G->printClusters();
        }
    }

    if(strcmp(argv[1], "--LINK") == 0)
    {
        ifstream F(argv[2]);
        if (!F) perror ("Error opening file");
        else
        {
            graph* G = new graph();
            string line;
            int nedges = 0;
            while(getline(F, line)){nedges++;}
            F.clear();
            F.seekg(0);
            int id1, id2;
            for(int i=0;i<nedges;i++)
            {
                F>>id1>>id2;
                if(G->vertexMap.find(id1)==G->vertexMap.end())
                {
                    G->addVertex(id1, "");
                }
                if(G->vertexMap.find(id2)==G->vertexMap.end())
                {
                    G->addVertex(id2, "");
                }
                G->addEdge(id1, id2);
            }

            G->printGraph();

            
            if(stof(argv[3])>1 || stof(argv[3])<=0){cout<<"Epsilon value should be between 0 and 1"<<endl;exit(0);}
            if(stoi(argv[4])<=0){cout<<"Mu value should be greater than 0"<<endl;exit(0);}

            // create clusters and generates hubs and outliers
            iscan *IS = new iscan(stof(argv[3]), stoi(argv[4]), G);
            IS->executeSCAN();
            G->printClusters();


            cout<<"Vertices information"<<endl;
            G->printVertices();

            cout<<"Initial clustering done."<<endl;
            cout<<"Number of edges to add/delete:"<<endl;
            
            int numupdates, update, src, dest;
            cin>>numupdates;
            for(int i=0;i<numupdates;i++)
            {
                // Check if vertex exists or not and same for edge
                cin>>update>>src>>dest;
                IS->updateEdge(src, dest, update);   
                // G->printVertices();
            }

            G->printClusters();
        }
    }
}

/*
    Dataset links:
        http://snap.stanford.edu/data/index.html
        http://law.di.unimi.it/datasets.php

*/