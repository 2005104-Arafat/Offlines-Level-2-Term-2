#include<iostream>
#include<vector>
#include<cstdlib>
#include<queue>
#include <fstream>
#include<sstream>
#include<cstring>
#include<stack>
using namespace std;
#define INF 10e5
class Graph{
    vector<pair<int,int>> *graph;
    vector<vector<int>> edges;
    vector<int> distance;
    vector<int> parent;
    int n,m;
    public:
    Graph(int n,int m){
        this->n=n;
        this->m=m;
        graph=new vector<pair<int,int>>[n];
    }
    void add(int u, int v,int w){
        graph[u].push_back(make_pair(v,w));
        edges.push_back({u,v,w});
    }
    void dijkstra(int source,int destination,ofstream &output){
        output<<"Dijkstra Algorithm:"<<endl;
        distance=vector<int>(n,INF) ;
        parent=vector<int>(n,-1);
        priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> heap;
        distance[source]=0;
        heap.push(make_pair(0,source));
        while(!heap.empty()){
            int u=heap.top().second;
            heap.pop();
            vector<pair<int,int>>:: iterator it;
            for(it=graph[u].begin();it!=graph[u].end();it++){
                int v=(*it).first;
                int w=abs((*it).second);
                if(distance[u]+w<distance[v]){
                    parent[v]=u;
                    distance[v]=distance[u]+w;
                    heap.push(make_pair(distance[v],v));
                }
            }
        }
        output<<distance[destination]<<endl;
        stack<int> path;
        int i=destination;
        path.push(i);
        while(parent[i]!=-1){
            path.push(parent[i]);
            i=parent[i];
        }
        while(!path.empty()){
            output<<path.top();
            path.pop();
            if(!path.empty()){
                output<<"->";
            }
        }
        output<<endl;
    }
    void bellman_ford(int source,int destination,ofstream &output){
        output<<"Bellman Ford Algorithm: "<<endl;
        distance=vector<int>(n,INF) ;
        parent=vector<int>(n,-1);
        bool change;
        distance[source]=0;
        for(int i=0;i<n-1;i++){
            vector<vector<int>> :: iterator it;
            change=false;
            for(it=edges.begin();it!=edges.end();it++){
                int u=(*it)[0];
                int v=(*it)[1];
                int w=(*it)[2];
                if((distance[u]+w)<distance[v]){
                    distance[v]=distance[u]+w;
                    parent[v]=u;
                    change=true;
                }
            }
        }
        if(change){
            output<<"Negative weight cycle present"<<endl;
            output<<endl;
            return;
        }
        output<<distance[destination]<<endl;
        stack<int> path;
        int i=destination;
        path.push(i);
        while(parent[i]!=-1){
            path.push(parent[i]);
            i=parent[i];
        }
        while(!path.empty()){
            output<<path.top();
            path.pop();
            if(!path.empty()){
                output<<"->";
            }
        }
        output<<endl;
        output<<endl;
    }
};
int main(){
    ifstream myInput;
    ofstream myOutput;
    myOutput.open("out.txt");
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
    Graph newGraph(n,m);
    for(int i=0;i<m;i++){
        getline(myInput,line);
        istringstream s3(line);
        int u,v,w;
        s3>>u;
        s3>>v;
        s3>>w;
        newGraph.add(u,v,w);
    }
    getline(myInput,line);
    istringstream s1(line);
    int source,destination;
    s1>>source;
    s1>>destination;
    newGraph.bellman_ford(source,destination,myOutput);
    newGraph.dijkstra(source,destination,myOutput);
}