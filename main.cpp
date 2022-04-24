#include <bits/stdc++.h>

using namespace std;
class Page
{
    public :
        vector<int> rec;
};


int main()
{
    int b,recPerPage,totalRec;
    cin >> b >> recPerPage >> totalRec;
    int totalPages = ceil((double)totalRec/(double)recPerPage);
    int records[totalRec];
    Page *file[totalPages];
    for(int i=0;i<totalRec;i++){
        cin >> records[i];
    }
    int curr = 0;
    for(int i=0;i<totalPages-1;i++){
        file[i] = new Page;
        for(int j=curr;j<curr+recPerPage;j++){
            file[i]->rec.push_back(records[j]);
        }
        // sort the records internally
        // one loop iteration corresponds to one time loading data in buffer during pass 0
//        sort(file[i]->rec.begin(),file[i]->rec.end());
        curr+=recPerPage;
    }
    file[totalPages-1] = new Page;
    for(int i=curr;i<totalRec;i++){
        file[totalPages-1]->rec.push_back(records[i]);
    }
//    for(int i=0;i<totalPages;i++){
//        cout << "page number " << i << endl;
//        for(auto j : file[i]->rec){
//            cout << j << " ";
//        }
//        cout << endl;
//    }
//    sort(file[totalPages-1]->rec.begin(),file[totalPages-1]->rec.end());
    Page *runs[(int)ceil((double)totalPages/(double)b)];
//    cout << "total pages " << totalPages << endl;
    // PASS 0
    for(int i=0;i<ceil((double)totalPages/(double)b);i++){
        runs[i] = new Page;
        for(int j=i*b;j<(i+1)*b;j++){
            if(j>=totalPages)break;
            for(auto r : file[j]->rec){
                runs[i]->rec.push_back(r);
            }
        }
        sort(runs[i]->rec.begin(),runs[i]->rec.end());
    }
//    for(int i=0;i<ceil((double)totalPages/(double)b);i++){
//        cout << "run number " << i << endl;
//        for(auto r : runs[i]->rec){
//            cout << r << " " ;
//        }
//        cout << endl;
//    }

    int last = 0,totalRuns = ceil((double)totalPages/(double)b);
//    while(totalRuns>1){
//        // load B-1 pages into memory
//        Page *newRun[(int)ceil((double)totalRuns/(double)b)];
//        for(int i=0;i<ceil((double)totalRuns/(double)b);i++){
//            Page *p = new Page;
//            for(int j=i*b;j<(i+1)*b;j++){
//                if(j>=totalRuns)break;
//                for(auto r : runs[j]->rec){
//                    p->rec.push_back(r);
//                    if(p->rec.size()==recPerPage){
//                        newRun[i]=p;
//                        break;
//                    }
//                }
//            }
//            sort(p->rec.begin(),p->rec.end());
//            newRun[i]=p;
//        }
////        delete runs;
//        totalRuns = ceil((double)totalRuns/(double)b);
//        Page *runs[totalRuns];
//        for(int i=0;i<totalRuns;i++){
//            runs[i] = newRun[i];
//        }
//        delete newRun;
//    }
    int CURR=1;
    while(totalRuns>1){
//        cout << "total runs " << totalRuns << " " << b << endl;
        Page *newRun[(int)ceil((double)totalRuns/(double)(b-1))];
        // load B-1 runs into memory and insert into newRun
//        cout << "before insertion " << p->rec.size() ;
        for(int i=0;i<ceil((double)totalRuns/(double)(b-1));i++){
            Page *p = new Page;
//            cout << "iteration " << i << endl;
            for(int j=i*(b-1);j<(i+1)*(b-1);j++){
                if(j>=totalRuns){
//                        cout << "shoulbe break" << endl;
                        break;
                }
                for(auto r : runs[j]->rec){
//                    cout << "inserting " << r << " " << CURR++<< endl;
                    p->rec.push_back(r);
                }
                runs[j]=nullptr;
            }
//            cout << "here " << endl;
            newRun[i] = p;
//            delete p;
//            cout << "before " << newRun[i]->rec.size() << endl;
            sort(newRun[i]->rec.begin(),newRun[i]->rec.end());
//            cout << "after" << endl;
        }
//        fill(runs,runs+totalRuns,nullptr);
        totalRuns = ceil((double)totalRuns/(double)(b-1));
        for(int i=0;i<totalRuns;i++){
            runs[i] = newRun[i];
        }
    }
//    cout << "quit " << endl;
//    cout << "size " << runs[0]->rec.size() << endl;
    for(auto i : runs[0]->rec){
        cout << i << " ";
    }
    cout << endl;
    return 0;
}


