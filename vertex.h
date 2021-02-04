#include<bits/stdc++.h>
using namespace std;

class vertex
{
    public:
        int ID;
        string name;
        bool isClassified=0;
        int memberType=-1;   // 0 is core, 1 is Non Member, 2 is Non Core Member
        int clusterId = -1;
        int hub_or_outlier = -1; // hub is 0, outlier is 1
        vertex(int id, string nm);
};

vertex::vertex(int id, string nm)
{
    this->ID = id;
    this->name = nm;
}


