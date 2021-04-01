#include<bits/stdc++.h>
#include"Iscan/iscan.h"
#include"readgml/readgml.h"

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
            graph* iscanG = new graph();        // Single thread
            graph* iscanG2 = new graph();       // Mutlithread

            // First add all vertices then add edges
            for(int i=0;i<N->nvertices;i++)
            {
                // Add new vertex to graph with id and name
                scanG->addVertex(N->vertex[i].id, N->vertex[i].label); 
                iscanG->addVertex(N->vertex[i].id, N->vertex[i].label); 
                iscanG2->addVertex(N->vertex[i].id, N->vertex[i].label); 
            }

            int nEdges = 0;
            for(int i=0;i<N->nvertices;i++)
            {
                for(int j=0;j<N->vertex[i].degree;j++)
                {
                    nEdges++;
                
                    // Adding directed edges as Network contains a->b and b->a both
                    scanG->addDirectedEdge(N->vertex[i].id, N->vertex[i].edge[j].target);
                    iscanG->addDirectedEdge(N->vertex[i].id, N->vertex[i].edge[j].target);
                    iscanG2->addDirectedEdge(N->vertex[i].id, N->vertex[i].edge[j].target);
                }
            }
            scanG->numofEdges = nEdges/2;
            iscanG->numofEdges = nEdges/2;
            iscanG2->numofEdges = nEdges/2;
            nEdges/=2;


            iscan *iscanObject = new iscan(stof(argv[3]), stoi(argv[4]), iscanG);
            iscan *iscanObject2 = new iscan(stof(argv[3]), stoi(argv[4]), iscanG2);

            auto start = chrono::steady_clock::now();
            iscanObject->executeSCAN(true);
            auto end = chrono::steady_clock::now();

            auto start_2 = chrono::steady_clock::now();
            iscanObject2->executeSCAN();
            auto end_2 = chrono::steady_clock::now();

            cout<<"Initial clustering Done"<<endl;
            iscanG->printClusters();
            cout<<"--------------------------------"<<endl;

            double incrementalTime = 0;
            double incrementalTime2 = 0;
            double scanTime = 0;
            for(int i=0;i<N->nvertices;i++)
            {
                for(int j=0;j<N->vertex[i].degree;j++)
                {
                    if(scanG->findEdge(N->vertex[i].id, N->vertex[i].edge[j].target))
                    {
                        cout<<"Deleting edge between "<<N->vertex[i].id<< " & "<<N->vertex[i].edge[j].target<<endl;
                        nEdges-=1;
                        scanG->numofEdges = nEdges;
                        iscanG->numofEdges = nEdges;
                        iscanG2->numofEdges = nEdges;
                    
                        // Adding directed edges as Network contains a->b and b->a both
                        scanG->removeEdge(N->vertex[i].id, N->vertex[i].edge[j].target);
                        
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
                        currentG->numofEdges = nEdges;
                                            
                        cout<<"Graph structure after deleting edge"<<endl;
                        currentG->printGraph();cout<<endl;
                        iscan *scanObject = new iscan(stof(argv[3]), stoi(argv[4]), currentG);
                        cout<<"Clustering by SCAN:"<<endl;
                        auto start = chrono::steady_clock::now();
                        scanObject->executeSCAN();
                        auto end = chrono::steady_clock::now();
                        auto diff = end - start;
                        scanTime += chrono::duration <double, milli> (diff).count();
                        currentG->printClusters();
                        
                        cout<<"\n\nIncremental Clustering by ISCAN:"<<endl;
                        /*Single thread*/
                        start = chrono::steady_clock::now();
                        iscanObject->updateEdge(N->vertex[i].id, N->vertex[i].edge[j].target, 0,false);
                        end = chrono::steady_clock::now();
                        diff = end - start;
                        incrementalTime += chrono::duration <double, milli> (diff).count();
                        
                        /*Multi Thread*/
                        start = chrono::steady_clock::now();
                        iscanObject2->updateEdge(N->vertex[i].id, N->vertex[i].edge[j].target, 0, true);
                        end = chrono::steady_clock::now();
                        diff = end - start;
                        incrementalTime2 += chrono::duration <double, milli> (diff).count();
                        
                        iscanG->printClusters();
                        cout<<"--------------------------------"<<endl;

                        delete(currentG);
                        delete(scanObject);
                    }
                    
                    
                }
            }       
            cout<<"--------------------------------"<<endl;
            cout<<"Running time:"<<endl;
            cout<<"SCAN Time:"<<scanTime<<endl;
            cout<<"Incremental SCAN Time without parallel threads:"<<incrementalTime<<endl;
            cout<<"Incremental SCAN Time with parallel threads:"<<incrementalTime2<<endl;
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
            graph* iscanG2 = new graph();

            // Loading initial graph
            while(getline(F, line)){nvertices++;}
            for(int i=0;i<nvertices;i++)
            {
                scanG->addVertex(i, "");
                iscanG->addVertex(i, "");
                iscanG2->addVertex(i, "");
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
                        scanG->addDirectedEdge(i, j);
                        iscanG->addDirectedEdge(i, j);
                        iscanG2->addDirectedEdge(i, j);

                    }
                }
            }
            scanG->numofEdges = nEdges/2;
            iscanG->numofEdges = nEdges/2;
            iscanG2->numofEdges = nEdges/2;
            nEdges/=2;


            iscan *iscanObject = new iscan(stof(argv[3]), stoi(argv[4]), iscanG);
            iscan *iscanObject2 = new iscan(stof(argv[3]), stoi(argv[4]), iscanG2);

            auto start = chrono::steady_clock::now();
            iscanObject->executeSCAN(true);
            auto end = chrono::steady_clock::now();

            auto start_2 = chrono::steady_clock::now();
            iscanObject2->executeSCAN();
            auto end_2 = chrono::steady_clock::now();

            cout<<"Initial clustering Done"<<endl;
            iscanG->printClusters();
            cout<<"--------------------------------"<<endl;

            double incrementalTime = 0;
            double incrementalTime2 = 0;
            double scanTime = 0;

            for(int i=0;i<nvertices;i++)
            {
                for(int j=0;j<nvertices;j++)
                {
                    if(scanG->findEdge(i,j))
                    {
                        cout<<"Deleting edge between "<<i<< " & "<<j<<endl;
                        nEdges-=1;
                        scanG->numofEdges = nEdges;
                        iscanG->numofEdges = nEdges;
                        iscanG2->numofEdges = nEdges;
                        
                        // Adding directed edges as Network contains a->b and b->a both
                        scanG->removeEdge(i, j);
                        
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
                        currentG->numofEdges = nEdges;
                                            
                        cout<<"Graph structure after adding edge"<<endl;
                        currentG->printGraph();cout<<endl;

                        iscan *scanObject = new iscan(stof(argv[3]), stoi(argv[4]), currentG);
                        cout<<"\n\nClustering by SCAN:"<<endl;
                        auto start = chrono::steady_clock::now();
                        scanObject->executeSCAN();
                        auto end = chrono::steady_clock::now();
                        auto diff = end - start;
                        scanTime += chrono::duration <double, milli> (diff).count();
                        currentG->printClusters();
                        
                        cout<<"\n\nIncremental Clustering by ISCAN:"<<endl;
                        // Single thread
                        start = chrono::steady_clock::now();
                        iscanObject->updateEdge(i, j, 0, false);
                        end = chrono::steady_clock::now();
                        diff = end - start;
                        incrementalTime += chrono::duration <double, milli> (diff).count();

                        // Multi thread
                        start = chrono::steady_clock::now();
                        iscanObject2->updateEdge(i, j, 0, true);
                        end = chrono::steady_clock::now();
                        diff = end - start;
                        incrementalTime2 += chrono::duration <double, milli> (diff).count();
                        iscanG2->printClusters();
                        cout<<"--------------------------------"<<endl;

                        delete(currentG);
                        delete(scanObject);
                    }
                        

                    
                }
            }
            cout<<"--------------------------------"<<endl;
            cout<<"Running time:"<<endl;
            cout<<"SCAN Time:"<<scanTime<<endl;
            cout<<"Incremental SCAN Time without parallel threads:"<<incrementalTime<<endl;
            cout<<"Incremental SCAN Time with parallel threads:"<<incrementalTime2<<endl;

        }
    }

}