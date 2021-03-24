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
            graph* iscanG = new graph();        // Single thread
            graph* iscanG2 = new graph();       // Mutlithread 2
            graph* iscanG3 = new graph();       // Mutlithread 4
            graph* iscanG4 = new graph();       // Mutlithread 8

            // First add all vertices then add edges
            for(int i=0;i<N->nvertices;i++)
            {
                // Add new vertex to graph with id and name
                iscanG->addVertex(N->vertex[i].id, N->vertex[i].label); 
                iscanG2->addVertex(N->vertex[i].id, N->vertex[i].label);
                iscanG3->addVertex(N->vertex[i].id, N->vertex[i].label); 
                iscanG4->addVertex(N->vertex[i].id, N->vertex[i].label);  
            }

            int nEdges = 0;
            for(int i=0;i<N->nvertices;i++)
            {
                for(int j=0;j<N->vertex[i].degree;j++)
                {
                    nEdges++;
                
                    // Adding directed edges as Network contains a->b and b->a both
                    iscanG->addDirectedEdge(N->vertex[i].id, N->vertex[i].edge[j].target);
                    iscanG2->addDirectedEdge(N->vertex[i].id, N->vertex[i].edge[j].target);
                    iscanG3->addDirectedEdge(N->vertex[i].id, N->vertex[i].edge[j].target);
                    iscanG4->addDirectedEdge(N->vertex[i].id, N->vertex[i].edge[j].target);
                }
            }
            iscanG->numofEdges = nEdges/2;
            iscanG2->numofEdges = nEdges/2;
            iscanG3->numofEdges = nEdges/2;
            iscanG4->numofEdges = nEdges/2;
            nEdges/=2;

            iscan *iscanObject = new iscan(stof(argv[3]), stoi(argv[4]), iscanG);
            iscan *iscanObject2 = new iscan(stof(argv[3]), stoi(argv[4]), iscanG2,2);
            iscan *iscanObject3 = new iscan(stof(argv[3]), stoi(argv[4]), iscanG3,4);
            iscan *iscanObject4 = new iscan(stof(argv[3]), stoi(argv[4]), iscanG4,8);

            auto start = chrono::steady_clock::now();
            iscanObject->executeSCAN();
            auto end = chrono::steady_clock::now();

            auto start_2 = chrono::steady_clock::now();
            iscanObject2->executeSCAN(true);
            auto end_2 = chrono::steady_clock::now();

            auto start_3 = chrono::steady_clock::now();
            iscanObject3->executeSCAN(true);
            auto end_3 = chrono::steady_clock::now();

            auto start_4 = chrono::steady_clock::now();
            iscanObject4->executeSCAN(true);
            auto end_4 = chrono::steady_clock::now();


            
                    
                
                 
            cout<<"--------------------------------"<<endl;
            cout<<"Running time:"<<endl;
            
            cout<<"SCAN Time without parallel threads:"<<chrono::duration <double, milli> (end - start).count()<<endl;
            cout<<"SCAN Time with 2 parallel threads:"<<chrono::duration <double, milli> (end_2 - start_2).count()<<endl;
            cout<<"SCAN Time with 4 parallel threads:"<<chrono::duration <double, milli> (end_3 - start_3).count()<<endl;
            cout<<"SCAN Time with 8 parallel threads:"<<chrono::duration <double, milli> (end_4 - start_4).count()<<endl;

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

            graph* iscanG = new graph();        // Single thread
            graph* iscanG2 = new graph();       // Mutlithread 2
            graph* iscanG3 = new graph();       // Mutlithread 4
            graph* iscanG4 = new graph();       // Mutlithread 8

           // Loading initial graph
            while(getline(F, line)){nvertices++;}
            for(int i=0;i<nvertices;i++)
            {
                iscanG->addVertex(i, "");
                iscanG2->addVertex(i, "");
                iscanG3->addVertex(i, "");
                iscanG4->addVertex(i, "");
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
                        iscanG->addDirectedEdge(i, j);
                        iscanG2->addDirectedEdge(i, j);
                        iscanG3->addDirectedEdge(i, j);
                        iscanG4->addDirectedEdge(i, j);
                    }
                }
            }
            iscanG->numofEdges = nEdges/2;
            iscanG2->numofEdges = nEdges/2;
            iscanG3->numofEdges = nEdges/2;
            iscanG4->numofEdges = nEdges/2;
            nEdges/=2;

            iscan *iscanObject = new iscan(stof(argv[3]), stoi(argv[4]), iscanG);
            iscan *iscanObject2 = new iscan(stof(argv[3]), stoi(argv[4]), iscanG2,2);
            iscan *iscanObject3 = new iscan(stof(argv[3]), stoi(argv[4]), iscanG3,4);
            iscan *iscanObject4 = new iscan(stof(argv[3]), stoi(argv[4]), iscanG4,8);

            auto start = chrono::steady_clock::now();
            iscanObject->executeSCAN();
            auto end = chrono::steady_clock::now();

            auto start_2 = chrono::steady_clock::now();
            iscanObject2->executeSCAN(true);
            auto end_2 = chrono::steady_clock::now();

            auto start_3 = chrono::steady_clock::now();
            iscanObject3->executeSCAN(true);
            auto end_3 = chrono::steady_clock::now();

            auto start_4 = chrono::steady_clock::now();
            iscanObject4->executeSCAN(true);
            auto end_4 = chrono::steady_clock::now();
                
                 
            cout<<"--------------------------------"<<endl;
            cout<<"Running time:"<<endl;
            
            cout<<"SCAN Time without parallel threads:"<<chrono::duration <double, milli> (end - start).count()<<endl;
            cout<<"SCAN Time with 2 parallel threads:"<<chrono::duration <double, milli> (end_2 - start_2).count()<<endl;
            cout<<"SCAN Time with 4 parallel threads:"<<chrono::duration <double, milli> (end_3 - start_3).count()<<endl;
            cout<<"SCAN Time with 8 parallel threads:"<<chrono::duration <double, milli> (end_4 - start_4).count()<<endl;

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

            graph* iscanG = new graph();        // Single thread
            graph* iscanG2 = new graph();       // Mutlithread 2
            graph* iscanG3 = new graph();       // Mutlithread 4
            graph* iscanG4 = new graph();       // Mutlithread 8

            string line;
            int nedges = 0;
            while(getline(F, line)){nedges++;}
            F.clear();
            F.seekg(0);
            int id1, id2;
            for(int i=0;i<nedges;i++)
            {
                F>>id1>>id2;
                if(iscanG->vertexMap.find(id1)==iscanG->vertexMap.end())
                {
                    iscanG->addVertex(id1, "");
                    iscanG2->addVertex(id1, "");
                    iscanG3->addVertex(id1, "");
                    iscanG4->addVertex(id1, "");
                }
                if(iscanG->vertexMap.find(id2)==iscanG->vertexMap.end())
                {
                    iscanG->addVertex(id2, "");
                    iscanG2->addVertex(id2, "");
                    iscanG3->addVertex(id2, "");
                    iscanG4->addVertex(id2, "");
                }
                iscanG->addEdge(id1, id2);
                iscanG2->addEdge(id1, id2);
                iscanG3->addEdge(id1, id2);
                iscanG4->addEdge(id1, id2);
            }

            iscan *iscanObject = new iscan(stof(argv[3]), stoi(argv[4]), iscanG);
            iscan *iscanObject2 = new iscan(stof(argv[3]), stoi(argv[4]), iscanG2,2);
            iscan *iscanObject3 = new iscan(stof(argv[3]), stoi(argv[4]), iscanG3,4);
            iscan *iscanObject4 = new iscan(stof(argv[3]), stoi(argv[4]), iscanG4,8);

            auto start = chrono::steady_clock::now();
            iscanObject->executeSCAN();
            auto end = chrono::steady_clock::now();

            auto start_2 = chrono::steady_clock::now();
            iscanObject2->executeSCAN(true);
            auto end_2 = chrono::steady_clock::now();

            auto start_3 = chrono::steady_clock::now();
            iscanObject3->executeSCAN(true);
            auto end_3 = chrono::steady_clock::now();

            auto start_4 = chrono::steady_clock::now();
            iscanObject4->executeSCAN(true);
            auto end_4 = chrono::steady_clock::now();
                
                 
            cout<<"--------------------------------"<<endl;
            cout<<"Running time:"<<endl;
            
            cout<<"SCAN Time without parallel threads:"<<chrono::duration <double, milli> (end - start).count()<<endl;
            cout<<"SCAN Time with 2 parallel threads:"<<chrono::duration <double, milli> (end_2 - start_2).count()<<endl;
            cout<<"SCAN Time with 4 parallel threads:"<<chrono::duration <double, milli> (end_3 - start_3).count()<<endl;
            cout<<"SCAN Time with 8 parallel threads:"<<chrono::duration <double, milli> (end_4 - start_4).count()<<endl;
        }
    }

}



