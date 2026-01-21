#include<iostream>
#include<vector>
#include<random>
#include<time.h>

using namespace std;

class Node{
public:
    int key;
    int next;
    Node(int k,int n):key(k),next(n){}
    Node(){}
};

class staticQueue{
public:
    int head,tail;
};

void distribute(vector<Node>&rec,vector<staticQueue>&qRec,int first,int cnt,int r){//将元素分配进不同的静态队列中
    for(int j=0;j<r;j++)//初始化队列
        qRec[j].head=-1;
    int cur=first;//当前元素下标
    while(cur!=-1){
        int k=rec[cur].key;
        for(int a=0;a<cnt;a++)
            k/=r;
        k%=r;
        //加入队列
        if(qRec[k].head==-1)
            qRec[k].head=cur;
        else
            rec[qRec[k].tail].next=cur;
        qRec[k].tail=cur;
        //找下一个
        cur=rec[cur].next;
    }
}

void collect(vector<Node>&rec,vector<staticQueue>&qRec,int&first,int r){//将队列按顺序连接起来
    int last;
    int k=0;
    while(qRec[k].head==-1)
        k++;
    first=qRec[k].head;//最后一次收集使first会指向值最小的元素
    last=qRec[k].tail;
    while(k<r-1){//到r-1就是已经连接到最后一个队列了
        k++;
        while(k<r-1&&qRec[k].head==-1)
            k++;
        if(qRec[k].head!=-1){
            rec[last].next=qRec[k].head;
            last=qRec[k].tail;
        }
    }
    rec[last].next=-1;//注意补上结尾
}

void adjustRec(vector<Node>&rec,int first){//最后调整数组使之有序
    int n=rec.size();
    int j=first;
    for(int i=0;i<n-1;i++){
        Node tmp=rec[j];//rec[j]是当前第i小的元素，先取出来
        rec[j]=rec[i];//把rec的第一个位置让出来
        rec[i]=tmp;//第i小的元素归位
        rec[i].next=j;//保留调换轨迹
        j=tmp.next;//j移到下一位，tmp的作用体现
        while(j<=i)//是轨迹
            j=rec[j].next;
    }
}

void radixSort(vector<Node>&rec,int d,int r){//d为排序码分成的段数，r是基数（类似于maxi）
    int n=rec.size();
    for(int i=0;i<n-1;i++)//初始化next指针，便于第一次分配
        rec[i].next=i+1;
    rec[n-1].next=-1;//注意补上结尾
    vector<staticQueue>qRec(r);
    int first=0;
    for(int i=0;i<d;i++){
        distribute(rec,qRec,first,i,r);
        collect(rec,qRec,first,r);
    }
    adjustRec(rec,first);
}

int main(){
    vector<Node>rec(10);
    srand(time(nullptr));
    for(int i=0;i<10;i++){
        rec[i].key=rand()%100+(rand()%9+1)*100;
        cout<<rec[i].key<<' ';
    }
    cout<<endl;
    radixSort(rec,3,10);
    for(int i=0;i<10;i++)
        cout<<rec[i].key<<' ';
    cout<<endl;
    return 0;
}