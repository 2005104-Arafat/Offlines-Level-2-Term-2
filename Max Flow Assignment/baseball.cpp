#include <iostream>
#include <vector>
#include <fstream>
#include<sstream>
#include<cstring>
#include<queue>
using namespace std;
class Team{
    public:
        int win,loss, left;
        string name;

        Team(string n, int w, int lo,int le){
            name=n;
            win=w;
            loss=lo;
            left=le;
        }
};

class FlowNetwork{
    public:
        int vertices;
        vector<vector<int>> matrix;
        vector<int> teams;
        vector<bool> visited;
        vector<int> parent;
        int total_team;
        FlowNetwork(int v,int t) {
            vertices= v;
            matrix = vector<vector<int>>(vertices,vector<int>(vertices,0));
            total_team=t;
            visited=vector<bool> (vertices);
            parent=vector<int>(vertices,-1);
        }
        void add(vector<Team> team,vector<vector<int>> residual,int test ){
            matrix = vector<vector<int>>(vertices,vector<int>(vertices,0));
            int k=total_team;
            for(int i=0;i<total_team;i++){
                for(int j=i+1;j<total_team;j++){
                    if(i==test || j==test){
                        k++;
                        continue;
                    }
                    matrix[vertices-2][k]=residual[i][j];
                    matrix[k][i]=INT32_MAX;
                    matrix[k][j]=INT32_MAX;
                    k++;
                }
                matrix[i][vertices-1]=team[test].win+team[test].left-team[i].win;
            }
            matrix[test][vertices-1]=0;
        }

        int Edmonds_Karp(int source,int destination,int test){

            for (int i = 0; i < vertices; i++){
                visited[i]=false;
            }
            teams.clear();
            vector<vector<int>> temp(vertices,vector<int>(vertices));
            for(int i=0;i<vertices;i++){
                for(int j=0;j<vertices;j++){
                    temp[i][j]=matrix[i][j];
                }
            }
            
            for (int i = 0; i < vertices; i++){
                parent[i]=-1;
            }
            int max_flow=0;
            while(BFS(temp,source,destination)){
                int flow=INT32_MAX;
                for(int i=destination;i!=source;i=parent[i]){
                    int u=parent[i];
                    flow=min(flow,temp[u][i]);
                }
                for(int i=destination;i!=source;i=parent[i]){
                    int u=parent[i];
                    temp[u][i]-=flow;
                    temp[i][u]+=flow;
                    
                }
                max_flow+=flow;
            }
            for(int i=0;i<total_team;i++){
                if(BFS(temp,source,i)){
                    teams.push_back(i);
                }
            }
            
            return max_flow;
        }

        bool BFS(vector<vector<int>> temp,int source, int destination){
            for (int i = 0; i < vertices; i++){
                visited[i]=false;
            }
            queue<int> bfs;
            visited[source]=true;
            bfs.push(source);
            while (!bfs.empty()){
                int s=bfs.front();
                //cout<<s<<"->";
                bfs.pop();
                for (int i = 0; i <vertices; i++){
                    if(!visited[i] && temp[s][i]>0){
                        if(i==destination){
                            parent[i]=s;
                            //cout<<i<<endl;
                            return true;
                        }
                        bfs.push(i);
                        parent[i]=s;
                        visited[i]=true;
                    }
                }
                
            }
            return false; 
        }
        int sum(){
            int s=0;
            for(int i=0;i<vertices;i++){
                s+=matrix[vertices-2][i];
            }
            return s;
        }
        int check(int test, vector<Team> team){
            for(int i=0;i<total_team;i++){
                if(team[i].win>team[test].win+team[test].left){
                    teams.push_back(i);
                    return i;
                }
            }
            return -1;
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
    int total_teams;
    s>>total_teams;
    vector<Team> team;
    vector<vector<int>> residual(total_teams,vector<int>(total_teams));
    for(int i=0;i<total_teams;i++){
        getline(myInput,line);
        istringstream s3(line);
        string n;
        int w,lo,le;
        s3>>n;
        s3>>w;
        s3>>lo;
        s3>>le;
        team.emplace_back(n,w,lo,le);
        for (int j = 0; j < total_teams; j++){
            int q;
            s3>>q;
            residual[i][j]=q;
        }
    }
    int vertices=2+total_teams+(total_teams*(total_teams-1))/2;
    FlowNetwork flowNetwork(vertices,total_teams);
    for(int i=0;i<total_teams;i++){
        flowNetwork.add(team,residual,i);
        //cout<<flowNetwork.Edmonds_Karp(vertices-2,vertices-1,i)<<" "<<flowNetwork.sum()<<endl;
        if(flowNetwork.Edmonds_Karp(vertices-2,vertices-1,i)!=flowNetwork.sum()){
            myOutput<<team[i].name<<" is eliminated."<<endl;
            myOutput<<"They can win at most "<<team[i].win<<" + "<<team[i].left<<" = "<<(team[i].win+team[i].left)<<" games."<<endl;
            int check=flowNetwork.check(i,team);
            if(check!=-1){
                myOutput<<team[check].name<<" has won a total of "<<team[check].win<<" games."<<endl;
                myOutput<<"They play each other 0 times."<<endl;
                myOutput<<"So on average, each of the teams in this group wins "<<team[check].win<<"/1 = "<<team[check].win<<" games."<<endl;
                myOutput<<endl;
                continue;
            }
            int total_win=0,total_left=0,count=0;
            for(int i=0;i<flowNetwork.teams.size();i++){
                total_win+=team[flowNetwork.teams[i]].win;
                count++;
            }
            for(int i=0;i<flowNetwork.teams.size();i++){
                for(int j=0;j<flowNetwork.teams.size();j++){
                    total_left+=residual[flowNetwork.teams[i]][flowNetwork.teams[j]];
                }
            }
            for(int i=0;i<flowNetwork.teams.size();i++){
                myOutput<<team[flowNetwork.teams[i]].name;
                if(i==count-2){
                    myOutput<<" and ";
                    continue;
                }
                if(i==count-1){
                    continue;
                }
                myOutput<<" , ";
            }
            myOutput<<" have won a total of "<<total_win<<" games."<<endl;
            myOutput<<"They play each other "<<total_left/2<<" times."<<endl;;
            myOutput<<"So on average, each of the teams wins "<<(total_win+total_left/2)<<"/"<<count<<" = "<<((float)(total_win+(float)(total_left)/2))/count<<" games."<<endl;
            myOutput<<endl;
        }
   
    }
}



