#include <bits/stdc++.h>
#include "vertex.h"
using namespace std;

class bfsTree 
{
    public:
        
        // Stores edges not part of bfs forest ; for edge a<->b only one of {a,b} or {b,a} is present
        unordered_set<pair<vertex*, vertex*>,hash_pair> phi;

        // Stores edges part of bfs forest ; for edge a<->b only one of {a,b} or {b,a} is present
        unordered_set<pair<vertex*, vertex*>,hash_pair> bfsSet;

        void addEdgeToPhi(vertex* v1, vertex* v2);

        bool removeEdgeFromPhi(vertex* v1, vertex* v2);

        int findInPhi(vertex* v1, vertex* v2);

        void addEdgeToBfsSet(vertex* v1, vertex* v2);

        bool removeEdgeFromBfsSet(vertex* v1, vertex* v2);

        int findInBfsSet(vertex* v1, vertex* v2);

        // Merges two clusters of which v1 and v2 are part of 
        void merge(vertex* v1, vertex* v2);

        // Recurse through the children of v
        void recurseChildren(vertex* v, int clusterId);

        // Recurse through the parent of u and all its children
        void recurseParent(vertex* u, vertex* parent, int clusterId);

        void printBfsSet();
        
        void printPhiSet();

        // Switch the parents from v to all the nodes till we reach the root
        void switchParents(vertex* v);

        bfsTree();
};


// Add edge only if {v1,v2} and {v2,v1} not alredy present in phi
void bfsTree::addEdgeToPhi(vertex* v1, vertex* v2)
{
    int temp = findInPhi(v1, v2);
    if(temp == 2){
        phi.insert({v1, v2});
    }

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

// Check for {v1,v2} and {v2,v1}
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

// Add edge only if {v1,v2} and {v2,v1} not alredy present in bfs
void bfsTree::addEdgeToBfsSet(vertex* v1, vertex* v2)
{

    int temp = findInBfsSet(v1, v2);
    if(temp == 2){
        bfsSet.insert({v1, v2});
        v1->children.insert(v2);
        v2->parent = v1;
    }

}

bool bfsTree::removeEdgeFromBfsSet(vertex* v1, vertex* v2)
{
    int temp = findInBfsSet(v1, v2);

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

// Merges two clusters where cluster having v1 has smaller size
void bfsTree::merge(vertex* v1, vertex* v2)
{
    removeEdgeFromPhi(v1, v2);
    switchParents(v1);
    addEdgeToBfsSet(v2, v1);

    int clusterId = v2->clusterId;
    recurseChildren(v1, clusterId);

}

void bfsTree::switchParents(vertex* v){
    
    if(v->parent == NULL)
    {
        return;
    }
    switchParents(v->parent);
    bfsSet.erase({v->parent,v});
    bfsSet.insert({v,v->parent});
    v->parent->children.erase(v);
    v->parent->parent= v;    
    v->children.insert(v->parent);
    
}

// Change clusterid of current vertex v then run bfs on all its children
void bfsTree::recurseChildren(vertex* v, int clusterId)
{
    v->clusterId = clusterId;
    queue<vertex* > q;
    
    for(auto it=v->children.begin(); it!=v->children.end();it++)
    {
        q.push(*it);
    }

    while(!q.empty())
    {
        vertex* temp = q.front();
        q.pop();
        temp->clusterId = clusterId;
        for(auto it=temp->children.begin(); it!=temp->children.end();it++)
        {
            q.push(*it);
        }
    }
}

// Change the clusterid of parent and recurse through all the children of parent except current vertex
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
        // Recurse only for other children not u
        if(*it != u){
            recurseChildren(*it, clusterId);
        }

    }
    recurseParent(parent, parent->parent, clusterId);
}

// Constructor
bfsTree::bfsTree()
{
    this->phi.clear();
}

// Util function to print the bfs set
void bfsTree::printBfsSet(){
    cout<<"BFS Set\n";
    for( auto it:bfsSet)
    {
        cout<<(it.first)->ID<<" "<<(it.second)->ID<<endl;
    }
    cout<<endl;
}

// Util function to print the phi set
void bfsTree::printPhiSet(){
    cout<<"Phi Set\n";
    for( auto it:phi)
    {
        cout<<(it.first)->ID<<" "<<(it.second)->ID<<endl;
    }
    cout<<endl;
}