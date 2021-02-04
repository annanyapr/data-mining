#include<bits/stdc++.h>
#include<graph.h>
#define CORE 0
#define NON_MEMBER 1
#define NON_CORE_MEMBER 2
#define HUB 0
#define OUTLIER 1

using namespace std;

class scan
{
public:
    float epsilon;
    int mu;
    graph* inputGraph;
    scan(/* args */);
    float calculateSimilarity(vertex*, vertex*);
    vector<vertex*> getEpsilonNeighbourhood(vertex*);
    bool isCore(vertex*);
    void execute();
    
};

scan::scan(/* args */)
{
}

float scan::calculateSimilarity(vertex* v1, vertex* v2)
{
    vector<vertex*>neighbour1 = inputGraph->graphObject[v1];
    vector<vertex*>neighbour2 = inputGraph->graphObject[v2];

    set<vertex*> s1(neighbour1.begin(), neighbour1.end());
    s1.insert(v1);

    set<vertex*> s2(neighbour2.begin(), neighbour2.end());
    s2.insert(v2); 

    set<int> intersect;
    set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), inserter(intersect, intersect.begin()));

    return (intersect.size()/(sqrt(s1.size()*s2.size())));
}

vector<vertex*> scan::getEpsilonNeighbourhood(vertex* v)
{
    vector<vertex*>ret;
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

bool scan::isCore(vertex* v)
{
    if(getEpsilonNeighbourhood(v).size()>=mu)
        return true;
}

void scan::execute()
{
    int cluster_id  = 0;
    vector<vertex*> sequence;
    for(auto iter = inputGraph->graphObject.begin(); iter != inputGraph->graphObject.end(); iter++){
        sequence.push_back(iter->first);
    } 

    vector<vector<vertex*>>clusters;

    for(int start = 0; start < sequence.size(); start++){
        if(sequence[start]->isClassified == 1){
            continue;
        }

        if(isCore(sequence[start]) == 1){
            vector<vertex*> cluster;
            cluster.push_back(sequence[start]);
            sequence[start]->memberType = CORE;
            sequence[start]->isClassified = 1;
            sequence[start]->clusterId = cluster_id;
            vector<vertex*>temp = getEpsilonNeighbourhood(sequence[start]);
            queue<vertex*> q;

            for (size_t i = 0; i < temp.size(); i++)
            {
                if(sequence[start] == temp[i]) continue;
                q.push(temp[i]);
            }
            
            while (q.size() != 0){
                vertex* temp_node = q.front();
                q.pop();

                if(temp_node->memberType != CORE){
                    continue;
                }

                vector<vertex*> R = getEpsilonNeighbourhood(temp_node);

                for(int index = 0; index < R.size(); index++){

                    // Check this line if its needed
                    if(temp_node == R[index]){
                        continue;
                    }

                    vertex* neighbour = R[index];
                    // If already a member of cluster, continue
                    if ((neighbour->isClassified == 1) && (neighbour->memberType != NON_MEMBER)){
                        continue;
                    }

                    // If already a non member dont push in queue as it is already a non core vertex
                    if(neighbour->memberType != NON_MEMBER){
                        q.push(neighbour);
                    }


                    if(isCore(neighbour) == true){
                        neighbour->memberType = CORE;
                    }
                    else{
                        neighbour->memberType = NON_CORE_MEMBER;
                    }

                    neighbour->clusterId = cluster_id;

                    neighbour->isClassified == 1;
                    cluster.push_back(neighbour);


                }
            }
            clusters.push_back(cluster);
            cluster_id++;
        }
        else{
            sequence[start]->isClassified = 1;
            sequence[start]->memberType = NON_MEMBER;
        }

    }

    
    // for each non_member vertex check whether its a hub or an outlier


    for(int start = 0; start < sequence.size(); start++){
        if(sequence[start]->memberType == NON_MEMBER){
            vector<vertex*> neighbours = inputGraph->graphObject[sequence[start]];
            vector<int> cluster_ids;

            for(int i = 0; i < neighbours.size(); i++){
                if (neighbours[i]->memberType != NON_MEMBER)
                    cluster_ids.push_back(neighbours[i]->clusterId);
            }

            if(cluster_ids.size() > 0){
                int flag = 1;
                for(int i = 1; i < cluster_ids.size(); i++){
                    if(cluster_ids[i] == cluster_ids[i-1]){
                        flag = 1;
                    }
                    else{
                        flag = 0;
                        break;
                    }
                }

                if(flag == 0){
                    sequence[start]->hub_or_outlier = HUB;
                    inputGraph->hubs.push_back(sequence[start]);
                }
                else{
                    sequence[start]->hub_or_outlier = OUTLIER;
                    inputGraph->outliers.push_back(sequence[start]);
                }
            }
            else{
                sequence[start]->hub_or_outlier = OUTLIER;
                inputGraph->outliers.push_back(sequence[start]);
            }

        }
    }



}

