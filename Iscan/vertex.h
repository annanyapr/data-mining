// vertex class

#ifndef _VERTEX_GUARD
#define _VERTEX_GUARD

#include<bits/stdc++.h>
using namespace std;

class vertex
{
    public:
        int ID;  // id
        string name; // name
        bool isClassified=0;  // tells whether a vertex is classified or not
        int memberType=-1;   // 0 is core, 1 is Non Member, 2 is Non Core Member
        int clusterId = -1;  // cluster id
        int hub_or_outlier = -1; // hub is 0, outlier is 1
        vertex * parent = NULL; // Parent of this vertex in BFS tree
        unordered_set<vertex*> children;    // Children of this vertex in BFS tree
        vertex(int id, string nm);
};

// constructor
vertex::vertex(int id, string nm)
{
    this->ID = id;
    this->name = nm;
}

// A hash function used to hash a pair of any kind 
struct hash_pair { 
    template <class T1, class T2> 
    size_t operator()(const pair<T1, T2>& p) const
    { 
        auto hash1 = hash<T1>{}(p.first); 
        auto hash2 = hash<T2>{}(p.second); 
        return hash1 ^ hash2; 
    } 
};
#endif
