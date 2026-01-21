#include<iostream>
#include<vector>
#include<random>
#include<time.h>
using namespace std;

void modInsertSort(vector<int>&rec,int start,int delta){//基于delta的插入排序
    int n=rec.size();
    for(int i=start+delta;i<n;i+=delta){
        int tmp=rec[i],j;
        for(j=i-delta;j>=start;j-=delta){//向前找
            if(tmp<rec[j])
                rec[j+delta]=rec[j];
            else break;
        }
        rec[j+delta]=tmp;
    }
}

void shellSort(vector<int>&rec){
    int n=rec.size();
    for(int delta=n/2;delta>0;delta/=2){//增量每次除以2递减
        for(int i=0;i<delta;i++){//有delta个子序列
            modInsertSort(rec,i,delta);//对每个子序列进行插入排序
        }
    }
    // modInsertSort(rec,0,1);//若选取其他增量，则要加扫尾排序，因为可能漏掉了一些元素
}

int main(){
    vector<int>rec(10,0);
    srand(time(nullptr));
    for(int i=0;i<10;i++){
        rec[i]=rand()%10;
        cout<<rec[i]<<' ';
    }
    cout<<endl;
    shellSort(rec);
    for(int i=0;i<rec.size();i++)
        cout<<rec[i]<<' ';
    cout<<endl;
    return 0;
}