#include<iostream>
#include<vector>
#include<string>
using namespace std;
class Node{
    public:
    int value;
    string key;
    Node* next;
    Node(){
        value=0;
        key="";
        next=NULL;
    }
    Node(int val,string w){
        value=val;
        key=w;
        next=NULL;
    }
};

class Hashing{
    public:
    vector<Node*> hashTable;
    int maxSize;
    int size;
    int collision;
    int probe;
    int method;
    vector<int> length;
    Hashing(int s,int m){ 
        maxSize=s;
        size=0;
        collision=0;
        method=m;
        probe=0;
        hashTable = vector<Node*>(s);
        length=vector<int>(s);
        for(int i=0;i<s;i++){
            hashTable[i]=NULL;
            length[i]=0;
        }
    }

    int hash1(string key){//Polynomial Rolling Hash Function
        int hash = 0;
        int n = maxSize;
        for(int i = 0; i<key.length();  i++){
            hash = 37*hash+key[i];
        }
        hash %= n;
        if(hash<0){
            hash += n;
        }
        return hash;
    }

    int hash2(string key){// x-or shift hash
        unsigned int n = maxSize;
        int hash = 0;
        for (char c : key){
            hash = (hash ^ ((c - 'a' + 1) + (hash<<5) + (hash>>2) ))%n;
        }
        if(hash<0){
            hash+=maxSize;
        }
        return hash%n ;
    }

    int aux_hash(string key){
        int hash = 1;
        int n = maxSize;
        for(int i = 0; i<key.length();  i++){
            hash = ((hash*37) + ((int)key[i]))%n;
        }
        hash =(hash*10 + 1)% maxSize;
        if(hash<0) hash += maxSize;
        return hash;
    }
    int getSize(){
        return size;
    }
    int getCollision(){
        return collision;
    }
    void setProbe(int p){
        probe=p;
    }
    int getProbe(){
        return probe;
    }
    void Insert(string key, int function){
        if(size==maxSize){
            return;
        }
        // if(Find(key,function)!=-1){
        //     return;
        // }
        int position;
        if(method==1){
            if(function==1){
                position=hash1(key);
            }else{
                position=hash2(key);
            }
            if(length[position]==1){
                collision++;
            }
            Node* newNode=new Node(position,key);
            if(hashTable[position]==NULL){
                hashTable[position]=newNode;
                size++;
                length[position]++;
            }else{
                newNode->next=hashTable[position];
                hashTable[position]=newNode;
                size++;
                length[position]++;
            }
        }
        else if(method==2){
            int position1;
            if(function==1){
                position1=hash1(key);
            }else{
                position1=hash2(key);
            }
            int position2=aux_hash(key);
            for(int i=0;i<maxSize;i++){
                position=(position1+i*position2)%maxSize;
                //doubleHash(k, i) = (Hash(k) + i × auxHash(k)) % N
                if(hashTable[position]==NULL){
                    Node* newNode=new Node(position,key);
                    hashTable[position]=newNode;
                    size++;
                    break;
                }
                collision++;
            }
        }
        else{
            int c1=17,c2=19;
            int position1;
            if(function==1){
                position1=hash1(key);
            }else{
                position1=hash2(key);
            }
            int position2=aux_hash(key);
            for(int i=0;i<maxSize;i++){
                position=(position1+c1*((position2*i)%maxSize)+(c2*((i*i)%maxSize))%maxSize)%maxSize;
                //customHash(k, i) = (Hash(k) + C1 × i × auxHash(k) + C2 × i2) % N
                if(hashTable[position]==NULL){
                    Node* newNode=new Node(position,key);
                    hashTable[position]=newNode;
                    size++;
                    break;
                }
                collision++;
            }
        }
    }
    int Find(string key, int function){
        int position;
        if(method==1){
            if(function==1){
                position=hash1(key);
            }else{
                position=hash2(key);
            }
            Node* temp=hashTable[position];
            while(temp!=NULL){
                if(temp->key==key){
                    probe++;
                    return position;
                }
                probe++;
                temp=temp->next;
            }
            return -1;
        }
        else if(method==2){
            int position1;
            if(function==1){
                position1=hash1(key);
            }else{
                position1=hash2(key);
            }
            int position2=aux_hash(key);
            int i=0;
            position=position1;
            while(i<maxSize && hashTable[position]!=NULL){
                position=(position1+i*position2)%maxSize;
                //doubleHash(k, i) = (Hash(k) + i × auxHash(k)) % N
                if(hashTable[position]!=NULL){
                    if(hashTable[position]->key==key ){
                        probe++;
                        return position;
                    }
                }
                probe++;
                i++;
            }
            return -1;
        }
        else{
            int c1=17,c2=19;
            int position1;
            if(function==1){
                position1=hash1(key);
            }else{
                position1=hash2(key);
            }
            int position2=aux_hash(key);
            for(int i=0;i<maxSize;i++){
                position=(position1+c1*((position2*i)%maxSize)+(c2*((i*i)%maxSize))%maxSize)%maxSize;
                //customHash(k, i) = (Hash(k) + C1 × i × auxHash(k) + C2 × i2) % N
                if(hashTable[position]!=NULL){
                    if(hashTable[position]->key==key){
                        probe++;
                        return position;
                    }
                }
                probe++;
            }
            return -1;
        }
    }
    void Delete(string key,int function){
        int position;
        if(method==1){
            if(function==1){
                position=hash1(key);
            }else{
                position=hash2(key);
            }
            if(hashTable[position]==NULL){
                return;
            }
            Node* temp=hashTable[position];
            if(temp->key == key){
                hashTable[position] = temp->next;
                delete temp;
                length[position]--;
                size--;
                return;
            }
            Node* prev;
            while(temp != NULL){
                if(temp->key == key){
                    prev->next = temp->next;
                    delete temp;
                    length[position]--;
                    size--;
                }
                prev = temp;
                temp = temp->next;
            }
        }
        else if(method==2){
            int position1;
            if(function==1){
                position1=hash1(key);
            }else{
                position1=hash2(key);
            }
            int position2=aux_hash(key);
            for(int i=0;i<maxSize;i++){
                position=(position1+i*position2)%maxSize;
                //doubleHash(k, i) = (Hash(k) + i × auxHash(k)) % N
                if(hashTable[position]->key==key){
                    hashTable[position]=NULL;
                    size--;
                    return;
                }
            }
        }
        else{
            int c1=17,c2=19;
            int position1;
            if(function==1){
                position1=hash1(key);
            }else{
                position1=hash2(key);
            }
            int position2=aux_hash(key);
            for(int i=0;i<maxSize;i++){
                position=(position1+c1*((position2*i)%maxSize)+(c2*((i*i)%maxSize))%maxSize)%maxSize;
                //customHash(k, i) = (Hash(k) + C1 × i × auxHash(k) + C2 × i2) % N
                if(hashTable[position]->key==key){
                    hashTable[position]=NULL;
                    size--;
                    return;
                }
            }
        }
    }
};

