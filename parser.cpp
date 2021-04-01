#include<bits/stdc++.h>


using namespace std;

int main(int argc, char* argv[])
{
    ifstream F(argv[1]);


    int a, b;
    int nedges = 0;
    string line;
    while(getline(F, line)){nedges++;}

    F.clear();
    F.seekg(0);
    set<pair<int, int>> S;
    for(int i=0;i<nedges;i++){
        F >> a >> b;
        
        if (a == b) continue;
        S.insert({min(a, b), max(a, b)});
    }

    string rawname = argv[2];
    ofstream MyFile(rawname);

    for(auto iter : S){
        MyFile << iter.first << "   " << iter.second << endl; 
    }


    return (0);


         
    
}