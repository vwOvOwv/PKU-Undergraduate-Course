#include<iostream>
#include<algorithm>

using namespace std;

int x,y,cnt;
int n,m,w,l;
int dir;

int main(){
    cin>>n>>m;
    l=w=n;
    cnt=1,x=y=1,dir=0;
    int cntx,cnty;
    cntx=cnty=1;
    while(cnt!=m){
        switch (dir%4)
        {
        case 0:
            y++;cnty++;
            if(cnty==w){dir++;l--;cntx=0;}
            break;
        case 1:
            x++;cntx++;
            if(cntx==l){dir++;w--;cnty=0;}
            break;
        case 2:
            y--;cnty++;
            if(cnty==w){dir++;l--;cntx=0;}
            break;
        case 3:
            x--;cntx++;
            if(cntx==l){dir++;w--;cnty=0;}
            break;
        }
        cnt++;
    }
    cout<<x<<' '<<y<<endl;
    return 0;
}