string Randomword(){
    int l = 5 + rand() % 6;
    vector<char> letters={'a','b','c','d','e','f','g','h',
                          'i','j','k','l','m','n','o','p',
                          'q','r','s','t','u','v','w','x','y','z'};
    string s="";
    for (int i = 0; i < l; i++){
        int j= rand() % 26;
        s+=letters[j];
    }
    return s;
}

bool isPrime(int n){
    if(n<=1){
        return false;
    }
    if(n<=3){
        return true;
    }
    if(n%2==0 || n%3 == 0){
        return false;
    }
   
    for (int i=5; i*i<=n; i=i+6){
        if (n%i==0 || n%(i+2)==0){
            return false;
        }
    }
    return true;
}
 
int nextPrime(int N){
    if (N <= 1){
        return 2;
    }
    int next = N;
    bool found = false;
    while (!found) {
        next++;
        if (isPrime(next)){
            found = true;
        }
    }
    return next;
}
int main(){
    Hashing sC1_5(nextPrime(5000),1);
    Hashing sC2_5(nextPrime(5000),1);
    Hashing dH1_5(nextPrime(5000),2);
    Hashing dH2_5(nextPrime(5000),2);
    Hashing cP1_5(nextPrime(5000),3);
    Hashing cP2_5(nextPrime(5000),3);
    Hashing sC1_10(nextPrime(10000),1);
    Hashing sC2_10(nextPrime(10000),1);
    Hashing dH1_10(nextPrime(10000),2);
    Hashing dH2_10(nextPrime(10000),2);
    Hashing cP1_10(nextPrime(10000),3);
    Hashing cP2_10(nextPrime(10000),3);
    Hashing sC1_20(nextPrime(20000),1);
    Hashing sC2_20(nextPrime(20000),1);
    Hashing dH1_20(nextPrime(20000),2);
    Hashing dH2_20(nextPrime(20000),2);
    Hashing cP1_20(nextPrime(20000),3);
    Hashing cP2_20(nextPrime(20000),3);
    vector<string> seed;
    for(int i=0;i<10000;i++){
        string s="";
        s+=Randomword();
        if(sC1_10.Find(s,1)!=-1){
            i--;
            continue;
        }
        sC1_5.Insert(s,1);
        sC2_5.Insert(s,2);
        dH1_5.Insert(s,1);
        dH2_5.Insert(s,2);
        cP1_5.Insert(s,1);
        cP2_5.Insert(s,2);
        sC1_10.Insert(s,1);
        sC2_10.Insert(s,2);
        dH1_10.Insert(s,1);
        dH2_10.Insert(s,2);
        cP1_10.Insert(s,1);
        cP2_10.Insert(s,2);
        sC1_20.Insert(s,1);
        sC2_20.Insert(s,2);
        dH1_20.Insert(s,1);
        dH2_20.Insert(s,2);
        cP1_20.Insert(s,1);
        cP2_20.Insert(s,2);
        seed.push_back(s);
    }
    
    sC1_5.setProbe(0);
    sC2_5.setProbe(0);
    dH1_5.setProbe(0);
    dH2_5.setProbe(0);
    cP1_5.setProbe(0);
    cP2_5.setProbe(0);
    sC1_10.setProbe(0);
    sC2_10.setProbe(0);
    dH1_10.setProbe(0);
    dH2_10.setProbe(0);
    cP1_10.setProbe(0);
    cP2_10.setProbe(0);
    sC1_20.setProbe(0);
    sC2_20.setProbe(0);
    dH1_20.setProbe(0);
    dH2_20.setProbe(0);
    cP1_20.setProbe(0);
    cP2_20.setProbe(0);
    for(int i=0;i<1000;i++){
        int index=rand()%10000;
        sC1_5.Find(seed[index%5000],1);
        sC2_5.Find(seed[index%5000],2);
        dH1_5.Find(seed[index%5000],1);
        dH2_5.Find(seed[index%5000],2);
        cP1_5.Find(seed[index%5000],1);
        cP2_5.Find(seed[index%5000],2);
        sC1_10.Find(seed[index],1);
        sC2_10.Find(seed[index],2);
        dH1_10.Find(seed[index],1);
        dH2_10.Find(seed[index],2);
        cP1_10.Find(seed[index],1);
        cP2_10.Find(seed[index],2);
        sC1_20.Find(seed[index],1);
        sC2_20.Find(seed[index],2);
        dH1_20.Find(seed[index],1);
        dH2_20.Find(seed[index],2);
        cP1_20.Find(seed[index],1);
        cP2_20.Find(seed[index],2);
    }
    int SC2_5 =sC2_5.getCollision();
    int SC1_5 =sC1_5.getCollision();
    int DH1_5=dH1_5.getCollision();
    int DH2_5=dH2_5.getCollision();
    int CP1_5 =cP1_5.getCollision();
    int CP2_5 =cP2_5.getCollision();
    int SC1_10 =sC1_10.getCollision();
    int SC2_10 =sC2_10.getCollision();
    int DH1_10=dH1_10.getCollision();
    int DH2_10=dH2_10.getCollision();
    int CP1_10=cP1_10.getCollision();
    int CP2_10=cP2_10.getCollision();
    int SC1_20 =sC1_20.getCollision();
    int SC2_20 =sC2_20.getCollision();
    int DH1_20=dH1_20.getCollision();
    int DH2_20=dH2_20.getCollision();
    int CP1_20 =cP1_20.getCollision();
    int CP2_20 =cP2_20.getCollision();

    double sc1_5 =sC1_5.getProbe()/1000.0;
    double sc2_5 =sC2_5.getProbe()/1000.0;
    double dh1_5=dH1_5.getProbe()/1000.0;
    double dh2_5=dH2_5.getProbe()/1000.0;
    double cp1_5 =cP1_5.getProbe()/1000.0;
    double cp2_5 =cP2_5.getProbe()/1000.0;
    double sc1_10 =sC1_10.getProbe()/1000.0;
    double sc2_10 =sC2_10.getProbe()/1000.0;
    double dh1_10=dH1_10.getProbe()/1000.0;
    double dh2_10=dH2_10.getProbe()/1000.0;
    double cp1_10=cP1_10.getProbe()/1000.0;
    double cp2_10=cP2_10.getProbe()/1000.0;
    double sc1_20 =sC1_20.getProbe()/1000.0;
    double sc2_20 =sC2_20.getProbe()/1000.0;
    double dh1_20=dH1_20.getProbe()/1000.0;
    double dh2_20=dH2_20.getProbe()/1000.0;
    double cp1_20 =cP1_20.getProbe()/1000.0;
    double cp2_20 =cP2_20.getProbe()/1000.0;
    cout<<" ______________________________________________________________________________"<<endl;
    cout<<"|           |                |           Hash1        |          Hash2         |"<<endl;
    cout<<"|  Hash     | Collision      |________________________|________________________|"<<endl;
    cout<<"|  Table    | Resolution     |    # of    |  Average  |    # of    |  Average  |"<<endl;
    cout<<"|  Size     | Method         | Collisions |  Probes   | Collisions |  Probes   |"<<endl;
    cout<<"|___________|________________|____________|___________|____________|___________|"<<endl;
    cout<<"|           |                |            |           |            |           |"<<endl;
    cout<<"|  5000     | Chaining       |   "<<SC1_5<<"     |   "<<sc1_5<<"   |   "<<SC2_5<<"     |   "<<sc2_5<<"   |"<<endl;
    cout<<"|___________|________________|____________|___________|____________|___________|"<<endl;
    cout<<"|           |                |            |           |            |           |"<<endl;
    cout<<"|  5000     | Double Hasing  |   "<<DH1_5<<"    |   "<<dh1_5<<"   |   "<<DH2_5<<"    |   "<<dh2_5<<"   |"<<endl;
    cout<<"|___________|________________|____________|___________|____________|___________|"<<endl;
    cout<<"|           |                |            |           |            |           |"<<endl;
    cout<<"|  5000     | Custom Probing |   "<<CP1_5<<"    |   "<<cp1_5<<"   |   "<<CP2_5<<"    |   "<<cp2_5<<"   |"<<endl;
    cout<<"|___________|________________|____________|___________|____________|___________|"<<endl;
    cout<<endl;
    cout<<endl;
    cout<<" ______________________________________________________________________________"<<endl;
    cout<<"|           |                |           Hash1        |          Hash2         |"<<endl;
    cout<<"|  Hash     | Collision      |________________________|________________________|"<<endl;
    cout<<"|  Table    | Resolution     |    # of    |  Average  |    # of    |  Average  |"<<endl;
    cout<<"|  Size     | Method         | Collisions |  Probes   | Collisions |  Probes   |"<<endl;
    cout<<"|___________|________________|____________|___________|____________|___________|"<<endl;
    cout<<"|           |                |            |           |            |           |"<<endl;
    cout<<"|  10000    | Chaining       |   "<<SC1_10<<"     |   "<<sc1_10<<"   |   "<<SC2_10<<"     |   "<<sc2_10<<"   |"<<endl;
    cout<<"|___________|________________|____________|___________|____________|___________|"<<endl;
    cout<<"|           |                |            |           |            |           |"<<endl;
    cout<<"|  10000    | Double Hasing  |   "<<DH1_10<<"    |   "<<dh1_10<<"   |   "<<DH2_10<<"    |   "<<dh2_10<<"   |"<<endl;
    cout<<"|___________|________________|____________|___________|____________|___________|"<<endl;
    cout<<"|           |                |            |           |            |           |"<<endl;
    cout<<"|  10000    | Custom Probing |   "<<CP1_10<<"    |   "<<cp1_10<<"   |   "<<CP2_10<<"    |   "<<cp2_10<<"   |"<<endl;
    cout<<"|___________|________________|____________|___________|____________|___________|"<<endl;

    cout<<endl;
    cout<<endl;
    cout<<" ______________________________________________________________________________"<<endl;
    cout<<"|           |                |           Hash1        |          Hash2         |"<<endl;
    cout<<"|  Hash     | Collision      |________________________|________________________|"<<endl;
    cout<<"|  Table    | Resolution     |    # of    |  Average  |    # of    |  Average  |"<<endl;
    cout<<"|  Size     | Method         | Collisions |  Probes   | Collisions |  Probes   |"<<endl;
    cout<<"|___________|________________|____________|___________|____________|___________|"<<endl;
    cout<<"|           |                |            |           |            |           |"<<endl;
    cout<<"|  20000    | Chaining       |   "<<SC1_20<<"     |   "<<sc1_20<<"   |   "<<SC2_20<<"     |   "<<sc2_20<<"   |"<<endl;
    cout<<"|___________|________________|____________|___________|____________|___________|"<<endl;
    cout<<"|           |                |            |           |            |           |"<<endl;
    cout<<"|  20000    | Double Hasing  |   "<<DH1_20<<"     |   "<<dh1_20<<"   |   "<<DH2_20<<"     |   "<<dh2_20<<"   |"<<endl;
    cout<<"|___________|________________|____________|___________|____________|___________|"<<endl;
    cout<<"|           |                |            |           |            |           |"<<endl;
    cout<<"|  20000    | Custom Probing |   "<<CP1_20<<"     |   "<<cp1_20<<"   |   "<<CP2_20<<"     |   "<<cp2_20<<"   |"<<endl;
    cout<<"|___________|________________|____________|___________|____________|___________|"<<endl;
}