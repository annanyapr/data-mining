// Graph Class
#include<bits/stdc++.h>
#include"vertex.h"
using namespace std;

class graph
{

public:
    // stores the graph as a mapping from vertex pointer to vector of pointers to vertices
    unordered_map<vertex*,vector<vertex*>> graphObject;  

    // number of vertices
    int numOfNodes=0; 

    // number of edges
    int numofEdges=0;   

    // stores the clusters as a mapping from clusterID to vector of pointers to vertices
    map<int,vector<vertex*>> clusters;  

    // stores the hubs
    vector<vertex*> hubs;   

    // stores the outliers
    vector<vertex*> outliers; 

    // mapping from (vertex id) to vertex pointer
    unordered_map<int,vertex*> vertexMap; 

    // Constructor
    graph();

    // add edge to graph
    void addEdge(int Id1, int Id2);  

    // add directed edge to graph
    void addDirectedEdge(int Id1, int Id2);

    // add vertex to graph 
    void addVertex(int Id, string name);

    // prints clusters  
    void printClusters();

    // prints graphs  
    void printGraph(); 
};

// contructor
graph::graph()
{
    graphObject = unordered_map<vertex*, vector<vertex*>>();
}

// add undirected edge to graph 
// v1 to v2 and v2 to v1
void graph::addEdge(int Id1, int Id2)
{
    vertex* v1 = vertexMap[Id1];
    vertex* v2 = vertexMap[Id2];
    graphObject[v1].push_back(v2);
    graphObject[v2].push_back(v1);
}

// Add directed edge from v1 to v2
void graph::addDirectedEdge(int Id1,  int Id2)
{
    vertex* v1 = vertexMap[Id1];
    vertex* v2 = vertexMap[Id2];
    graphObject[v1].push_back(v2);
}

// add vertex to graph
void graph::addVertex(int Id, string name)
{
    vertex *newVertex = new vertex(Id,name);
    vertexMap[Id] = newVertex;
    vector<vertex*> newList;
    graphObject[newVertex] = newList;
    numOfNodes++;
}


// Comparator to sort vertices in increasing order of their IDs
bool comp(const vertex *v1,const vertex *v2)
{
    return v1->ID < v2->ID;
}

// prints clusters, hubs, outliers after scan has been completed its execution
void graph::printClusters()
{
    // Print clusters
    cout<<"CLUSTERS"<<endl;
    for(auto it=clusters.begin(); it!=clusters.end();it++)
    {
        cout<<it->first<<": ";
        vector<vertex*> temp = clusters[it->first];
        sort(temp.begin(), temp.end(), comp);
        for(int i=0;i<temp.size();i++)
        {
            cout<<temp[i]->ID<<" ";
        }
        cout<<endl;
    }

    // Print hubs
    vector<vertex*> temp = hubs;
    sort(temp.begin(), temp.end(), comp);
    cout<<"HUBS: ";
    for(int i=0;i<temp.size();i++)
    {
        cout<<temp[i]->ID<<" ";
    }
    cout<<endl;

    // Print outliers
    temp = outliers;
    sort(temp.begin(), temp.end(), comp);
    cout<<"OUTLIERS: ";
    for(int i=0;i<temp.size();i++)
    {
        cout<<temp[i]->ID<<" ";
    }
    cout<<endl;
}

// prints graph
void graph::printGraph()
{
    cout<<"GRAPH: ";
    cout<<"Number of vertices: "<< numOfNodes<<endl;
    cout<<"Number of edges: "<< numofEdges<<endl;
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