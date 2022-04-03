#include <bits/stdc++.h>

using namespace std;

// ===========================THE BUCKET CLASS================================
class Bucket
{
    public:
        Bucket();
        Bucket(int local_depth,int bucket_size);
        virtual ~Bucket();
        void display();
        int localDepth;
        int capacity;
        multiset<int> values;
        multiset<int> getValues();
        void incrementLocalDepth();
    protected:

    private:

};

Bucket::Bucket()
{
    //ctor
}

Bucket::Bucket(int local_depth, int bucket_size)
{
    localDepth = local_depth;
    capacity = bucket_size;
}

multiset<int> Bucket::getValues(){
    return values;
}

void Bucket::display(){
    for(auto i : this->getValues()){
        cout << i << " " ;
    }
    cout << endl;
}

void Bucket::incrementLocalDepth(){
    localDepth++;
}

Bucket::~Bucket()
{
    //dtor
}

// ===========================THE DIRECTORY CLASS================================

class Directory
{
    public:
        Directory();
        Directory(int,int);
        virtual ~Directory();
        int getGlobalDepth();
        void setGlobalDepth(int);
        int currBuckets;
        map<int,Bucket*> buckets;
        vector<Bucket*> vec;
        int calculateHash(int,int);
        void rearrangeBucketElements(Bucket);
        void insertElement(int);
        void displayBuckets();
        int handleFull(int);
        bool search_(int);
        void deleteElement(int);
        void displayVec();

    protected:

    private:
        int globalDepth;
        int bucketSize;
};

Directory::Directory()
{
    //ctor
}

Directory::Directory(int global_depth,int bucket_capacity)
{
    globalDepth=global_depth;
    bucketSize = bucket_capacity;
    for(int i=0;i<(1<<globalDepth);i++){
        Bucket * b = new Bucket(global_depth,bucket_capacity) ;
        buckets[i] = b;
        vec.push_back(b);
//        buckets.push_back(b);
//        cout << "the bucket " << i <<  " has cap " << buckets[i]->capacity << " " << buckets[i]->localDepth << endl;
    }
}

int Directory::getGlobalDepth(){
    return globalDepth;
}

void Directory::setGlobalDepth(int n){
    globalDepth=n;
}

int Directory::calculateHash(int val, int depth){
    return val&((1<<depth)-1);
}

void Directory::insertElement(int val){
    int hashValue = calculateHash(val,globalDepth);
    if(buckets[hashValue]->values.size()<bucketSize){
        buckets[hashValue]->values.insert(val);
        return;
    }

    // if the local depth is less than global depth
    if(buckets[hashValue]->localDepth < globalDepth){
        int local_depth = buckets[hashValue]->localDepth;
        Bucket *A = new Bucket(local_depth+1,bucketSize);
        buckets[hashValue]->incrementLocalDepth();
        for(auto i : buckets[hashValue]->getValues()){
            if(calculateHash(i,local_depth+1)!=hashValue){
                buckets[hashValue]->values.erase(i);
                A->values.insert(i);
            }
        }
        vec.push_back(A);
        for(int i=hashValue+(1<<(local_depth));i<(1<<globalDepth);i+=(1<<(local_depth+1))){
            buckets[i] = A;
        }
    }
    else{
        // doubling the directory
        globalDepth++;
        int newHash = hashValue+(1<<(globalDepth-1));
        for(int i=(1<<(globalDepth-1));i<(1<<globalDepth);i++){
            if(i!=newHash){
                buckets[i] = buckets[i%(1<<(globalDepth-1))];
            }
        }
        // split this bucket into two
        Bucket *nH = new Bucket(globalDepth,bucketSize);
        Bucket *hV = new Bucket(globalDepth,bucketSize);
        for(auto i : buckets[hashValue]->getValues()){
            if(calculateHash(i,globalDepth)!=hashValue){
                nH->values.insert(i);
            }else{
                hV->values.insert(i);
            }
        }
        vec[hashValue]=hV;
        vec.push_back(nH);
        buckets[hashValue]=hV;
        buckets[newHash]=nH;
    }
    insertElement(val);
}

void Directory::displayBuckets(){
    for(int i=0;i<(1<<globalDepth);i++){
        cout << i << " : " ;
        for(auto j : buckets[i]->getValues()){
            cout << j << " " ;
        }
        cout << " local depth " << buckets[i]->localDepth << endl;
    }
}

bool Directory::search_(int val){
    int hashValue=calculateHash(val,globalDepth);
    for(auto i : buckets[hashValue]->values){
        if(i==val){
            return true;
        }
    }
    return false;
}

void Directory::deleteElement(int val){
    if(search_(val)){
        int hashValue = calculateHash(val,globalDepth);
        buckets[hashValue]->values.erase(buckets[hashValue]->values.find(val));
    }
}

void Directory::displayVec(){
    cout << globalDepth << endl << vec.size() << endl;
    for(int i=0;i<vec.size();i++){
        cout << vec[i]->getValues().size() << " " << vec[i]->localDepth << endl;
//        cout << " local depth " << buckets[i]->localDepth << endl;
    }
}

Directory::~Directory()
{
    //dtor
}

// ===========================THE MAIN PROGRAM================================
int main()
{

    int globalDepth,bucket_cap;
    cin >> globalDepth >> bucket_cap;
    Directory dir(globalDepth,bucket_cap);

    int currOp;
    while(cin >> currOp){
        switch(currOp){
            int number;
            case 1 : {
                dir.displayBuckets();
            }
            case 2 : {
                cin >> number;
                dir.insertElement(number);
                break;
            }
            case 3: {
                cin >> number;
                if(dir.search_(number)){
                    // cout << number << " is present" << endl;
                }else{
                    // cout << number << " is absent" << endl;
                };
                break;
            }
            case 4: {
                cin >> number;
                dir.deleteElement(number);
                break;
            }
            case 5:{
//                dir.displayBuckets();
                dir.displayVec();
                break;
            }
            case 6: {
                return 0;
            }
        }
    }
    return 0;
}

