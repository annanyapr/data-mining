#include<bits/stdc++.h>
#include"Iscan/iscan.h"
#include "readgml/readgml.h"

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
            if(stof(argv[3])>1 || stof(argv[3])<=0){cout<<"Epsilon value should be between 0 and 1"<<endl;exit(0);}
            if(stoi(argv[4])<=0){cout<<"Mu value should be greater than 0"<<endl;exit(0);}

            int r = read_network(N, F);
            graph* scanG = new graph();
            graph* iscanG = new graph();


            // First add all vertices then add edges
            for(int i=0;i<N->nvertices;i++)
            {
                // Add new vertex to graph with id and name
                scanG->addVertex(N->vertex[i].id, N->vertex[i].label); 
                iscanG->addVertex(N->vertex[i].id, N->vertex[i].label); 
            }

            int nEdges = 0;
            scanG->numofEdges = nEdges;
            iscanG->numofEdges = nEdges;


            iscan *iscanObject = new iscan(stof(argv[3]), stoi(argv[4]), iscanG);

            iscanObject->executeSCAN();
            cout<<"Initial Iscan clustering Done"<<endl;

            double incrementalTime = 0;
            double scanTime = 0;
            for(int i=0;i<N->nvertices;i++)
            {
                for(int j=0;j<N->vertex[i].degree;j++)
                {
                    if(scanG->findEdge(N->vertex[i].id, N->vertex[i].edge[j].target))
                    {
                        continue;
                    }
                    cout<<"Adding edge between "<<N->vertex[i].id<< " & "<<N->vertex[i].edge[j].target<<endl;
                    nEdges++;
                
                    // Adding directed edges as Network contains a->b and b->a both
                    scanG->addEdge(N->vertex[i].id, N->vertex[i].edge[j].target);
                    
                    graph* currentG = new graph();

                    for(auto it:scanG->graphObject)
                    {
                        currentG->addVertex((it.first)->ID,(it.first)->name);
                    }
                    for(auto iter = scanG->graphObject.begin(); iter != scanG->graphObject.end(); iter++)
                    {
                        for(auto it = iter->second.begin(); it!=iter->second.end();it++)
                        {
                            currentG->addDirectedEdge((iter->first)->ID,(*it)->ID);
                        }
                    }
                                        
                    iscan *scanObject = new iscan(stof(argv[3]), stoi(argv[4]), currentG);
                    cout<<"Clustering by SCAN:"<<endl;
                    auto start = chrono::steady_clock::now();
                    scanObject->executeSCAN();
                    auto end = chrono::steady_clock::now();
                    auto diff = end - start;
                    scanTime += chrono::duration <double, milli> (diff).count();
                    currentG->printClusters();
                    
                    cout<<"\n\nIncremental Clustering by ISCAN:"<<endl;
                    start = chrono::steady_clock::now();
                    iscanObject->updateEdge(N->vertex[i].id, N->vertex[i].edge[j].target, 1);
                    end = chrono::steady_clock::now();
                    diff = end - start;
                    incrementalTime += chrono::duration <double, milli> (diff).count();
                    iscanG->printGraph();
                    // iscanG->printVertices();
                    iscanG->printClusters(); 
                    
                }
            }       

            cout<<"SCAN Time:"<<scanTime<<endl;
            cout<<"Incremental Time:"<<incrementalTime<<endl;

        }
    }

    // Input taken from txt file as adjacency matrix
    if (strcmp(argv[1], "--MATRIX") == 0)
    {        
        ifstream F(argv[2]);
        if (!F) perror ("Error opening file");
        else
        {

            string line;
            int nvertices = 0;

            if(stof(argv[3])>1 || stof(argv[3])<=0){cout<<"Epsilon value should be between 0 and 1"<<endl;exit(0);}
            if(stoi(argv[4])<=0){cout<<"Mu value should be greater than 0"<<endl;exit(0);}

            graph* scanG = new graph();
            graph* iscanG = new graph();


            while(getline(F, line)){nvertices++;}
            for(int i=0;i<nvertices;i++)
            {
                scanG->addVertex(i, "");
                iscanG->addVertex(i, "");
            }
            F.clear();
            F.seekg(0);

            int nEdges = 0;
            scanG->numofEdges = nEdges;
            iscanG->numofEdges = nEdges;


            iscan *iscanObject = new iscan(stof(argv[3]), stoi(argv[4]), iscanG);

            iscanObject->executeSCAN();
            cout<<"Initial Iscan clustering Done"<<endl;

            double incrementalTime = 0;
            double scanTime = 0;
            int temp;
            // cout<<nvertices<<endl;
            for(int i=0;i<nvertices;i++)
            {
                for(int j=0;j<nvertices;j++)
                {
                    F>>temp;
                    if(temp==1)
                    {
                        // cout<<"A"<<endl;
                        if(scanG->findEdge(i,j))
                        {
                            continue;
                        }
                        nEdges++;
                        cout<<"Adding edge between "<<i<< " & "<<j<<endl;
                
                        // Adding directed edges as Network contains a->b and b->a both
                        scanG->addDirectedEdge(i, j);
                        
                        graph* currentG = new graph();

                        for(auto it:scanG->graphObject)
                        {
                            currentG->addVertex((it.first)->ID,(it.first)->name);
                        }
                        for(auto iter = scanG->graphObject.begin(); iter != scanG->graphObject.end(); iter++)
                        {
                            for(auto it = iter->second.begin(); it!=iter->second.end();it++)
                            {
                                currentG->addDirectedEdge((iter->first)->ID,(*it)->ID);
                            }
                        }
                                            
                        iscan *scanObject = new iscan(stof(argv[3]), stoi(argv[4]), currentG);
                        cout<<"\n\nClustering by SCAN:"<<endl;
                        auto start = chrono::steady_clock::now();
                        scanObject->executeSCAN();
                        auto end = chrono::steady_clock::now();
                        auto diff = end - start;
                        scanTime += chrono::duration <double, milli> (diff).count();
                        currentG->printClusters();
                        
                        cout<<"\n\nIncremental Clustering by ISCAN:"<<endl;
                        start = chrono::steady_clock::now();
                        iscanObject->updateEdge(i, j, 1);
                        end = chrono::steady_clock::now();
                        diff = end - start;
                        incrementalTime += chrono::duration <double, milli> (diff).count();
                        iscanG->printGraph();
                        iscanG->printClusters(); 
                    }
                }
            }
        }
    }

}

/*
    Dataset links:
        http://snap.stanford.edu/data/index.html
        http://law.di.unimi.it/datasets.php

*/