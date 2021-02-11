// SCAN Class
#include<bits/stdc++.h>
#include"graph.h"
#define CORE 0
#define NON_MEMBER 1
#define NON_CORE_MEMBER 2
#define HUB 0
#define OUTLIER 1 

using namespace std;

class scan
{
public:
    float epsilon;  // epsilon parameter
    int mu;  // mu parameter
    graph* inputGraph; // graph to be analysed
    scan(float, int, graph*);
    float calculateSimilarity(vertex*, vertex*);
    vector<vertex*> getEpsilonNeighbourhood(vertex*);
    bool isCore(vertex*);
    void execute();
    void intermediateOutput(vector<vertex*>, ofstream &, int);
    void printEpsilonNeighbours(ofstream &);
    
};

// constructor
scan::scan(float ep,int mu, graph* inputGraph)
{
    this->epsilon = ep;
    this->mu = mu;
    this->inputGraph = inputGraph;
}

// calculates similarity between two vertices
float scan::calculateSimilarity(vertex* v1, vertex* v2)
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
vector<vertex*> scan::getEpsilonNeighbourhood(vertex* v)
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
bool scan::isCore(vertex* v)
{
    if(getEpsilonNeighbourhood(v).size()>=mu)
        return true;
    else return false;
}


// creates clustering and classifies non member vertices as hubs or outliers
void scan::execute()
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
                        continue;
                    }

                    // If node already a non member, dont push in queue as it is already a non core vertex
                    if(neighbour->isClassified == 0){
                        if(isCore(neighbour) == true){
                            neighbour->memberType = CORE;
                            q.push(neighbour);
                            outputFile << "(CORE MEMBER " <<  neighbour->ID <<  ") ";
                        }
                        else{
                            neighbour->memberType = NON_CORE_MEMBER;
                            outputFile << "(NON CORE MEMBER " <<  neighbour->ID <<  ") ";
                        }
                    }
                    else{
                        neighbour->memberType = NON_CORE_MEMBER;
                        outputFile << "(NON CORE MEMBER " <<  neighbour->ID <<  ") ";                        
                    }

                    flag = 0;

                    neighbour->clusterId = cluster_id;
                    neighbour->isClassified = 1;
                    cluster.push_back(neighbour);

                }
                if (flag){
                    outputFile << "All nodes in the epsilon neighbourhood have already been classified";
                }
                outputFile<<endl;
            }

            // Printing a cluster
            intermediateOutput(cluster, outputFile, cluster_id);
            inputGraph->clusters[cluster_id] = cluster;
            cluster_id++;


                    

        }
        else{  // label the vertex as a non_member
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

            }
            else{
                sequence[start]->hub_or_outlier = OUTLIER;
                inputGraph->outliers.push_back(sequence[start]);
            } 

        }
    }

}

void scan::intermediateOutput(vector<vertex*> cluster, ofstream &outputFile, int cluster_id)
{
    outputFile<<"Finalised a Cluster with ID "<<cluster_id<<" : ";
    for(int i=0;i<cluster.size();i++)
    {
        outputFile<<cluster[i]->ID<<" ";
    }
    outputFile<<endl<<"--------------------------------------------------"<<endl;
    outputFile<<endl;
}

void scan::printEpsilonNeighbours(ofstream &outputFile)
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