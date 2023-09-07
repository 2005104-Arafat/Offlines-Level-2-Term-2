#include <iostream>
#include <vector>
#include <fstream>
#include<sstream>
#include<cstring>
using namespace std;
#define INF 10e5
class Graph{
    public:
        vector<vector<int>> adjoint;
        int n;
        int m;
        Graph(int n1, int m1){
            this->n = n1;
            this->m = m1;
            adjoint = vector<vector<int>>(n + 1, vector<int>(n + 1, INF));
        }
        void add(int u, int v, int w){
            adjoint[u][v] = w;
        }
        void Floyd_Warshall(ofstream &output){
            for(int i=1;i<=n;i++){
                adjoint[i][i]=0;
            }
            output<<"Shortest distance matrix"<<endl;
            for(int i=1;i<=n;i++){
                for(int j=1;j<=n;j++){
                    for(int k=1;k<=n;k++){
                        if(j==i || k==i || j==k){
                            continue;
                        }
                        if(adjoint[j][k]>(adjoint[j][i]+adjoint[i][k])){
                            adjoint[j][k]=adjoint[j][i]+adjoint[i][k];
                        }
                    }
                }
            }
            for(int i=1;i<=n;i++){
                for(int j=1;j<=n;j++){
                    if(adjoint[i][j]==INF){
                        output<<"INF"<<" ";
                    }else{
                        output<<adjoint[i][j]<<" ";
                    }
                }
                output<<endl;
            }
        }
        vector<vector<int>> extend_path(vector<vector<int>> L){
            vector<vector<int>> new_L(n + 1, vector<int>(n + 1, INF));
            for(int i=1;i<=n;i++){
                for(int j=1;j<=n;j++){
                    for(int k=1;k<=n;k++){
                        new_L[i][j]=min(new_L[i][j],L[i][k]+adjoint[k][j]);
                    }
                }
            }
            return new_L;
        }
        void matrix(ofstream &output){
            for(int i=1;i<=n;i++){
                adjoint[i][i]=0;
            }
            output<<"Shortest distance matrix"<<endl;
            vector<vector<int>>L=adjoint;
            int m1=1;
            while(m1<n-1){
                L=extend_path(L);
                m1=2*m1;
            }
            for(int i=1;i<=n;i++){
                for(int j=1;j<=n;j++){
                    if(L[i][j]==INF){
                        output<<"INF"<<" ";
                    }else{
                        output<<L[i][j]<<" ";
                    }
                }
                output<<endl;
            }
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
    newGraph.Floyd_Warshall(myOutput);
    newGraph.matrix(myOutput);
}