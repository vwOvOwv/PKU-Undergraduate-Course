#include<iostream>
#include<deque>
#include<algorithm>
#include<vector>

using namespace std;

deque<int>maxi,mini;
vector<int>rec_maxi,rec_mini,nums;
int n,k;
int main(){//双端单调队列模拟滑动窗口
    cin>>n>>k;
    for(int i=0;i<k;i++){//前k个元素要先形成窗口
        int num;
        cin>>num;
        nums.push_back(num);
        while(!maxi.empty()&&nums[maxi.back()]<num){
            maxi.pop_back();
        }
        maxi.push_back(i);
        while(!mini.empty()&&nums[mini.back()]>num){
            mini.pop_back();
        }
        mini.push_back(i);
        
    }
    rec_maxi.push_back(maxi.front());
    rec_mini.push_back(mini.front());
    for(int i=k;i<n;i++){
        int num;
        cin>>num;
        nums.push_back(num);
        while(!maxi.empty()&&maxi.front()<=i-k){//先把窗口以外的元素去掉,入队列的顺序必然导致下标单调递增，因此从前端弹出即可
            maxi.pop_front();
        }
        while(!maxi.empty()&&nums[maxi.back()]<num){
            maxi.pop_back();
        }
        maxi.push_back(i);
        rec_maxi.push_back(maxi.front());
        while(!mini.empty()&&mini.front()<=i-k){
            mini.pop_front();
        }
        while(!mini.empty()&&nums[mini.back()]>num){
            mini.pop_back();
        }
        mini.push_back(i);
        rec_mini.push_back(mini.front());
    }
    for(int i=0;i<n-k+1;i++){
        cout<<nums[rec_mini[i]];
        if(i!=n-1){
            cout<<' ';
        }
    }
    cout<<endl;
    for(int i=0;i<n-k+1;i++){
        cout<<nums[rec_maxi[i]];
        if(i!=n-1){
            cout<<' ';
        }
    }
    return 0;
}