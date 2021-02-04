// Graph Class
#include<bits/stdc++.h>
#include"vertex.h"
using namespace std;

class graph
{

public:
    map<vertex*,vector<vertex*>> graphObject;  // stores the graph as a mapping from vertex pointer to vector of pointers to vertices
    int numOfNodes=0;   // number of vertices
    int numofEdges=0;   // number of edges
    map<int,vector<vertex*>> clusters;  // stores the clusters as a mapping from clusterID to vector of pointers to vertices
    vector<vertex*> hubs;   // stores the hubs
    vector<vertex*> outliers; // stores the outliers
    map<pair<int,string>,vertex*> vertexMap; // mapping from (vertex id, vertex name) to vertex pointer
    graph(/* args */);
    void addEdge(int Id1,string name1, int Id2, string name2);  // add edge to graph
    void addVertex(int Id, string name);  // add vertex to graph
    void printClusters();  // prints clusters
    void printGraph(); // prints graphs
};

// contructor
graph::graph(/* args */)
{
    graphObject = map<vertex*, vector<vertex*>>();
}


// add edge to graph
void graph::addEdge(int Id1, string name1, int Id2, string name2)
{
    vertex* v1 = vertexMap[{Id1,name1}];
    vertex* v2 = vertexMap[{Id2,name2}];
    graphObject[v1].push_back(v2);
    graphObject[v2].push_back(v1);
    numofEdges++;
}


// add vertex to graph
void graph::addVertex(int Id, string name)
{
    vertex *newVertex = new vertex(Id,name);
    vertexMap[{Id,name}] = newVertex;
    vector<vertex*> newList;
    graphObject[newVertex] = newList;
    numOfNodes++;
}


// prints clusters, hubs, outliers after scan has been completed
void graph::printClusters()
{
    cout<<"CLUSTERS"<<endl;
    for(auto it=clusters.begin(); it!=clusters.end();it++)
    {
        cout<<it->first<<": ";
        vector<vertex*> temp = clusters[it->first];
        for(int i=0;i<temp.size();i++)
        {
            cout<<temp[i]->ID<<" ";
        }
        cout<<endl;
    }

    cout<<"HUBS: ";
    for(int i=0;i<hubs.size();i++)
    {
        cout<<hubs[i]->ID<<" ";
    }
    cout<<endl;

    cout<<"OUTLIERS: ";
    for(int i=0;i<outliers.size();i++)
    {
        cout<<outliers[i]->ID<<" ";
    }
    cout<<endl;
}

// prints graphs
void graph::printGraph()
{
    cout<<"GRAPH: ";
    for(auto it=graphObject.begin(); it!=graphObject.end();it++)
    {
        cout<<(it->first)->ID<<": ";
        vector<vertex*> temp = graphObject[it->first];
        for(int i=0;i<temp.size();i++)
        {
            cout<<temp[i]->ID<<" ";
        }
        cout<<endl;
    }   
}