#include <iostream>
#include<vector>
#include<cmath>
#include<iomanip>
#include<algorithm>

using namespace std;

class Node{//点类
public:
    int x,y,z;
    Node(){}
    Node(int xx,int yy,int zz):x(xx),y(yy),z(zz){}
};

class ansNode{//答案结点
public:
    Node point1,point2;
    int index1,index2;//定位在输入数组中的位置
    double dis;
    ansNode(){}
    ansNode(Node p1,Node p2,double d,int i1,int i2):point1(p1),point2(p2),dis(d),index1(i1),index2(i2){}
};

bool Compare(ansNode a,ansNode b){//比较函数
    if(a.dis>b.dis)//距离优先
        return true;
    else if(a.dis<b.dis)
        return false;
    else{//距离相等时比较在输入中的位置
        if(a.index1<b.index1)
            return true;
        else if(a.index1>b.index1)
            return false;
        else{
            if(a.index2<b.index2)
                return true;
            else
                return false;
        }
    }
}

int main(){
    int n;
    cin>>n;
    vector<Node>rec(n);
    vector<ansNode>ans;
    vector<vector<double> >dis(n,vector<double>(n,0));
    for(int i=0;i<n;i++){
        cin>>rec[i].x>>rec[i].y>>rec[i].z;
    }
    for(int i=0;i<n;i++){
        for(int j=i+1;j<n;j++){
            int x1=rec[i].x,y1=rec[i].y,z1=rec[i].z;
            int x2=rec[j].x,y2=rec[j].y,z2=rec[j].z;
            int dx=x1-x2,dy=y1-y2,dz=z1-z2;
            dis[i][j]=sqrt(dx*dx+dy*dy+dz*dz);
            ans.push_back(ansNode(rec[i],rec[j],dis[i][j],i,j));//保证两个点的先后顺序与输入一致
        }
    }
    sort(ans.begin(),ans.end(),Compare);
    int size=ans.size();
    for(int i=0;i<size;i++){
        cout<<'('<<ans[i].point1.x<<','<<ans[i].point1.y<<','<<ans[i].point1.z<<')';
        cout<<'-';
        cout<<'('<<ans[i].point2.x<<','<<ans[i].point2.y<<','<<ans[i].point2.z<<')';
        cout<<'=';
        cout<<fixed<<setprecision(2)<<ans[i].dis;
        cout<<endl;
    }
    return 0;
}