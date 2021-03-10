#include <bits/stdc++.h>
#include "vertex.h"
using namespace std;

class bfsTree 
{
    public:
        
        unordered_set<pair<vertex*, vertex*>> phi;

        unordered_set<pair<vertex*, vertex*>> bfsSet;

        void addEdgeToPhi(vertex* v1, vertex* v2);

        bool removeEdgeFromPhi(vertex* v1, vertex* v2);

        int findInPhi(vertex* v1, vertex* v2);

        void addEdgeToBfsSet(vertex* v1, vertex* v2);

        bool removeEdgeFromBfsSet(vertex* v1, vertex* v2);

        int findInBfsSet(vertex* v1, vertex* v2);

        void merge(vertex* v1, vertex* v2);

        void recurseChildren(vertex* v, int clusterId);

        void recurseParent(vertex* u, vertex* parent, int clusterId);
};



void bfsTree::addEdgeToPhi(vertex* v1, vertex* v2)
{
    phi.insert({v1, v2});
}

bool bfsTree::removeEdgeFromPhi(vertex* v1, vertex* v2)
{
    int temp = findInPhi(v1, v2);

    if(temp == 0)
    {
        phi.erase({v1, v2});
        return true;
    }
    else if(temp == 1)
    {
        phi.erase({v2, v1});
        return true;
    }
    else return false;
}

int bfsTree::findInPhi(vertex* v1, vertex* v2)
{
    if(phi.find({v1, v2})!=phi.end())
    {
        return 0;
    }
    if(phi.find({v2, v1})!=phi.end())
    {
        return 1;
    }
    return 2;
}


void bfsTree::addEdgeToBfsSet(vertex* v1, vertex* v2)
{
    bfsSet.insert({v1, v2});
    v1->children.insert(v2);
    v2->parent = v1;
}

bool bfsTree::removeEdgeFromBfsSet(vertex* v1, vertex* v2)
{
    int temp = findInPhi(v1, v2);

    if(temp == 0)
    {
        bfsSet.erase({v1, v2});
        v1->children.erase(v2);
        v2->parent = NULL;
        return true;
    }
    else if(temp == 1)
    {
        bfsSet.erase({v2, v1});
        v2->children.erase(v1);
        v1->parent = NULL;
        return true;
    }
    else return false;
}

int bfsTree::findInBfsSet(vertex* v1, vertex* v2)
{
    if(bfsSet.find({v1, v2})!=bfsSet.end())
    {
        return 0;
    }
    if(bfsSet.find({v2, v1})!=bfsSet.end())
    {
        return 1;
    }
    return 2;
}

// Assumed that v1 has smaller size of cluster
// Do accordingly in mergeCluster
void bfsTree::merge(vertex* v1, vertex* v2)
{
    removeEdgeFromPhi(v1, v2);
    addEdgeToBfsSet(v2, v1);

    int clusterId = v2->clusterId;
    recurseParent(v1, v1->parent, clusterId);
    recurseChildren(v1, clusterId);

}

void bfsTree::recurseChildren(vertex* v, int clusterId)
{
    queue<vertex* > q;
    for(auto it=v->children.begin(); it!=v->children.end();it++)
    {
        q.push(*it);
    }
    while(!q.empty())
    {
        vertex* temp = q.front();
        temp->clusterId = clusterId;
        for(auto it=temp->children.begin(); it!=temp->children.end();it++)
        {
            q.push(*it);
        }
    }
}

void bfsTree::recurseParent(vertex* u, vertex* parent, int clusterId)
{
    if(parent == NULL)
    {
        return;
    }

    parent->clusterId = clusterId;
    for(auto it = parent->children.begin(); it!=parent->children.end();it++)
    {
        (*it)->clusterId = clusterId;

        if(*it != u){
            recurseChildren(*it, clusterId);
        }

    }
    recurseParent(parent, parent->parent, clusterId);
}