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
            IS->executeSCAN(1);
            G->printClusters();

            cout<<"Initial clustering done."<<endl;
            cout<<"Number of updates"<<endl;
            cout<<"Edge/vertex(0/1)  add/delete(0/1)  id1 id2"<<endl;
            
            int numupdates, edge_vertex,update, src, dest, id;
            cin>>numupdates;
            for(int i=0;i<numupdates;i++)
            {
                // Check if vertex exists or not and same for edge
                cin>>edge_vertex;
                if(edge_vertex)
                {
                    // Vertex
                    cin>>update>>id;
                    if(update)
                    {
                        // Vertex delete
                        // First check if vertex exists
                        if(G->vertexMap.find(id)==G->vertexMap.end())
                        {
                            cout<<"Vertex not present in graph."<<endl;
                            continue;
                        }
                        vector<vertex*> neighbours = G->graphObject[G->vertexMap[id]];
                        for(auto it:neighbours)
                        {
                            // Removing all the edges corresponding to this vertex
                            IS->updateEdge(id, it->ID, 0, 1);
                        }
                        G->graphObject.erase(G->vertexMap[id]);
                        G->outliers.erase(remove(G->outliers.begin(), G->outliers.end(), G->vertexMap[id]), G->outliers.end());
                        G->vertexMap.erase(id);

                    }
                    else
                    {
                        // Vertex add
                        G->addVertex(id, "");
                        G->outliers.push_back(G->vertexMap[id]);
                        G->vertexMap[id]->hub_or_outlier = 1;
                    }
                }
                else
                {
                    // Edge
                    // First check if edge exists by checking if both vertices exist in graph or not
                    if(G->vertexMap.find(src) == G->vertexMap.end() || G->vertexMap.find(dest) == G->vertexMap.end())
                    {
                        cout<<"Edge not present in graph."<<endl;
                        continue;
                    }
                    cin>>update>>src>>dest;
                    IS->updateEdge(src, dest, !update); 
                }
                  
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
            G->printClusters();


            cout<<"Initial clustering done."<<endl;
            cout<<"Number of updates"<<endl;
            cout<<"Edge/vertex(0/1)  add/delete(0/1)  id1 id2"<<endl;
            
            int numupdates, edge_vertex,update, src, dest, id;
            cin>>numupdates;
            for(int i=0;i<numupdates;i++)
            {
                // Check if vertex exists or not and same for edge
                cin>>edge_vertex;
                if(edge_vertex)
                {
                    // Vertex
                    cin>>update>>id;
                    if(update)
                    {
                        // Vertex delete
                        // First check if vertex exists
                        if(G->vertexMap.find(id)==G->vertexMap.end())
                        {
                            cout<<"Vertex not present in graph."<<endl;
                            continue;
                        }
                        vector<vertex*> neighbours = G->graphObject[G->vertexMap[id]];
                        for(auto it:neighbours)
                        {
                            // Removing all the edges corresponding to this vertex
                            IS->updateEdge(id, it->ID, 0, 1);
                        }
                        G->graphObject.erase(G->vertexMap[id]);
                        G->outliers.erase(remove(G->outliers.begin(), G->outliers.end(), G->vertexMap[id]), G->outliers.end());
                        G->vertexMap.erase(id);

                    }
                    else
                    {
                        // Vertex add
                        G->addVertex(id, "");
                        G->outliers.push_back(G->vertexMap[id]);
                        G->vertexMap[id]->hub_or_outlier = 1;
                    }
                }
                else
                {
                    // Edge
                    // First check if edge exists by checking if both vertices exist in graph or not
                    if(G->vertexMap.find(src) == G->vertexMap.end() || G->vertexMap.find(dest) == G->vertexMap.end())
                    {
                        cout<<"Edge not present in graph."<<endl;
                        continue;
                    }
                    cin>>update>>src>>dest;
                    IS->updateEdge(src, dest, !update); 
                }
            }
            G->printClusters();         
        }
       
    
    }

    // TODO: add updates section in --LINK
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


            // cout<<"Vertices information"<<endl;
            // G->printVertices();

            cout<<"Initial clustering done."<<endl;
            cout<<"Number of updates"<<endl;
            cout<<"Edge/vertex(0/1)  add/delete(0/1)  id1 id2"<<endl;
            
            int numupdates, edge_vertex,update, src, dest, id;
            cin>>numupdates;
            for(int i=0;i<numupdates;i++)
            {
                // Check if vertex exists or not and same for edge
                cin>>edge_vertex;
                if(edge_vertex)
                {
                    // Vertex
                    cin>>update>>id;
                    if(update)
                    {
                        // Vertex delete
                        // First check if vertex exists
                        if(G->vertexMap.find(id)==G->vertexMap.end())
                        {
                            cout<<"Vertex not present in graph."<<endl;
                            continue;
                        }
                        vector<vertex*> neighbours = G->graphObject[G->vertexMap[id]];
                        for(auto it:neighbours)
                        {
                            // Removing all the edges corresponding to this vertex
                            IS->updateEdge(id, it->ID, 0, 1);
                        }
                        G->graphObject.erase(G->vertexMap[id]);
                        G->outliers.erase(remove(G->outliers.begin(), G->outliers.end(), G->vertexMap[id]), G->outliers.end());
                        G->vertexMap.erase(id);

                    }
                    else
                    {
                        // Vertex add
                        G->addVertex(id, "");
                        G->outliers.push_back(G->vertexMap[id]);
                        G->vertexMap[id]->hub_or_outlier = 1;
                    }
                }
                else
                {
                    // Edge
                    // First check if edge exists by checking if both vertices exist in graph or not
                    if(G->vertexMap.find(src) == G->vertexMap.end() || G->vertexMap.find(dest) == G->vertexMap.end())
                    {
                        cout<<"Edge not present in graph."<<endl;
                        continue;
                    }
                    cin>>update>>src>>dest;
                    IS->updateEdge(src, dest, !update); 
                }   
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