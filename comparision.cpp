#include<bits/stdc++.h>
#include"Iscan/iscan.h"
#include"readgml/readgml.h"

using namespace std;

void checkClusters(map<int,vector<vertex*>> , map<int,vector<vertex*>>);

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

            // auto start = chrono::steady_clock::now();
            // iscanObject->executeSCAN(true);
            // auto end = chrono::steady_clock::now();

            // auto start_2 = chrono::steady_clock::now();
            // iscanObject2->executeSCAN();
            // auto end_2 = chrono::steady_clock::now();
            // cout<<"Initial Iscan clustering Done"<<endl;


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
                    iscanG->printClusters();
                    cout<<"--------------------------------"<<endl;

                    // Checking if two clusters are same
                    // map<int,vector<vertex*>> SCANClusters  = currentG->clusters;
                    // map<int,vector<vertex*>> ISCANClusters = iscanG->clusters;
                    // map<int,vector<vertex*>> ISCANClusters2 = iscanG2->clusters;
                    // checkClusters(SCANClusters, ISCANClusters);
                    
                }
            }       
            cout<<"--------------------------------"<<endl;
            cout<<"Running time:"<<endl;
            cout<<"SCAN Time:"<<scanTime<<endl;
            cout<<"Incremental SCAN Time with parallel threads:"<<incrementalTime<<endl;
            cout<<"Incremental SCAN Time without parallel threads:"<<incrementalTime2<<endl;
            // cout<<"Initial SCAN Time with parallel threads: "<< chrono::duration <double, milli> (end-start).count() << endl; 
            // cout<<"Initial SCAN Time without parallel threads: "<< chrono::duration <double, milli> (end_2-start_2).count() << endl; 

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

            // iscanObject->executeSCAN();
            // cout<<"Initial Iscan clustering Done"<<endl;

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
                                            
                        // cout<<"Graph structure after adding edge"<<endl;
                        // currentG->printGraph();cout<<endl;

                        iscan *scanObject = new iscan(stof(argv[3]), stoi(argv[4]), currentG);
                        // cout<<"\n\nClustering by SCAN:"<<endl;
                        auto start = chrono::steady_clock::now();
                        scanObject->executeSCAN();
                        auto end = chrono::steady_clock::now();
                        auto diff = end - start;
                        scanTime += chrono::duration <double, milli> (diff).count();
                        // currentG->printClusters();
                        
                        // cout<<"\n\nIncremental Clustering by ISCAN:"<<endl;
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
                        
                        // iscanG2->printClusters();
                        // iscanG->printClusters();

                        // cout<<"--------------------------------"<<endl;

                        // map<int,vector<vertex*>> SCANClusters  = currentG->clusters;
                        // map<int,vector<vertex*>> ISCANClusters = iscanG->clusters;
                        // map<int,vector<vertex*>> ISCANClusters2 = iscanG2->clusters;
                        // checkClusters(SCANClusters, ISCANClusters);

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
            scanG->numofEdges = 0;
            iscanG->numofEdges = 0;
            iscanG2->numofEdges = 0;

            iscan *iscanObject = new iscan(stof(argv[3]), stoi(argv[4]), iscanG);
            iscan *iscanObject2 = new iscan(stof(argv[3]), stoi(argv[4]), iscanG2);


            double incrementalTime = 0;
            double incrementalTime2 = 0;
            double scanTime = 0;
            int temp;
            int curEdges = 0;
            F.clear();
            F.seekg(0);
            int i, j;
            for(int k=0;k<nedges;k++)
            {
                F>>i>>j;
                curEdges+=1;
                scanG->numofEdges = curEdges;
                iscanG->numofEdges = curEdges;
                iscanG2->numofEdges = curEdges;
                cout<<"Adding edge between "<<i<< " & "<<j<<" edge number"<<curEdges <<endl;
        
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
                currentG->numofEdges = curEdges;
                                    
                // cout<<"Graph structure after adding edge"<<endl;
                // currentG->printGraph();
                // cout<<endl;

                iscan *scanObject = new iscan(stof(argv[3]), stoi(argv[4]), currentG);
                // cout<<"\n\nClustering by SCAN:"<<endl;
                auto start = chrono::steady_clock::now();
                scanObject->executeSCAN();
                auto end = chrono::steady_clock::now();
                auto diff = end - start;
                scanTime += chrono::duration <double, milli> (diff).count();
                // currentG->printClusters();

                
                // cout<<"\n\nIncremental Clustering by ISCAN:"<<endl;
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
                
                // iscanG2->printClusters();
                // iscanG->printClusters();

                // cout<<"--------------------------------"<<endl;

                // map<int,vector<vertex*>> SCANClusters  = currentG->clusters;
                // map<int,vector<vertex*>> ISCANClusters = iscanG->clusters;
                // map<int,vector<vertex*>> ISCANClusters2 = iscanG2->clusters;
                // checkClusters(SCANClusters, ISCANClusters);
                delete(scanObject);
                delete(currentG);

            }
            cout<<"--------------------------------"<<endl;
            cout<<"Running time:"<<endl;
            cout<<"SCAN Time:"<<scanTime<<endl;
            cout<<"Incremental SCAN Time with parallel threads:"<<incrementalTime<<endl;
            cout<<"Incremental SCAN Time without parallel threads:"<<incrementalTime2<<endl;
        }
    }

}

void checkClusters(map<int,vector<vertex*>> s, map<int,vector<vertex*>>i)
{
    bool val = true;
    if(s.size()!=i.size()){val = false;}
    for(auto it=s.begin(); it!=s.end();it++)
    {
        int minID = (it)->second[0]->ID;
        int cID = -1;
        for(auto it1:i)
        {
            for(auto it2:(it1).second)
            {
                if(minID == it2->ID)
                {
                    cID = it2->clusterId;
                    break;
                }
            }
        }

        if(cID == -1){val = false;break;}
        if(it->second.size() != i[cID].size()){val = false;}
        for(auto it1:it->second)
        {
            bool temp = false;
            for(auto it2:i[cID])
            {
                if(it1->ID == (it2)->ID)
                {
                    temp = true;
                    break;
                }
            }
            if(!temp)
            {
                val = false;
                break;
            }

        }


    }
    assert(val);

}
/*
    Dataset links:
        http://snap.stanford.edu/data/index.html
        http://law.di.unimi.it/datasets.php

*/



/*
Comparison matrices:

1. comparing scan(on whole graph once) with iscan(intial execute scan on k edges and updation on n-k edges) (without multithreading)
2. comparing scan(on whole graph once) with iscan(intial execute scan on k edges and updation on n-k edges) (with multithreading)
3. comparing scan with multithreading and without multithreading
5. comparing on edge addition
6. comparing on edge deletion
7. comparing by changing epsilon values

*/