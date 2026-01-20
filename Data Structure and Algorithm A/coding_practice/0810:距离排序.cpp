#include<iostream>
#include<vector>
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
            int x1=rec[i].x,y1=rec[i].y,z1=rec[i].z;
            int x2=rec[j].x,y2=rec[j].y,z2=rec[j].z;
            int dx=x1-x2,dy=y1-y2,dz=z1-z2;
            double dis=sqrt(dx*dx+dy*dy+dz*dz);
            ans.push_back(Node(rec[i],rec[j],i,j,dis));
        }
    }
    sort(ans.begin(),ans.end(),Compare);
    int size=ans.size();
    for(int i=0;i<size;i++){
        cout<<'('<<ans[i].a.x<<','<<ans[i].a.y<<','<<ans[i].a.z<<')';
        cout<<'-';
        cout<<'('<<ans[i].b.x<<','<<ans[i].b.y<<','<<ans[i].b.z<<')';
        cout<<'=';
        cout<<fixed<<setprecision(2)<<ans[i].dis<<endl;
    }
}