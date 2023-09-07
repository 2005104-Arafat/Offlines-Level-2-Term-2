#include <iostream>
#include <fstream>
#include<sstream>
#include<cstring>
#include<string>
#include<chrono>
using namespace std;
using namespace std::chrono;
template<typename T>
class Node{
    public:
    T value;
    Node* left;
    Node* right;
    Node(T val){
        value=val;
        left=NULL;
        right=NULL;
    }
};
template<typename T>
class AVL{
    public:
    Node<T>* Insert(Node<T>* &root,T val){
        if (root==NULL){
            return new Node<T>(val);
        }
        if(val<root->value){
            root->left=Insert(root->left,val);
        }else{
            root->right=Insert(root->right,val);
        }
        int bf=balancefactor(root);
        if (bf > 1 &&  val < root->left->value){
            root=rightRotate(root);
        }
        else if (bf < -1 && val > root->right->value){
            root=leftRotate(root);
        }
        else if (bf > 1 && val > root->left->value) {
            root->left =leftRotate(root->left);
            root=rightRotate(root);
        }
        else if (bf < -1 && val < root->right->value) {
            root->right =rightRotate(root->right);
            root=leftRotate(root);
        }
        return root;
    }
    Node<T>* rightRotate(Node<T>* & root){
        Node<T>* temp1=root->left;
        Node<T>* temp2=temp1->right;
        temp1->right=root;
        root->left=temp2;
        return temp1;
    }
    Node<T>* leftRotate(Node<T>* & root){
        Node<T>* temp1=root->right;
        Node<T>* temp2=temp1->left;
        temp1->left=root;
        root->right=temp2;
        return temp1;
    }
    int balancefactor(Node<T>* &root){
        if (root == NULL)
            return 0;
        return height(root->left) - height(root->right);
    }
    int height(Node<T>* &root) {
        if (root== NULL)
            return 0;
        return max(height(root->left),height(root->right))+1;
    }
    void InOrder(Node<T>* &root,ofstream & output){
        if(root==NULL){
            return;
        }
        InOrder(root->left,output);
        output<<root->value<<" ";
        InOrder(root->right,output);
    }
    bool Find(Node<T>* &root, T val){
        if(root==NULL){
            return false;
        }
        if(root->value==val){
            return true;
        }
        else if(val > root->value){
            return Find(root->right,val);
        }
        else if(val<root->value){
            return Find(root->left,val);
        }
    }
    Node<T> * Delete(Node<T>* &root,T val){
        if(root==NULL){
            return NULL;
        }
        if(val< root->value){
            root->left=Delete(root->left,val);
        }
        else if(val>root->value){
            root->right=Delete(root->right,val);
        }
        else{
            if(root->left==NULL){
                Node<T>* temp=root->right;
                delete root;
                return temp;
            }
            else if(root->right==NULL){
                Node<T>* temp=root->left;
                delete root;
                return temp;
            }
            Node<T>* temp= InOrderSuccessor(root->right);
             root->value=temp->value;
            root->right=Delete(root->right,temp->value);
        }
        int bf=balancefactor(root);
        if(bf==2 && balancefactor(root->left)>=0){
            root=rightRotate(root);
        }
        else if(bf==2 && balancefactor(root->left)==-1){
            root->left=leftRotate(root->left);
            root=rightRotate(root);
        }
        else if (bf == -2 && balancefactor(root->right)<=0){
            root=leftRotate(root);
        } 
        else if (bf == -2 && balancefactor(root->right)== 1) {
            root->right = rightRotate(root->right);
            root=leftRotate(root);
        }
        return root;
    }
    Node<T>* InOrderSuccessor(Node<T>* &root){
        Node<T>* curr=root;
        while(curr && curr->left!=NULL){
            curr=curr->left;
        }
        return curr;
    }
    void print(Node<T>* root, string &str){
        if (root == NULL){
            return;
        }
        str=str+to_string(root->value);
        if (!root->left && !root->right){
            return;
        }
        str=str+"(";
        print(root->left, str);
        str=str+",";
        print(root->right, str);
        str=str+")";  
    }
};

int main(){
    ifstream myInput;
    ofstream myOutput;
    ofstream myOutput2;
    myOutput.open("out_avl.txt");
    myOutput2.open("report_avl.txt");
    myInput.open("in.txt");
    if(!myInput){
        cout<<"File not valid"<<endl;
    }
    string line;
    Node<int>* root=NULL;
    AVL<int> avl;
    char options;
    string item;
    double total_insert=0,total_delete=0,total_find=0,total_traverse=0;
    int count_insert=0,count_delete=0,count_find=0,count_traverse=0;
    while(getline(myInput,line)){
        istringstream s(line);
        s>>options;
        s>>item;
        switch(options){
            case 'I':{
                int num1=stoi(item);
                auto start =high_resolution_clock::now();
                root=avl.Insert(root,num1);
                auto end =high_resolution_clock::now();
                duration<double,milli> duration=(end-start);
                total_insert+=duration.count();
                count_insert++;
                string str1="";
                avl.print(root, str1);
                myOutput<<str1<<endl;
                break;
            }
            case 'D':{
                int num2=stoi(item);
                auto start =high_resolution_clock::now();
                root=avl.Delete(root,num2);
                auto end =high_resolution_clock::now();
                duration<double,milli> duration=(end-start);
                total_delete+=duration.count();
                count_delete++;
                if(avl.Find(root,num2)){
                    myOutput<<"Invalid Operation"<<endl;
                }else{
                    string str1 = "";
                    avl.print(root, str1);
                    myOutput<<str1<<endl;
                }
                break;
            }
            case 'F':{
                int num3=stoi(item);
                auto start =high_resolution_clock::now();
                bool sign=avl.Find(root,num3);
                auto end =high_resolution_clock::now();
                duration<double,milli> duration=(end-start);
                total_find+=duration.count();
                count_find++;
                if(sign){
                    myOutput<<"found"<<endl;
                }else{
                    myOutput<<"not found"<<endl;
                }
                break;
            }
            case 'T':{
                auto start =high_resolution_clock::now();
                avl.InOrder(root,myOutput);
                auto end =high_resolution_clock::now();
                duration<double,milli> duration=(end-start);
                total_traverse+=duration.count();
                count_traverse++;
                myOutput<<endl;
                break;
            }
        }
    }
    myOutput2<<"operation	time(ms)"<<endl;
    myOutput2<<"insert	"<<total_insert<<endl;
    myOutput2<<"delete	"<<total_delete<<endl;
    myOutput2<<"search	"<<total_find<<endl;
    myOutput2<<"trav	"<<total_traverse<<endl;
    myOutput2<<"total	"<<(total_delete+total_find+total_insert+total_traverse)<<endl;
    myOutput2<<endl;
    myOutput2<<"operation	avg_time(ms)"<<endl;
    myOutput2<<"insert	"<<total_insert/count_insert<<endl;
    myOutput2<<"delete	"<<total_delete/count_delete<<endl;
    myOutput2<<"search	"<<total_find/count_find<<endl;
    myOutput2<<"trav	"<<total_traverse/count_traverse<<endl;
    double avg_total=(total_delete+total_find+total_insert+total_traverse)/(count_insert+count_delete+count_find+count_traverse);
    myOutput2<<"total   "<<avg_total<<endl;
}
