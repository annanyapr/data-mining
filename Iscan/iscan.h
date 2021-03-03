// ISCAN Class
#include <bits/stdc++.h>
#include "graph.h"
#include "bfsTree.h"


#define CORE 0
#define NON_MEMBER 1
#define NON_CORE_MEMBER 2
#define HUB 0
#define OUTLIER 1 

using namespace std;

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

class iscan
{
public:
    // epsilon parameter 
    float epsilon;  

    // mu parameter
    int mu;  

    // graph to be analysed
    graph* inputGraph; 

    bfsTree* bfsTreeObject;

    // Constructor
    iscan(float, int, graph*);

    // Calculates similarity between two vertices
    float calculateSimilarity(vertex*, vertex*);

    // Returns epsilon neighbourhood of a neighbourhood
    vector<vertex*> getEpsilonNeighbourhood(vertex*);

    // Checks if a vertex is core
    bool isCore(vertex*);

    // Main clustering algorithm
    void executeSCAN();

    // void updateEdge()

    // Output formed cluster to intermediate file
    void printClusterToFile(vector<vertex*>, ofstream &, int);

    // Output epsilon neighbourhood of all vertices in intermediate file
    void printEpsilonNeighbours(ofstream &);

    unordered_set<vertex*> getNuv(int Id1, int Id2);

    unordered_set<pair<vertex*,vertex*>,hash_pair> getRuv(int Id1, int Id2, unordered_set<vertex*> Nuv);

    void updateEdge(int id1, int id2, bool isAdded);

    void mergeCluster(vertex* v);

    void splitCluster(vertex* v1, vertex* v2);
    
};

// constructor
iscan::iscan(float ep,int mu, graph* inputGraph)
{
    this->epsilon = ep;
    this->mu = mu;
    this->inputGraph = inputGraph;
}

// calculates similarity between two vertices
float iscan::calculateSimilarity(vertex* v1, vertex* v2)
{
    vector<vertex*>neighbour1 = inputGraph->graphObject[v1];
    vector<vertex*>neighbour2 = inputGraph->graphObject[v2];

    unordered_set<vertex*> s1(neighbour1.begin(), neighbour1.end());
    s1.insert(v1);

    neighbour2.push_back(v2);

    int count=0;
    for(auto it=neighbour2.begin();it!=neighbour2.end();it++)
    {
        if(s1.find(*it)!=s1.end())
        {
            count++;
        }
    }
    return ((float)count)/(sqrt(s1.size()*neighbour2.size()));
}

// calculates epsilon neighbourhood of a vertex
vector<vertex*> iscan::getEpsilonNeighbourhood(vertex* v)
{
    vector<vertex*>ret;
    ret.push_back(v);
    vector<vertex*>neighbour = inputGraph->graphObject[v];

    for (int i = 0; i < neighbour.size(); i++)
    {
        if(calculateSimilarity(v,neighbour[i])>=epsilon)
        {
            ret.push_back(neighbour[i]);
        }
    }
    return ret;
}

// tells whether a vertex is core or not
bool iscan::isCore(vertex* v)
{
    if(getEpsilonNeighbourhood(v).size()>=mu)
        return true;
    else return false;
}


