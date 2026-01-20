#include<iostream>
#include<vector>

using namespace std;

const int maxi=100003;

vector<vector<int> >lst[maxi+5];

int Hash(vector<int>rec){
    long long int sum=0;
    long long int multi=1;
    for(int i=0;i<6;i++){
        sum=(sum+rec[i])%maxi;
        multi=((multi*rec[i]))%maxi;
    }
    return (sum+multi)%maxi;
}

bool isIdentical(vector<int>a,vector<int>b){
    // int index1=0,index2=0,mem=0;//不能只找第一位！！！
    // while(index2<6&&a[index1]!=b[index2])
    //     index2++;
    // if(index2==6)
    //     return false;
    // mem=index2;
    // while(index1<6&&a[index1]==b[index2]){
    //     index1++;
    //     index2=(index2+1)%6;
    // }
    // if(index1==6)
    //     return true;
    // index1=0,index2=mem;
    // while(index1<6&&a[index1]==b[index2]){
    //     index1++;
    //     index2--;
    //     if(index2==-1)
    //         index2=5;
    // }
    // if(index1==6)
    //     return true;
    // return false;

    for(int i=0;i<6;i++){
        for(int j=0;j<6;j++){
            bool flag=true;
            for(int k=0;k<6;k++){
                if(a[(i+k)%6]!=b[(j+k)%6]){
                    flag=false;
                    break;
                }
            }
            if(flag)
               return true;

            flag=true;
            for(int k=0;k<6;k++){
                if(a[(i+k)%6]!=b[(j-k+6)%6]){
                    flag=false;
                    break;
                }
            }
            if(flag)
               return true;
        }
    }
    return false;
}

int main(){
    int n;
    cin>>n;
    vector<int>rec(6);
    for(int i=0;i<n;i++){
        for(int j=0;j<6;j++){
            cin>>rec[j];
        }
        int val=Hash(rec);
        int size=lst[val].size();
        for(int j=0;j<size;j++){
            if(isIdentical(rec,lst[val][j])){
                cout<<"Twin snowflakes found."<<endl;
                return 0;
            }
        }
        lst[val].push_back(rec);
    }
    cout<<"No two snowflakes are alike."<<endl;
    return 0;
}