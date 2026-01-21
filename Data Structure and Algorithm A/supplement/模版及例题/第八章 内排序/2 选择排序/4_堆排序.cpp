#include<vector>
#include<iostream>
#include<random>
#include<time.h>
#include<queue>

using namespace std;

priority_queue<int,vector<int> >q;//最大堆，考虑成在带排序数组中直接建了堆，因此不能用最小堆（不好保持堆结构）

void heapSort(vector<int>&rec){
    int n=rec.size();
    for(int i=0;i<n;i++)
        q.push(rec[i]);
    for(int i=n-1;i>=0;i--){
        rec[i]=q.top();//对应deleteMax操作，直接把堆顶元素挪到数组末尾，这样可以保持堆结构
        q.pop();//相当于让堆的大小-1
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
    heapSort(rec);
    for(int i=0;i<10;i++)
        cout<<rec[i]<<' ';
    cout<<endl;
    return 0;
}