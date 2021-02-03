#include<bits/stdc++.h>
#include<vertex.h>
using namespace std;

class graph
{

public:
    map<vertex*,vector<vertex*>> graphObject;
    int numOfNodes=0;
    int numofEdges=0;
    map<int,vector<vertex*>> clusters;
    vector<vertex*> hubs;
    vector<vertex*> outliers;
    map<pair<int,string>,vertex*> vertexMap;
    graph(/* args */);
    void addEdge(int Id1,string name1, int Id2, string name2);
    void addVertex(int Id, string name);
};

graph::graph(/* args */)
{
}

void graph::addEdge(int Id1, string name1, int Id2, string name2)
{
    vertex* v1 = vertexMap[{Id1,name1}];
    vertex* v2 = vertexMap[{Id2,name2}];

    graphObject[v1].push_back(v2);
    graphObject[v2].push_back(v1);
    numofEdges++;
}

void graph::addVertex(int Id, string name)
{
    vertex *newVertex = new vertex(Id,name);
    vertexMap[{Id,name}] = newVertex;
    vector<vertex*> newList;
    graphObject[newVertex] = newList;
    numOfNodes++;
}