// creates clustering and classifies non member vertices as hubs or outliers
void iscan::executeSCAN()
{
    ofstream outputFile;
    outputFile.open("intermediate.txt", std::ofstream::out | std::ofstream::trunc);
    printEpsilonNeighbours(outputFile);


    int cluster_id  = 0;
    vector<vertex*> sequence;
    for(auto iter = inputGraph->graphObject.begin(); iter != inputGraph->graphObject.end(); iter++){
        sequence.push_back(iter->first);
    } 

    // starts iteration on all vertices
    for(int start = 0; start < sequence.size(); start++){
        // if vertex already visited, continue
        if(sequence[start]->isClassified == 1){
            continue;
        }

        // if vertex is core, use BFS to generate cluster
        if(isCore(sequence[start]) == 1){
            vector<vertex*> cluster;
            cluster.push_back(sequence[start]);
            sequence[start]->memberType = CORE;
            sequence[start]->isClassified = 1;
            sequence[start]->clusterId = cluster_id;

            
            queue<vertex*> q;
            q.push(sequence[start]);

            while (q.size() > 0){
                vertex* temp_node = q.front();
                q.pop();
                outputFile << "CORE " << temp_node->ID << " generates: ";
                vector<vertex*> R = getEpsilonNeighbourhood(temp_node);  // generate epsilon neighbourhood to push in the queue

                int flag = 1;

                for(int index = 0; index < R.size(); index++){

                    vertex* neighbour = R[index];
                    // If node already a member of cluster, continue
                    if ((neighbour->isClassified == 1) && (neighbour->memberType != NON_MEMBER)){
                        bfsTreeObject->addEdgeToPhi(neighbour, temp_node);
                        continue;
                    }

                    // If node not already classified then classify it as core or non-core member
                    if(neighbour->isClassified == 0){
                        // Core
                        if(isCore(neighbour) == true){
                            neighbour->memberType = CORE;
                            q.push(neighbour);
                            outputFile << "(CORE MEMBER " <<  neighbour->ID <<  ") ";
                        }
                        // Non-core member
                        else{
                            neighbour->memberType = NON_CORE_MEMBER;
                            outputFile << "(NON CORE MEMBER " <<  neighbour->ID <<  ") ";
                        }
                    }
                    // If already classified then Non-core member
                    else{

                        neighbour->memberType = NON_CORE_MEMBER;
                        outputFile << "(NON CORE MEMBER " <<  neighbour->ID <<  ") ";  

                                              
                    }

                    flag = 0;
                    bfsTreeObject->addEdgeToBfsSet(temp_node, neighbour);
                    neighbour->clusterId = cluster_id;
                    neighbour->isClassified = 1;
                    cluster.push_back(neighbour);

                }
                if (flag){
                    outputFile << "All nodes in the epsilon neighbourhood have already been classified";
                }
                outputFile<<endl;
            }

            // Printing recently formed cluster to file
            printClusterToFile(cluster, outputFile, cluster_id);
            inputGraph->clusters[cluster_id] = cluster;
            cluster_id++;  

        }
        else{  
            // label the vertex as a non_member
            sequence[start]->isClassified = 1;
            sequence[start]->memberType = NON_MEMBER;
        }

    }

    // for each non_member vertex check whether its a hub or an outlier
    for(int start = 0; start < sequence.size(); start++){
        if(sequence[start]->memberType == NON_MEMBER){ 
            vector<vertex*> neighbours = inputGraph->graphObject[sequence[start]];
            unordered_set<int> cluster_ids;
            
            for(int i = 0; i < neighbours.size(); i++){
                if (neighbours[i]->memberType != NON_MEMBER)
                    cluster_ids.insert(neighbours[i]->clusterId);
            }

            if (cluster_ids.size() >=  2){
                sequence[start]->hub_or_outlier = HUB;
                inputGraph->hubs.push_back(sequence[start]);
                outputFile<<"HUB: "<<sequence[start]->ID<<" is connected to clusters ";
                for(auto it=cluster_ids.begin(); it!=cluster_ids.end();it++)
                {
                    outputFile<<*it<<" ";
                }
                outputFile<<endl;
            }
            else{
                sequence[start]->hub_or_outlier = OUTLIER;
                inputGraph->outliers.push_back(sequence[start]);
            } 

        }
    }

}

// Output formed cluster to intermediate file
void iscan::printClusterToFile(vector<vertex*> cluster, ofstream &outputFile, int cluster_id)
{
    outputFile<<"Finalised a Cluster with ID "<<cluster_id<<" : ";
    for(int i=0;i<cluster.size();i++)
    {
        outputFile<<cluster[i]->ID<<" ";
    }
    outputFile<<endl<<"--------------------------------------------------"<<endl;
    outputFile<<endl;
}

// Output epsilon neighbourhood of all vertices in intermediate file
void iscan::printEpsilonNeighbours(ofstream &outputFile)
{
    outputFile<<"-------------------------Epsilon neighbourboods-------------------------"<<endl;
    outputFile<<"VERTEX ID: EPSILON NEIGHBOURS"<<endl;
    unordered_map<vertex*,vector<vertex*>> go = inputGraph->graphObject;
    vector<vertex*> vertices;
    for(auto it=go.begin(); it!=go.end();it++)
    {
        vertices.push_back(it->first);
    }   

    sort(vertices.begin(), vertices.end(), comp);
    for(int i=0;i<vertices.size();i++)
    {
        outputFile<<vertices[i]->ID<<": ";
        vector<vertex*> temp = getEpsilonNeighbourhood(vertices[i]);
        for(int i=0;i<temp.size();i++)
        {
            outputFile<<temp[i]->ID<<" ";
        }
        outputFile<<endl;
    }

    outputFile<<endl<<"-------------------------SCAN EXECUTION starts-------------------------"<<endl;

}

