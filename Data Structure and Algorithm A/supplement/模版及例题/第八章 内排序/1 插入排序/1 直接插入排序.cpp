#include<iostream>
#include<vector>
#include<random>
#include<time.h>
using namespace std;

void insertSort(vector<int>&rec){
    int n=rec.size();
    for(int i=1;i<n;i++){
        int tmp=rec[i];
        int j=i-1;//向前找，前面的元素都已经有序了
        //也可以向前找相邻逆序对，找到了就交换
        while(j>=0&&tmp<rec[j]){//找tmp应该在的位置
            rec[j+1]=rec[j];//腾位置
            j--;
        }
        rec[j+1]=tmp;//多减了一个，加回来
    }
}

int main(){
    vector<int>rec(10,0);
    srand(time(nullptr));
    for(int i=0;i<10;i++){
        rec[i]=rand()%10;
        cout<<rec[i]<<' ';
    }
    cout<<endl;
    insertSort(rec);
    for(int i=0;i<rec.size();i++)
        cout<<rec[i]<<' ';
    cout<<endl;
    return 0;
}