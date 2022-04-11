#include <bits/stdc++.h>

using namespace std;

// ====================THE NODE CLASS===================

class Node
{
    public:
        Node();
        Node(int cap,bool isD);
        virtual ~Node();
        int capacity;
        bool isData;
        vector<int> values;
        vector<Node*> pointers;
        Node * parent;


    protected:

    private:
};

Node::Node()
{
    //ctor
}

Node::Node(int cap, bool isD)
{
    capacity = cap;
    isData = isD;
    parent = nullptr;
    if(isData){
        for(int i=0;i<=capacity;i++){
            pointers.push_back(nullptr);
        }
    }
}

Node::~Node()
{
    //dtor
}



// =========================THE B PLUS TREE CLASS =================
class bPlus_tree
{
    public:
        bPlus_tree();
        bPlus_tree(int data_cap,int ind_cap);
        virtual ~bPlus_tree();
        int dataCap;
        int indCap;
        void insertElement(int val);
        void insertAssist(Node *);
        void displayTree();
        Node *root;
        void inOrderTraversal();
        void inOrderTraversalRec(Node*);
        void showStatus();
        void countNodes(Node *);

    protected:

    private:
        int dataNodes,indexNodes;
};


bPlus_tree::bPlus_tree()
{
    //ctor
    root = nullptr;
}

bPlus_tree::bPlus_tree(int data_cap, int ind_cap)
{
    //ctor
    dataCap = 2*data_cap;
    indCap = 2*ind_cap+1;
    root = nullptr;
}

void bPlus_tree::insertAssist(Node *n){
    // cout << "assist " << n->capacity << " " << n->values.size() << endl;
    if(n->capacity >= n->values.size())return;
    // cout << "node full " << n->values.size() << " " << n->isData << " " << dataCap <<  endl;
    int shiftIndex = n->capacity/2;
//    if(n->isData==false)shiftIndex++;
//    cout << "shift index is " << shiftIndex << endl;
    Node * par = n->parent;
    Node *left ;
    Node *right;
    if(n->isData){
        left = new Node(dataCap,true);
        right= new Node(dataCap,true);
    }
    else{
        left = new Node(indCap,false);
        right= new Node(indCap,false);
    }
    if(par==nullptr){
        par = new Node(indCap,false);
        // shift the value to parent node
        par->values.push_back(n->values[shiftIndex]);
        // assign values and pointers to the right and left children
        for(int i=0;i<=shiftIndex;i++){
            if(i<shiftIndex)left->values.push_back(n->values[i]);
            if(!n->isData){
                left->pointers.push_back(n->pointers[i]);
                n->pointers[i]->parent=left;
            }
        }
        for(int i=shiftIndex;i<=n->capacity+1;i++){
            if(n->isData==false && i==shiftIndex)continue;
            if(i<=n->capacity)right->values.push_back(n->values[i]);
            if(!n->isData){
                right->pointers.push_back(n->pointers[i]);
                n->pointers[i]->parent=right;
            }
        }
        // delete the node as we won't need it now
        delete n;
        // assign parent of left and right nodes
        left->parent = par;
        right->parent = par;
        par->pointers.push_back(left);
        par->pointers.push_back(right);
        // cout << "assigning root " << endl;
        // assign parent as the root
        root = par;
    }else{
        par = n->parent;
        int index = upper_bound(par->values.begin(),par->values.end(),n->values[shiftIndex])-par->values.begin();
        // cout << "index is " << index << " " << par->values.size() << endl;
        // for(auto i : par->values){
        //     cout << i << " " ;
        // }
        // cout << endl;
        par->values.push_back(0);
        // shift the values in parent node
        for(int i=n->values.size()-1;i>index;i--){
            par->values[i] = par->values[i-1];
        }

        // pushing the index element to the parent
        par->values[index] = n->values[shiftIndex];
        // split the node n into two nodes left and right
        for(int i=0;i<=shiftIndex;i++){
            if(i<shiftIndex)left->values.push_back(n->values[i]);
            if(!n->isData){
                left->pointers.push_back(n->pointers[i]);
                n->pointers[i]->parent=left;
            }
        }
        for(int i=shiftIndex;i<=n->capacity+1;i++){
            // no need to transfer the index node to right if it is not part of data
            // if(n->isData==false && i==shiftIndex)continue;
            // right->values.push_back(n->values[i]);
            if(n->isData==false && i==shiftIndex)continue;
            if(i<=n->capacity)right->values.push_back(n->values[i]);
            if(!n->isData){
                right->pointers.push_back(n->pointers[i]);
                n->pointers[i]->parent=right;
            }
        }
        // cout << "left and right have " << left->values.size() << " " << right->values.size() << endl;
        // delete the node as we won't need it now
        delete n;
        left->parent = par;
        right->parent = par;
        // rearrange the pointers
        par->pointers.push_back(nullptr);
        for(int i=par->pointers.size()-1;i>index+1;i--){
            par->pointers[i] = par->pointers[i-1];
        }
        par->pointers[index]=left;
        par->pointers[index+1]=right;
        // check the conditions for parent node
        insertAssist(par);
    }
}


