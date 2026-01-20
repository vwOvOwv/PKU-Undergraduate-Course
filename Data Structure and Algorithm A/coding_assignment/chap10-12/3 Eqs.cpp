#include<iostream>

using namespace std;

const int maxi=25000000;

short Hash[maxi+10];

int main(){
    int a1,a2,a3,a4,a5;
    cin>>a1>>a2>>a3>>a4>>a5;
    for(int x1=-50;x1<=50;x1++){
        if(x1==0)
            continue;
        int sum1=0;
        for(int x2=-50;x2<=50;x2++){
            if(x2==0)
                continue;
            for(int x3=-50;x3<=50;x3++){
                if(x3==0)
                    continue;
                //cout<<sum1<<endl;//
                sum1=a1*x1*x1*x1+a2*x2*x2*x2+a3*x3*x3*x3;
                if(sum1<0)
                    sum1+=maxi;
                //cout<<sum1<<endl;//
                Hash[sum1]++;
            }
        }
    }
    int cnt=0;
    for(int x4=-50;x4<=50;x4++){
        //cout<<"called"<<endl;
        if(x4==0)
            continue;
        int sum2=0;
        for(int x5=-50;x5<=50;x5++){
            if(x5==0)
                continue;
            sum2=-(a4*x4*x4*x4+a5*x5*x5*x5);
            if(sum2<0)
                sum2+=maxi;
            if(Hash[sum2])
                cnt+=Hash[sum2];//不是cnt++,同一个值可能在等式左侧出现多次
        }
    }
    cout<<cnt;
    return 0;
}