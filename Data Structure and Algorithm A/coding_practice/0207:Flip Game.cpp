#include<iostream>
#include<algorithm>
#include<climits>

using namespace std;

void SetBit(int &u,int v,int w){
    if(w){
        u|=(1<<v);
    }
    else{
        u&=~(1<<v);
    }
}
int Count1(int u){
    int cnt=0;
    for(int i=0;i<4;i++){
        if((u>>i)&1)
            cnt++;
    }
    return cnt;
}
int GetBit(int u,int v){
    return (u>>v)&1;
}
void FlipBit(int& u,int v){
    u^=(1<<v);
}
int plate[4];
int main(){
    char c;
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            cin>>c;
            if(c=='w'){
                SetBit(plate[i],j,1);
            }
        }
    }
    // for(int i=0;i<4;i++)
    //     cout<<plate[i]<<endl;
    int mini=INT_MAX;
    for(int n=0;n<16;n++){
        int tmp[4]={plate[0],plate[1],plate[2],plate[3]};
        int switches=n;
        int cnt=Count1(switches);
        // cout<<cnt;
        for(int i=0;i<4;i++){
            tmp[i]^=switches;
            for(int j=0;j<4;j++){
                if(GetBit(switches,j)){
                    if(j>0)
                        FlipBit(tmp[i],j-1);
                    if(j<3)
                        FlipBit(tmp[i],j+1);
                    if(i>0)
                        FlipBit(tmp[i-1],j);
                    if(i<3)
                        FlipBit(tmp[i+1],j);
                }
            }
            switches=tmp[i];//不要写再在上面的循环里了！！！
                cnt+=Count1(switches);
                if(i==3&&tmp[i]==0){
                    mini=min(cnt,mini);
                }
        }
    }
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            FlipBit(plate[i],j);
        }
    }
    for(int n=0;n<16;n++){
        int tmp[4]={plate[0],plate[1],plate[2],plate[3]};
        int switches=n;
        int cnt=Count1(switches);
        for(int i=0;i<4;i++){
            tmp[i]^=switches;
            for(int j=0;j<4;j++){
                if(GetBit(switches,j)){
                    if(j>0)
                        FlipBit(tmp[i],j-1);
                    if(j<3)
                        FlipBit(tmp[i],j+1);
                    if(i>0)
                        FlipBit(tmp[i-1],j);
                    if(i<3)
                        FlipBit(tmp[i+1],j);
                }
            }
            switches=tmp[i];
                cnt+=Count1(switches);
                if(i==3&&tmp[i]==0){
                    // for(int k=0;k<4;k++)
                    //     cout<<tmp[k]<<endl;
                    mini=min(cnt,mini);
                }
        }
    }
    if(mini!=INT_MAX){
        cout<<mini;
    }
    else{
        cout<<"Impossible";
    }
    return 0;
}