void bPlus_tree::insertElement(int val){
    if(root == nullptr){
        Node * ptr = new Node(dataCap,true);
        ptr->values.push_back(val);
        root = ptr;
        return;
    }

    Node * curr = root;
    int index;
    // traverse down the tree as long as the curr node is not a datanode
    while(curr!=nullptr){
        if(curr->isData)break;
        index = upper_bound(curr->values.begin(),curr->values.end(),val)-curr->values.begin();
        curr = curr->pointers[index];
//        if(index == curr->values.size()){
//            curr = curr->pointers[index+1];
//        }else{
//        }
    }
    index = upper_bound(curr->values.begin(),curr->values.end(),val)-curr->values.begin();
    if(index == curr->values.size()){
        curr->values.push_back(val);
    }else{
        curr->values.push_back(0);
        for(int i=curr->values.size()-1;i>index;i--){
            curr->values[i] = curr->values[i-1];
        }
        curr->values[index] = val;
    }
    // check if there is overflow in the the current node
    insertAssist(curr);
}

void bPlus_tree::inOrderTraversalRec(Node * n){
    if(n==nullptr)return;
    for(int i=0;i<n->pointers.size();i++){
        if(n->pointers.size()!=0)inOrderTraversalRec(n->pointers[i]);
        if(i<n->values.size())cout << " " << n->values[i] << " " ;
    }
}

void bPlus_tree::inOrderTraversal(){
    cout << "inorder traversal:" ;


    inOrderTraversalRec(root);
    cout << endl;

}

void bPlus_tree::countNodes(Node * n){
    if(n==nullptr)return;
    if(n->isData){
        dataNodes++;
    }else{
        indexNodes++;
    }
    if(n->pointers.size()!=0){
        for(int i=0;i<n->pointers.size();i++){
            countNodes(n->pointers[i]);
        }
    }
}

void bPlus_tree::showStatus(){
    dataNodes=0;
    indexNodes=0;
    countNodes(root);
    cout << indexNodes << " " << dataNodes << " " ;
    for(int i=0;i<root->values.size();i++){
        cout << root->values[i] << " " ;
    }
    cout << endl;
    // inOrderTraversal();
}

bPlus_tree::~bPlus_tree()
{
    //dtor
}


// ===================== THE MAIN PROGRAM ===================

int main()
{
    int d,t;
    cin >> d >> t;
    bPlus_tree tree(d,t);
    int opCode;
    while(cin >> opCode){
        switch(opCode){
            case 1 : {
                int n;
                cin >> n;
                tree.insertElement(n);
                break;
            }
            case 2 : {
                tree.showStatus();
                break;
            }
            case 3 : {
                return 0;
            }
        }
    }
    return 0;
}
