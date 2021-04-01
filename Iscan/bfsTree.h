#include <bits/stdc++.h>
#include "vertex.h"
using namespace std;

class bfsTree 
{
    public:
        
        unordered_set<pair<vertex*, vertex*>,hash_pair> phi;

        unordered_set<pair<vertex*, vertex*>,hash_pair> bfsSet;

        void addEdgeToPhi(vertex* v1, vertex* v2);

        bool removeEdgeFromPhi(vertex* v1, vertex* v2);

        int findInPhi(vertex* v1, vertex* v2);

        void addEdgeToBfsSet(vertex* v1, vertex* v2);

        bool removeEdgeFromBfsSet(vertex* v1, vertex* v2);

        int findInBfsSet(vertex* v1, vertex* v2);

        void merge(vertex* v1, vertex* v2);

        void recurseChildren(vertex* v, int clusterId);

        void recurseParent(vertex* u, vertex* parent, int clusterId);

        void printBfsSet();
        
        void printPhiSet();

        void switchParents(vertex* v);

        bfsTree();
};



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

// v1 has smaller size of cluster
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

bfsTree::bfsTree()
{
    this->phi.clear();
}

void bfsTree::printBfsSet(){
    cout<<"BFS Set\n";
    for( auto it:bfsSet)
    {
        cout<<(it.first)->ID<<" "<<(it.second)->ID<<endl;
    }
    cout<<endl;
}

void bfsTree::printPhiSet(){
    cout<<"Phi Set\n";
    for( auto it:phi)
    {
        cout<<(it.first)->ID<<" "<<(it.second)->ID<<endl;
    }
    cout<<endl;
}