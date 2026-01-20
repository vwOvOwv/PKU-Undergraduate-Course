#include<iostream>
using namespace std;
const int maxi=50001;
int t;
long long int length[maxi];//预处理，length[k]记录12345...k的长度
int main(){
    length[1]=1;
    for(int i=2;i<=maxi-1;i++){
        int cnt=0,tmp=i;
        while(tmp!=0){
            cnt++;
            tmp/=10;
        }
        length[i]=length[i-1]+cnt;
    }
    cin>>t;
    while(t--){
        int i;
        cin>>i;
        int k=1;
        for(k=1;k<=maxi-1;k++){//锁定第i位所在的123...k的k
            i-=length[k];
            if(i<=0){//注意是<=0
                i+=length[k];
                break;
            }
        }
        for(int j=1;j<=k;j++){//锁定i在123...k中的哪一个具体数字
            int tmp=j;
            int cnt=0;
            while(tmp!=0){//j的位数
                cnt++;
                tmp/=10;
            }
            i-=cnt;
            if(i<=0){//注意是<=0
                i+=cnt;
                for(int r=0;r<cnt-i;r++){//注意要i与j位数的关系
                    j/=10;
                }
                cout<<j%10<<endl;
                break;
            }
        }
    }
    return 0;
}