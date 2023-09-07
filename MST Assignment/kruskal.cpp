#include<iostream>
#include <fstream>
#include<sstream>
#include<cstring>
#include<vector>
#include<algorithm>
using namespace std;
vector<int> size;
vector<int> parent;
void start(int v){
    parent[v]=v;
    size[v]=1;
}
int find_parents(int u){
    if(u==parent[u]){
        return u;
    }
    return parent[u]=find_parents(parent[u]);
}
void join_sets(int u, int v){
    u=find_parents(u);
    v=find_parents(v);
    if(u!=v){
        if(size[u]<size[v]){
            swap(u,v);
        }
        parent[v]=u;
        size[u]+=size[v];
    }
}
int main(){
    ifstream myInput;
    ofstream myOutput;
    myOutput.open("kruskal.txt");
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
    size=vector<int> (n);
    parent=vector<int> (n);
    for(int i=0;i<n;i++){
        start(i);
    }
    vector<vector<int>> edges;
    vector<vector<int>> result;
    for(int i=0;i<m;i++){
        getline(myInput,line);
        istringstream s3(line);
        int u,v;
        int w;
        s3>>u;
        s3>>v;
        s3>>w;
        edges.push_back({w,u,v});
    }
    int cost=0;
    sort(edges.begin(),edges.end());
    cout<<"A";
    for(auto i:edges){
        int w=i[0];
        int u=i[1];
        int v=i[2];
        int x=find_parents(u);
        int y=find_parents(v);
        if(x==y){
            continue;
        }else{
            result.push_back({u,v});
            cost+=w;
            join_sets(u,v);
        }
    }
    myOutput<<"Kruskal's Algorithm:"<<endl;
    myOutput<<"Total weight = ";
    myOutput<<cost<<endl;
    for(auto i:result){
        myOutput<<i[0]<<" "<<i[1]<<endl;
    }
}