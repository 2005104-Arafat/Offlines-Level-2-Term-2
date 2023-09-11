#include<bits/stdc++.h>
using namespace std;
class Graph{
    public:
    int vertices;
    vector<vector<int>> adjacent;
    vector<vector<int>> edge;
    vector<int> *adjoint;
    vector<int> metric_path;
    vector<int> size;
    vector<int> parent;
    Graph(int v=5){
        vertices=v;
        adjacent=vector<vector<int>>(v+1,vector<int>(v+1,0));
        adjoint=new vector<int> [v+1];
        parent=vector<int>(v+1);
        size=vector<int>(v+1);
        for(int i=1;i<=v;i++){
            start(i);
        }
    }

    vector<int> Create_Random_Graph(int v){
        for(int i=1;i<=v;i++){
            for (int j=i;j<=v;j++){
                if(i==j){
                    adjacent[i][j]=0;
                }else{
                    adjacent[i][j]=adjacent[j][i]=rand()%151+50;
                }
            }
        }
        for(int i=1;i<=v;i++){
            for(int j=1;j<=v;j++){
                for(int k=1;k<=v;k++){
                    if(j==k){
                        continue;
                    }
                    if(adjacent[j][k]>(adjacent[j][i]+adjacent[i][k])){
                        adjacent[j][k]=adjacent[j][i]+adjacent[i][k];

                    }
                }
            }
        }
        for(int i=1;i<=v;i++){
            for (int j=i;j<=v;j++){
                if(i==j){
                    continue;
                }else{
                    edge.push_back({adjacent[i][j],i,j});
                }
            }
        }
    }
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
    vector<vector<int>> kruskal(){
        sort(edge.begin(),edge.end());
        vector<vector<int>> result;
        for(auto i:edge){
            int w=i[0];
            int u=i[1];
            int v=i[2];
            int x=find_parents(u);
            int y=find_parents(v);
            if(x==y){
                continue;
            }else{
                result.push_back({u,v,w});
                join_sets(u,v);
            }
        }
        return result;
    }



    vector<int> Exact_TSP(){
        int n = adjacent.size();
    
        vector<vector<int>> dp(1 << n, vector<int>(n, INT32_MAX));
        vector<vector<int>> parents(1 << n, vector<int>(n, -1));
        
        dp[1][0] = 0;

        for (int mask = 1; mask < (1 << n); ++mask) {
            for (int u = 1; u <= n; ++u) {
                if (mask & (1 << u)) {
                    for (int v = 1; v <= n; ++v) {
                        if (v != u && (mask & (1 << v))) {
                            if (dp[mask ^ (1 << u)][v] + adjacent[v][u] < dp[mask][u]) {
                                dp[mask][u] = dp[mask ^ (1 << u)][v] + adjacent[v][u];
                                parents[mask][u] = v;
                            }
                        }
                    }
                }
            }
        }
        
        int minCost = INT32_MAX;
        int end = -1;

        for (int u = 1; u < n; ++u) {
            if (adjacent[u][0] != INT32_MAX && dp[(1 << n) - 1][u] + adjacent[u][0] < minCost) {
                minCost = dp[(1 << n) - 1][u] + adjacent[u][0];
                end = u;
            }
        }

        vector<int> exact_path;
        int mask = (1 << n) - 1;
        int current = end;

        while (current != -1) {
            exact_path.push_back(current); 
            int prev = parents[mask][current];
            mask ^= (1 << current);
            current = prev;
        }
        int last=exact_path.back();
        reverse(exact_path.begin(), exact_path.end());
        exact_path.push_back(last); 
        return exact_path;
    }    



    void dfs(int u ,int parent){
        metric_path.push_back(u);
        for(int v: adjoint[u]){
            if(v!=parent){
                dfs(v,u);
            }
        }
    }
    vector<int> Metric_Approximation_TSP(){
        for(int i=1;i<=vertices;i++){
            adjoint[i].clear();
        }
        vector<vector<int>> result=kruskal();
        for(auto i : result){
            adjoint[i[0]].push_back(i[1]);
            adjoint[i[1]].push_back(i[0]);
        }
        dfs(result[0][0],0);
        metric_path.push_back(result[0][0]);
        return metric_path;
    }
    int Calculate_Tour_Length(vector<int> cycle){
        int u=cycle.back();
        cycle.pop_back();
        int tour_length=0;
        while(!cycle.empty()){
            int v=cycle.back();
            cycle.pop_back();
            tour_length+=adjacent[u][v];
            u=v;
        }
        return tour_length;
    }
};

int main(){
    int x;
    cin>>x;
    for(int i=0;i<x;i++){
        Graph graph(20);
        graph.Create_Random_Graph(20);
        vector<int> Approximate_Tour_Length=graph.Metric_Approximation_TSP();
        vector<int> Optimal_Tour_Length=graph.Exact_TSP();
        double ratio=(double)(graph.Calculate_Tour_Length(Approximate_Tour_Length))/(double)(graph.Calculate_Tour_Length(Optimal_Tour_Length));
        cout<<i+1<<" : "<<ratio<<endl;
    }
}