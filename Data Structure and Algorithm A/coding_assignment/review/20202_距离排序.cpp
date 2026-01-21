#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<cmath>
#include<iomanip>

using namespace std;

class Point{
public:
    int x,y,z;
    Point(){}
    Point(int xx,int yy,int zz):x(xx),y(yy),z(zz){}
};

class Node{
public:
    Point a,b;
    int index1,index2;
    double dis;
    Node(){}
    Node(Point aa,Point bb,int i1,int i2,double d):
        a(aa),b(bb),index1(i1),index2(i2),dis(d){}
};

bool Compare(Node a,Node b){
    if(a.dis>b.dis)
        return true;
    else if(a.dis<b.dis)
        return false;
    else{
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
    vector<Point>rec(n);
    for(int i=0;i<n;i++){
        cin>>rec[i].x>>rec[i].y>>rec[i].z;
    }
    vector<Node>ans;
    for(int i=0;i<n-1;i++){
        for(int j=i+1;j<n;j++){
            int dx=rec[i].x-rec[j].x;
            int dy=rec[i].y-rec[j].y;
            int dz=rec[i].z-rec[j].z;
            double dis=sqrt(dx*dx+dy*dy+dz*dz);
            ans.push_back(Node(rec[i],rec[j],i,j,dis));
        }
    }
    sort(ans.begin(),ans.end(),Compare);
    for(auto i:ans){
        cout<<'('<<i.a.x<<','<<i.a.y<<','<<i.a.z<<')';
        cout<<'-';
        cout<<'('<<i.b.x<<','<<i.b.y<<','<<i.b.z<<')';
        cout<<'='<<fixed<<setprecision(2)<<i.dis<<endl;
    }
}