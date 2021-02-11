#include<bits/stdc++.h>
#include"scan.h"
#include "readgml/readgml.h"
using namespace std;


void read_vlues(map<int, string>& values, string filepath)
{
    ifstream F(filepath);
    string line;
    int i=-1;
    while(getline(F, line))
    {
        if(line.find("value")!=string::npos)
        {
            i++;
            string word;
            stringstream ss(line);
            bool temp = false;
            while(ss>>word){if(temp){string s(1, word[1]);values[i] = s;}temp=true;}
        }
    }
}

void printClustersDEBUG(graph* G, map<int, string> &values)
{
    cout<<"CLUSTERS"<<endl;
    for(auto it=G->clusters.begin(); it!=G->clusters.end();it++)
    {
        cout<<it->first<<": ";
        vector<vertex*> temp = G->clusters[it->first];
        for(int i=0;i<temp.size();i++)
        {
            cout<<temp[i]->ID<<" "<<(values[temp[i]->ID])<<", ";
        }
        cout<<endl;
    }

    cout<<"HUBS: ";
    for(int i=0;i<G->hubs.size();i++)
    {
        cout<<G->hubs[i]->ID<<" "<<values[G->hubs[i]->ID]<<", ";
    }
    cout<<endl;

    cout<<"OUTLIERS: ";
    for(int i=0;i<G->outliers.size();i++)
    {
        cout<<G->outliers[i]->ID<<" "<<values[G->outliers[i]->ID]<<", ";
    }
    cout<<endl;
}

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
            // G->numOfNodes = N->nvertices;   // Number of vertices in graph
            // First add all vertices then add edges
            for(int i=0;i<N->nvertices;i++)
            {
                // cout<<"a"<<N->vertex[i].label<<endl;
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
            G->numofEdges = nEdges/2;
            G->printGraph();

            
            // create clusters and generates hubs and outliers
            scan *S = new scan(0.42, 2, G);
            S->execute();
            // G->printClusters();

            // DEBUGGING purposes
            map<int, string> values;
            read_vlues(values, argv[2]);
            printClustersDEBUG(G, values);

            cout<<endl;
            for(auto i: G->vertexMap)
            {
                cout<<i.first.first<<": ";
                vector<vertex*> temp = S->getEpsilonNeighbourhood(i.second);
                for(auto j: temp)
                {
                    cout<<j->ID<<" ";
                }
                cout<<endl;
            }

            cout<<endl;
            for(auto i: G->vertexMap)
            {
                for(auto j: G->vertexMap)
                {
                    cout<<i.first.first<<" "<<j.first.first<<" :";
                    cout<<S->calculateSimilarity(i.second,j.second)<<endl;
                }
                cout<<endl;
            }

            G->printClusters();
        }
    
    }
    if (strcmp(argv[1], "--MATRIX") == 0)
    {
        // NETWORK* N = new NETWORK();
        
        ifstream F(argv[2]);
        if (!F) perror ("Error opening file");
        else
        {
            graph* G = new graph();
            string line;
            int nvertices = 0;
            while(getline(F, line)){nvertices++;}
            for(int i=0;i<nvertices;i++)
            {
                G->addVertex(i, "");
                
            }
            cout<<G->numOfNodes<<endl;
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
                        G->addDirectedEdge(i, "", j, "");
                    }
                }
            }
            G->numofEdges = nEdges/2;
            G->printGraph();

            // create clusters and generates hubs and outliers
            scan *S = new scan(0.5, 2, G);
            S->execute();
            G->printClusters();
        }
    }
}


// 0.7, 2 in EXAMPLE dataset
// Map -> unordered map
// optimise

// void read_vlues(map<int, string>* values, string filepath)
// {
//     ifstream F(filepath);
//     string line;
//     while(getline(F, line))
//     {
//         // char* ptr = strstr((char*)line, "value");
//         if(line.find("value")!=string::npos)
//         {
//             cout<<line<<endl;
//         }
//     }
// }