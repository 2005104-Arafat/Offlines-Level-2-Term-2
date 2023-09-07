#include<iostream>
#include <fstream>
#include<sstream>
#include<cstring>
#include<vector>
#include<queue>
using namespace std;
#define INF 10e5
class Graph{
    public:
    vector<pair<int,int>> *adjacent;
    vector<int> key;
    vector<int> parent;
    vector<bool> visited;
    int n;
    int cost;
    Graph(int n){
        this->n=n;
        adjacent=new vector<pair<int,int>> [n];
        cost= 0;
        parent=vector<int>(n,-1);
        key= vector<int>(n,INF);
        visited=vector<bool>(n,false);
    }
    void edge(int u, int v, int w){
        adjacent[u].push_back(make_pair(v,w));
        adjacent[v].push_back(make_pair(u,w));
    }
    void MST(ofstream &output){
        priority_queue<pair<int,int>,vector <pair<int,int>> , greater<pair<int,int>> > heap;
        int source=0;
        heap.push(make_pair(0,source));
        key[source]=0;
        while(!heap.empty()){
            int u=heap.top().second;
            int c=heap.top().first;
            heap.pop();
            if(visited[u]){
                continue;
            }
            visited[u]=true;
            cost+=c;
            vector<pair<int,int>>:: iterator it;
            for(it=adjacent[u].begin();it!=adjacent[u].end();it++){
                int v=(*it).first;
                int w=(*it).second;
                if(!visited[v] && key[v]>w){
                    key[v]=w;
                    heap.push(make_pair(key[v],v));
                    parent[v]=u;
                }
            }
        }
        output<<"Prim-Jarnik Algorithm: "<<endl;
        output<<"Total weight = "<<cost<<endl;
        output<<"Root node = "<<source<<endl;
        for(int i=1;i<n;i++){
            output<<parent[i]<<" "<<i<<endl;
        }
        
    }
};
int main(){
    ifstream myInput;
    ofstream myOutput;
    myOutput.open("prim.txt");
    myInput.open("in.txt");
    if(!myInput){
        cout<<"File not valid"<<endl;
    }
    string line;
    getline(myInput,line);
    istringstream s(line);
    int n,m;
    s>>n;
    s>>m;
    Graph graph(n);
    for(int i=0;i<m;i++){
        getline(myInput,line);
        istringstream s3(line);
        int u,v,w;
        s3>>u;
        s3>>v;
        s3>>w;
        graph.edge(u,v,w);
    }
    graph.MST(myOutput);
}
