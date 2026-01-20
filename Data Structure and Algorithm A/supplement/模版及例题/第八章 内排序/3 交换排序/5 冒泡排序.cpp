#include<iostream>
#include<vector>
#include<random>
#include<time.h>

using namespace std;

void bubbleSort(vector<int>&rec){
    bool noSwap=true;//noSwap标记是重要的优化
    int n=rec.size();
    for(int i=0;i<n-1;i++){//每次找出第i小的
        for(int j=n-1;j>i;j--){
            if(rec[j]<rec[j-1]){
                swap(rec[j],rec[j-1]);
                noSwap=false;
            }
        }
        if(noSwap)
            return;
    }
}

int main(){
    vector<int>rec(10);
    srand(time(nullptr));
    for(int i=0;i<10;i++){
        rec[i]=rand()%10;
        cout<<rec[i]<<' ';
    }
    cout<<endl;
    bubbleSort(rec);
    for(int i=0;i<10;i++)
        cout<<rec[i]<<' ';
    cout<<endl;
    return 0;
}