#include<bits/stdc++.h>
#include"Iscan/iscan.h"
#include"readgml/readgml.h"

using namespace std;

int main(int argc, char* argv[])
{
    if(strcmp(argv[1], "--LINK") == 0)
    {
        ifstream F(argv[2]);
        if (!F) perror ("Error opening file");
        else
        {
            if(stof(argv[3])>1 || stof(argv[3])<=0){cout<<"Epsilon value should be between 0 and 1"<<endl;exit(0);}
            if(stoi(argv[4])<=0){cout<<"Mu value should be greater than 0"<<endl;exit(0);}

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

            int delEdgeNumber = 10;

            scanG->numofEdges = nedges;
            iscanG->numofEdges = nedges;
            iscanG2->numofEdges = nedges;

            F.clear();
            F.seekg(0);
            int i, j;
            for(int k=0;k<nedges;k++)
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

            F.clear();
            F.seekg(0);
            for(int k=0;k<delEdgeNumber;k++)
            {
                F>>i>>j;

                cout<<"Deleting edge between "<<i<< " & "<<j<<endl;
        
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
                currentG->numofEdges = scanG->numofEdges;
                
                iscan *tempScanObject = new iscan(stof(argv[3]), stoi(argv[4]), currentG);
                auto start = chrono::steady_clock::now();
                tempScanObject->executeSCAN();
                auto end = chrono::steady_clock::now();
                auto diff = end - start;
                scanTime += chrono::duration <double, milli> (diff).count();
                
                /*MultiThread*/
                start = chrono::steady_clock::now();
                iscanObject->updateEdge(i, j, 0);
                end = chrono::steady_clock::now();
                diff = end - start;
                incrementalTime += chrono::duration <double, milli> (diff).count();
                
                /*Single Thread*/
                start = chrono::steady_clock::now();
                iscanObject2->updateEdge(i, j, 0, true);
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
