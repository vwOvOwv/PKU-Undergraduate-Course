#include<iostream>
#include<vector>
#include<algorithm>
#include<set>
using namespace std;



set<pair<int,int> >rec;
vector<pair<int,int> >v;
int n;
int main(){
    while(cin>>n&&n){
        rec.clear();
        v.clear();
        pair<int,int>tmp;
        for(int i=0;i<n;i++){
            cin>>tmp.first>>tmp.second;
            rec.insert(tmp);
            v.push_back(tmp);
        }
        int cnt=0;
        for(int i1=0;i1<n;i1++){
            for(int i2=i1+1;i2<n;i2++){
                pair<int,int> p1=v[i1],p2=v[i2];
                int deltax=p2.first-p1.first,deltay=p1.second-p2.second;
                pair<int,int> p3,p4;
                p3.first=p2.first-deltay,p3.second=p2.second-deltax;
                p4.first=p1.first-deltay,p4.second=p1.second-deltax;
                if(rec.count(p3)&&rec.count(p4))
                    cnt++;
                p3.first=p2.first+deltay,p3.second=p2.second+deltax;
                p4.first=p1.first+deltay,p4.second=p1.second+deltax;
                if(rec.count(p3)&&rec.count(p4))
                    cnt++;
            }
        }
        cout<<cnt/4<<endl;
    }
    return 0;
}