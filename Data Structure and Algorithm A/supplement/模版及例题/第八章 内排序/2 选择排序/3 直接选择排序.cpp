#include<iostream>
#include<vector>
#include<random>
#include<time.h>

using namespace std;

void selectSort(vector<int>&rec){
    int n=rec.size();
    for(int i=0;i<n;i++){//每次选出、排好第i小的元素
        int miniPos=i;//最小位置初始化
        for(int j=i+1;j<n;j++){//前面的都找好了，只需向后找
            if(rec[j]<rec[miniPos]){
                miniPos=j;
            }
        }
        swap(rec[i],rec[miniPos]);
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
    selectSort(rec);
    for(int i=0;i<10;i++)
        cout<<rec[i]<<' ';
    cout<<endl;
    return 0;
}