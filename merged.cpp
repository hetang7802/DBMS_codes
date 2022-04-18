#include <bits/stdc++.h>
using namespace std;

// ================================THE PAGE CLASS================================
class Page
{
    public:
        Page();
        Page(int cap);
        virtual ~Page();
        Page *right;
        Page *left;
        int numberOfRecords;
        vector<int> sizeOfRecords;
        int* records;
        vector<int> primaryKeys;
        int freeIndex;
        int capacity;
        void insert(int cap, int val);

    protected:

    private:
};

Page::Page(int cap)
{
    capacity = cap;
    numberOfRecords = 0;
    freeIndex = 0;
    right = nullptr;
    left = nullptr;
    records = new int[1+(cap/4)];
    //ctor
}

void Page::insert(int cap,int val){
    // when we called insert in the File class we already 
    // checked if this page is full or not so now directly insert
    // cout << "inserting " << val << endl;
    numberOfRecords++;
    sizeOfRecords.push_back(cap);
    primaryKeys.push_back(val);
    records[freeIndex] = val;
    freeIndex+=cap;
}

Page::~Page()
{
    //dtor
}

// ===============================THE FILE CLASS===========================
class File
{
    public:
//        File();
        File(int);
        virtual ~File();
        void addPage();
        void insertElement(int cap,int val);
        void searchElement(int);
        void showStatus();
        int noOfPages;

    protected:

    private:
        Page *first;
        Page *last;
        int pageCapacity;
};

File::File(int pageCap)
{
    first = nullptr;
    noOfPages = 0;
    pageCapacity = pageCap;
}

void File::addPage(){
    Page *p = new Page(pageCapacity);
    noOfPages++;
    if(first == nullptr){
        first = p;
        last = p;
        first->right = nullptr;
        first->left = nullptr;
    }else if(first == last){
        // cout << "adding page " << endl;
        last = p;
        last->left = first;
        first->right = last;
        last->right = nullptr;
        first->left = nullptr;
    }else{
        Page* tmp =last;
        last = p;
        last->right = nullptr;
        last->left = tmp;
        tmp->right = last;
    }
}

void File::insertElement(int cap,int val){
    Page *curr = first;
    while(curr!=nullptr){
        if(curr->freeIndex+cap+4 > curr->capacity-16-4*curr->numberOfRecords){
            curr = curr->right;
        }else{break;}
    }
    if(curr == nullptr){
        // no page has the space available to insert the given record
        addPage();
        curr = last;
    }
    curr->insert(cap, val);
}

void File::searchElement(int val){
    Page * curr = first;
    int pageNumber=0,slotId=0;
    bool found = false;
    while(curr!=nullptr && found == false){
        // cout << "inside while" << endl;
        int i=0,id=0;
        // cout << "free index is " << curr->freeIndex << endl;
        // for(i;i<=curr->numberOfRecords;i++){
        //     // cout << "inside for " << i << endl;
        //     // cout << i << " pk is " << curr->primaryKeys[i] << endl;
        //     if(curr->primaryKeys[i]==val){
        //         found = true;
        //         slotId = i;
        //         break;
        //     }
        // }
        
        for(i;i<=curr->freeIndex;i){
            if(curr->records[i]==val){
                found = true;
                slotId = id;
                break;
            }else{
                i+=curr->sizeOfRecords[slotId];
                id++;
            }
        }
        if(found==false){
            curr=curr->right;
            pageNumber++;
        }
    }
    if(found==false){
        cout << "-1 -1" << endl;
    }else{
        // add 1 to pagenumber and sltoId for 1 based index
        cout << pageNumber << " " << slotId << endl;
    }
}

void File::showStatus(){
    cout << noOfPages << " " ;
    Page *curr = first;
    while(curr!=nullptr){
        cout << curr->numberOfRecords << " " ;
        curr = curr->right;
    }
    cout << endl;
}

File::~File()
{
    //dtor
}

// =============================THE MAIN PROGRAM=====================
int main()
{
    int pageSize;
    cin >> pageSize;
    File *F = new File(pageSize);
    int opCode;
    while(cin>>opCode){
        switch (opCode)
        {
            case 1:{
                int recSize,primaryKey;
                cin >> recSize >> primaryKey;
                F->insertElement(recSize,primaryKey);
                break;
            }
            case 2:{
                F->showStatus();
                break;
            }
            case 3:{
                int val;
                cin >> val;
                F->searchElement(val);
                break;
            }
        
            default: break;
        }
    }

    return 0;
}

