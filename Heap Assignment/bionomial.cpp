#include<iostream>
#include<fstream>
#include<sstream>
#include<cstring>
#include<algorithm>
#include<vector>
#include<queue>
using namespace std;
class Node{
    public:
    int data;
    int degree;
    Node *l_child;
    Node *r_sibling;
    Node *parent;

    Node(int key){
        data=key;
        degree=0;
        l_child=NULL;
        parent=NULL;
        r_sibling=NULL;
    }
};
class Heap{
    public:
    int min;
    Node* minNode;
    vector<Node*> mainHeap;
    Heap(){
        min=INT32_MAX;
        minNode=NULL;
    }
    vector<Node *> Union(vector<Node *> &Bk,vector<Node *> &Bj){
        vector<Node *>::iterator ik=Bk.begin();
        vector<Node *>::iterator ij=Bj.begin();
        vector<Node *> temp;
        while(ij!=Bj.end() && ik!=Bk.end()){
            if((*ij)->degree<(*ik)->degree){
                temp.push_back(*ij);
                ij++;
            }else{
                temp.push_back(*ik);
                ik++;
            }
        }
        while(ik!=Bk.end()){
            temp.push_back(*ik);
            ik++;
        }
        while (ij!=Bj.end()){
            temp.push_back(*ij);
            ij++;
        }
        temp=adjustHeap(temp);
        return temp;
    }
    
    vector<Node *> adjustHeap(vector<Node *> &temp){
        vector<Node *>::iterator x,n_x,next_nx;
        x=n_x=next_nx=temp.begin();
        if(temp.size()==1){
            return temp;
        }
        int i=0;
        while(x!=temp.end()){
            if(temp.size()==1){
                n_x=temp.end();
                next_nx=temp.end();
            }else if(temp.size()==2){
                n_x = x;
                n_x++;
                next_nx=temp.end();
            }else{
                n_x=x;
                n_x++;
                next_nx=n_x;
                next_nx++;
            }
            if(n_x==temp.end()){
                x++;
            }
            else if((*x)->degree<(*n_x)->degree){
                x++;
            }
            else if( next_nx!=temp.end() && (*x)->degree==(*n_x)->degree && (*x)->degree==(*next_nx)->degree ){
                x++;
            }
            else if((*x)->degree==(*n_x)->degree){
                *x=mergeHeap(*x,*n_x);
                n_x=temp.erase(n_x);
            }
        }
        return temp;
    }

    Node* mergeHeap(Node* x, Node* n_x){
        Node* temp;
        if(x->data>n_x->data){
            temp=n_x->l_child;
            n_x->l_child=x;
            x->r_sibling=temp;
            n_x->degree++;
            x->parent=n_x;
            n_x->parent = NULL;
            n_x->r_sibling = NULL;
            return n_x;
        }else{
            temp=x->l_child;
            x->l_child=n_x;
            n_x->r_sibling=temp;
            x->degree++;
            n_x->parent=x;
            x->parent = NULL;
            x->r_sibling = NULL;
            return x;
        }
    }

    vector<Node*> Insert(vector<Node*> &heap,int val){
        Node* temp_node=new Node(val);
        if(heap.size()==0){
            heap.push_back(temp_node);
            return heap;
        }
        vector<Node*> temp;
        temp.push_back(temp_node);
        heap=Union(heap,temp);
        return heap;
    }
    Node* Find_Min(vector<Node*> &heap){
        Node* min=*(heap.begin());
        
       for(auto it=heap.begin();it!=heap.end();it++){
            if((*it)->data<min->data){
                min=(*it);
            }
       }
       return min;
    }

    Node* Extract_Min(vector<Node*> heap){
        Node* temp=Find_Min(heap);
        Node* child=temp->l_child;
        vector<Node*> childHeap;
        vector<Node*> tempHeap;
        Node* newBk;
        for(auto it=heap.begin();it!=heap.end();it++){
            if((*it)!=temp){
                tempHeap.push_back(*it);
            }
        }
        while(child){
            newBk=child;
            child=child->r_sibling;
            newBk->r_sibling=NULL;
            childHeap.push_back(newBk);
        }
        reverse(childHeap.begin(),childHeap.end());
        mainHeap=Union(tempHeap,childHeap);
        tempHeap.clear();
        childHeap.clear();
        return temp;
    }

    void Print(vector<Node*> &heap, ofstream &output) {
        output << "Printing Binomial Heap..." << endl;
        for (auto root : heap){
            queue<Node*> q;
            q.push(root);
            int level = 0;
            output << "Binomial Tree, B" << root->degree << endl;
            while (!q.empty()) {
                int levelSize = q.size();
                for (int i = 0; i < levelSize; i++) {
                    Node* current = q.front();
                    q.pop();
                    if(i==0){
                        output << "Level " << level << " : "<<current->data<<" ";
                    }else{
                        output<<current->data<<" ";
                    }
                    Node* child = current->l_child;
                    while (child){
                        q.push(child);
                        child = child->r_sibling;
                    }
                }
                output << endl;
                level++;
            }
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
    Heap bionomial;
    string line;
    char option;
    int item;
    queue<int> item2;
    while(getline(myInput,line)){
        istringstream s(line);
        s>>option;
        if(option=='U'){
            int e;
            while(s>>e){
                item2.push(e);
            }
        }else if(option=='I'){
            s>>item;
        }
        switch (option){
            case 'I' :{
                bionomial.mainHeap=bionomial.Insert(bionomial.mainHeap,item);
                break;
            }
            case 'U':{
                vector<Node*> unionHeap;
                while(!item2.empty()){
                    int u=item2.front();
                    item2.pop();
                    unionHeap=bionomial.Insert(unionHeap,u);
                }
                bionomial.mainHeap=bionomial.Union(bionomial.mainHeap,unionHeap);
                break;
            }
            case 'F':{
                Node* find=bionomial.Find_Min(bionomial.mainHeap);
                myOutput<<"Find-Min returned "<<find->data<<endl;
                break;
            }
            case 'E':{
                Node* extract=bionomial.Extract_Min(bionomial.mainHeap);
                myOutput<<"Extract-Min returned "<<extract->data<<endl;
                break;
            }
            case 'P':{
                bionomial.Print(bionomial.mainHeap,myOutput);
                break;
            }
            default:
                break;
        }
    }
    
}

