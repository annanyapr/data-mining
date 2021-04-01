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
            graph* iscanG = new graph();
            graph* iscanG2 = new graph();


            // First add all vertices then add edges
            for(int i=0;i<N->nvertices;i++)
            {
                // Add new vertex to graph with id and name
                scanG->addVertex(N->vertex[i].id, N->vertex[i].label); 
                iscanG->addVertex(N->vertex[i].id, N->vertex[i].label); 
                iscanG2->addVertex(N->vertex[i].id, N->vertex[i].label);
            }



            int nEdges = 0;
            scanG->numofEdges = nEdges;
            iscanG->numofEdges = nEdges;
            iscanG2->numofEdges = nEdges;

            iscan *iscanObject = new iscan(stof(argv[3]), stoi(argv[4]), iscanG);
            iscan *iscanObject2 = new iscan(stof(argv[3]), stoi(argv[4]), iscanG2);

            double incrementalTime = 0;
            double incrementalTime2 = 0;
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
                    nEdges+=1;
                    scanG->numofEdges = nEdges;
                    iscanG->numofEdges = nEdges;
                    iscanG2->numofEdges = nEdges;
                
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
                    currentG->numofEdges = nEdges;
                                        
                    cout<<"Graph structure after adding edge"<<endl;
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
                    /*MultiThread*/
                    start = chrono::steady_clock::now();
                    iscanObject->updateEdge(N->vertex[i].id, N->vertex[i].edge[j].target, 1,true);
                    end = chrono::steady_clock::now();
                    diff = end - start;
                    incrementalTime += chrono::duration <double, milli> (diff).count();
                    
                    /*Single Thread*/
                    start = chrono::steady_clock::now();
                    iscanObject2->updateEdge(N->vertex[i].id, N->vertex[i].edge[j].target, 1);
                    end = chrono::steady_clock::now();
                    diff = end - start;
                    incrementalTime2 += chrono::duration <double, milli> (diff).count();
                    
                    iscanG2->printClusters();
                    cout<<"--------------------------------"<<endl;                    
                }
            }       
            cout<<"--------------------------------"<<endl;
            cout<<"Running time:"<<endl;
            cout<<"SCAN Time:"<<scanTime<<endl;
            cout<<"Incremental SCAN Time with parallel threads:"<<incrementalTime<<endl;
            cout<<"Incremental SCAN Time without parallel threads:"<<incrementalTime2<<endl;
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

            while(getline(F, line)){nvertices++;}
            for(int i=0;i<nvertices;i++)
            {
                scanG->addVertex(i, "");
                iscanG->addVertex(i, "");
                iscanG2->addVertex(i, "");
            }
            F.clear();
            F.seekg(0);

            int nEdges = 0;
            scanG->numofEdges = nEdges;
            iscanG->numofEdges = nEdges;
            iscanG2->numofEdges = nEdges;


            iscan *iscanObject = new iscan(stof(argv[3]), stoi(argv[4]), iscanG);
            iscan *iscanObject2 = new iscan(stof(argv[3]), stoi(argv[4]), iscanG2);

            double incrementalTime = 0;
            double incrementalTime2 = 0;
            double scanTime = 0;
            int temp;
            for(int i=0;i<nvertices;i++)
            {
                for(int j=0;j<nvertices;j++)
                {
                    F>>temp;
                    if(temp==1)
                    {
                        if(scanG->findEdge(i,j))
                        {
                            continue;
                        }
                        nEdges+=1;
                        scanG->numofEdges = nEdges;
                        iscanG->numofEdges = nEdges;
                        iscanG2->numofEdges = nEdges;
                        cout<<"Adding edge between "<<i<< " & "<<j<<endl;
                
                        // Adding directed edges as Network contains a->b and b->a both
                        scanG->addEdge(i, j);
                        
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
                                            
                        iscan *scanObject = new iscan(stof(argv[3]), stoi(argv[4]), currentG);
                        auto start = chrono::steady_clock::now();
                        scanObject->executeSCAN();
                        auto end = chrono::steady_clock::now();
                        auto diff = end - start;
                        scanTime += chrono::duration <double, milli> (diff).count();
                        
                        /*MultiThread*/
                        start = chrono::steady_clock::now();
                        iscanObject->updateEdge(i, j, 1,true);
                        end = chrono::steady_clock::now();
                        diff = end - start;
                        incrementalTime += chrono::duration <double, milli> (diff).count();
                        
                        /*Single Thread*/
                        start = chrono::steady_clock::now();
                        iscanObject2->updateEdge(i, j, 1);
                        end = chrono::steady_clock::now();
                        diff = end - start;
                        incrementalTime2 += chrono::duration <double, milli> (diff).count();
                        
                    }
                }
            }
            cout<<"--------------------------------"<<endl;
            cout<<"Running time:"<<endl;
            cout<<"SCAN Time:"<<scanTime<<endl;
            cout<<"Incremental SCAN Time with parallel threads:"<<incrementalTime<<endl;
            cout<<"Incremental SCAN Time without parallel threads:"<<incrementalTime2<<endl;

        }
    }

    if(strcmp(argv[1], "--LINK") == 0)
    {
        ifstream F(argv[2]);
        if (!F) perror ("Error opening file");
        else
        {
            if(stof(argv[3])>1 || stof(argv[3])<=0){cout<<"Epsilon value should be between 0 and 1"<<endl;exit(0);}
            if(stoi(argv[4])<=0){cout<<"Mu value should be greater than 0"<<endl;exit(0);}

            graph* scanG = new graph();
            graph* iscanG = new graph();
            graph* iscanG2 = new graph();

            string line;
            int nedges = 0;

            while(getline(F, line)){nedges++;}
            F.clear();
            F.seekg(0);
            int id1, id2;
            for(int i=0;i<nedges;i++)
            {
                F>>id1>>id2;
                if(scanG->vertexMap.find(id1)==scanG->vertexMap.end())
                {
                    scanG->addVertex(id1, "");
                    iscanG->addVertex(id1, "");
                    iscanG2->addVertex(id1, "");
                }
                if(scanG->vertexMap.find(id2)==scanG->vertexMap.end())
                {
                    scanG->addVertex(id2, "");
                    iscanG->addVertex(id2, "");
                    iscanG2->addVertex(id2, "");
                }
            }

            int updateEdgeNumber = 10;

            scanG->numofEdges = nedges;
            iscanG->numofEdges = nedges-updateEdgeNumber;
            iscanG2->numofEdges = nedges-updateEdgeNumber;

            F.clear();
            F.seekg(0);
            int i, j;
            for(int k=0;k<nedges-updateEdgeNumber;k++)
            {
                F>>i>>j;

                // Adding directed edges as Network contains a->b and b->a both
                scanG->addEdge(i, j);
                iscanG->addEdge(i, j);
                iscanG2->addEdge(i, j);

            }

            iscan *scanObject = new iscan(stof(argv[3]), stoi(argv[4]), iscanG);
            iscan *iscanObject = new iscan(stof(argv[3]), stoi(argv[4]), iscanG);
            iscan *iscanObject2 = new iscan(stof(argv[3]), stoi(argv[4]), iscanG2,4);


            double incrementalTime = 0;
            double incrementalTime2 = 0;
            double scanTime = 0;

            scanObject->executeSCAN();

            // /*MultiThread*/
            iscanObject->executeSCAN();
            // /*Single Thread*/
            iscanObject2->executeSCAN(true);
            
            for(int k=0;k<updateEdgeNumber;k++)
            {
                F>>i>>j;

                cout<<"Adding edge between "<<i<< " & "<<j<<endl;
        
                // Adding directed edges as Network contains a->b and b->a both
                scanG->addEdge(i, j);

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
                currentG->numofEdges = nedges+k+1;
                                    
                iscan *tempScanObject = new iscan(stof(argv[3]), stoi(argv[4]), currentG);
                auto start = chrono::steady_clock::now();
                tempScanObject->executeSCAN();
                auto end = chrono::steady_clock::now();
                auto diff = end - start;
                scanTime += chrono::duration <double, milli> (diff).count();
                
                /*MultiThread*/
                start = chrono::steady_clock::now();
                iscanObject->updateEdge(i, j, 1);
                end = chrono::steady_clock::now();
                diff = end - start;
                incrementalTime += chrono::duration <double, milli> (diff).count();
                
                /*Single Thread*/
                start = chrono::steady_clock::now();
                iscanObject2->updateEdge(i, j, 1, true);
                end = chrono::steady_clock::now();
                diff = end - start;
                incrementalTime2 += chrono::duration <double, milli> (diff).count();

                delete(tempScanObject);
                delete(currentG);
                
            }

            cout<<"--------------------------------"<<endl;
            cout<<"Running time:"<<endl;
            cout<<"SCAN Time:"<<scanTime<<endl;
            cout<<"Incremental SCAN Time without parallel threads:"<<incrementalTime<<endl;
            cout<<"Incremental SCAN Time  with parallel threads:"<<incrementalTime2<<endl;
        }
    }

}