unordered_set<vertex*> iscan::getNuv(int Id1, int Id2){

    vertex* v1 = inputGraph->vertexMap[Id1];
    vertex* v2 = inputGraph->vertexMap[Id2];

    vector<vertex*> neighbour1 = inputGraph->graphObject[v1];
    vector<vertex*> neighbour2 = inputGraph->graphObject[v2];
    
    unordered_set<vertex*> s1(neighbour1.begin(), neighbour1.end()); 
    s1.insert(v1);

    neighbour2.push_back(v2);
    for(auto it=neighbour2.begin();it!=neighbour2.end();it++)
    {
        s1.insert(*it);
    }

    return s1;
}

// TODO chage to unordered set
unordered_set<pair<vertex*,vertex*>,hash_pair> iscan::getRuv(int Id1, int Id2, unordered_set<vertex*> Nuv){

    vertex* v1 = inputGraph->vertexMap[Id1];
    vertex* v2 = inputGraph->vertexMap[Id2];

    vector<vertex*> neighbour1 = inputGraph->graphObject[v1];
    vector<vertex*> neighbour2 = inputGraph->graphObject[v2];

    unordered_set<pair<vertex*,vertex*>,hash_pair> ret;

    for(auto it=neighbour1.begin();it!=neighbour1.end();it++)
    {
        if(Nuv.find(*it)!=Nuv.end())
        {
            ret.insert({v1,*it});
        }
    }

    for(auto it=neighbour2.begin();it!=neighbour2.end();it++)
    {
        if(Nuv.find(*it)!=Nuv.end())
        {
            ret.insert({v2,*it});
        }
    }
    return ret;
}

void iscan::updateEdge(int id1, int id2, bool isAdded){
    

    unordered_set<vertex*> Nuv = getNuv(id1,id2);

    unordered_set<pair<vertex*,vertex*>,hash_pair> Ruv = getRuv(id1, id2, Nuv);

    map<pair<vertex*,vertex*>,int> sigmaOld;

    for(auto it: Ruv)
    {
        sigmaOld[it] = calculateSimilarity(it.first,it.second);
    }
    
    if(isAdded)
    {
        inputGraph->addEdge(id1,id2);
    }
    else
    {
        inputGraph->removeEdge(id1,id2);
    }

    for(auto it:Nuv)
    {
        if(isCore(it))
        {
            mergeCluster(it);
        }
    }

    for(auto it:Ruv)
    {
        if(sigmaOld[it]>= epsilon && calculateSimilarity(it.first,it.second)<epsilon)
            splitCluster(it.first,it.second);
    }    

    for(auto it:bfsTreeObject->phi)
    {
        if(it.first->clusterId != it.second->clusterId)
        {
            if(isCore(it.first) && isCore(it.second))
                bfsTreeObject->merge(it.first,it.second);
            
            if(isCore(it.first) && it.second->memberType ==1)
            {
                it.second->clusterId = it.first->clusterId;
                bfsTreeObject->addEdgeToBfsSet(it.first,it.second);
                bfsTreeObject->removeEdgeFromPhi(it.first,it.second);
            }

            if(isCore(it.second) && it.first->memberType ==1)
            {
                it.first->clusterId = it.second->clusterId;
                bfsTreeObject->addEdgeToBfsSet(it.second,it.first);
                bfsTreeObject->removeEdgeFromPhi(it.second,it.first);
            }
        }
    }

    // for each non_member vertex check whether its a hub or an outlier
    for(auto it : inputGraph->vertexMap){
        if(it.second->memberType == NON_MEMBER){ 
            vector<vertex*> neighbours = inputGraph->graphObject[it.second];
            unordered_set<int> cluster_ids;
            
            for(int i = 0; i < neighbours.size(); i++){
                if (neighbours[i]->memberType != NON_MEMBER)
                    cluster_ids.insert(neighbours[i]->clusterId);
            }

            if (cluster_ids.size() >=  2){
                it.second->hub_or_outlier = HUB;
                inputGraph->hubs.push_back(it.second);
                // outputFile<<"HUB: "<<it.second->ID<<" is connected to clusters ";
                // for(auto it=cluster_ids.begin(); it!=cluster_ids.end();it++)
                // {
                //     outputFile<<*it<<" ";
                // }
                // outputFile<<endl;
            }
            else{
                it.second->hub_or_outlier = OUTLIER;
                inputGraph->outliers.push_back(it.second);
            } 

        }
    }
}

void iscan::mergeCluster(vertex* v){

}

void iscan::splitCluster(vertex* v1, vertex* v2){

}

/*
TODO:

1. Reasssigning clusterid after split cluster
2. Storing all similariy values
3. set --> Unorderd_set in BFSTree
*/