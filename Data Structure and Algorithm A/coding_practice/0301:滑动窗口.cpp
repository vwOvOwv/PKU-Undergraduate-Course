#include<iostream>
#include<deque>
#include<vector>
#include<algorithm>

using namespace std;

deque<int>maxi_to_mini,mini_to_maxi;
vector<int>nums;
vector<int>maxi,mini;
int main(){
    int n,k;
    cin>>n>>k;
    for(int i=0;i<k;i++){
        int num;
        cin>>num;
        nums.push_back(num);
        while(!maxi_to_mini.empty()&&nums[maxi_to_mini.back()]<num)
            maxi_to_mini.pop_back();
        maxi_to_mini.push_back(i);
        while(!mini_to_maxi.empty()&&nums[mini_to_maxi.back()]>num)
            mini_to_maxi.pop_back();
        mini_to_maxi.push_back(i);
    }
    maxi.push_back(nums[maxi_to_mini.front()]);
    mini.push_back(nums[mini_to_maxi.front()]);
    for(int i=k;i<n;i++){
        int num;
        cin>>num;
        nums.push_back(num);
        while(!maxi_to_mini.empty()&&maxi_to_mini.front()<=i-k)
            maxi_to_mini.pop_front();
        while(!maxi_to_mini.empty()&&nums[maxi_to_mini.back()]<num)
            maxi_to_mini.pop_back();
        maxi_to_mini.push_back(i);
        while(!mini_to_maxi.empty()&&mini_to_maxi.front()<=i-k)
            mini_to_maxi.pop_front();
        while(!mini_to_maxi.empty()&&nums[mini_to_maxi.back()]>num)
            mini_to_maxi.pop_back();
        mini_to_maxi.push_back(i);
        maxi.push_back(nums[maxi_to_mini.front()]);
        mini.push_back(nums[mini_to_maxi.front()]);
    }
    int len=maxi.size();
    for(int i=0;i<len;i++){
        cout<<mini[i];
        if(i<len-1)
            cout<<' ';
    }
    cout<<endl;
    for(int i=0;i<len;i++){
        cout<<maxi[i];
        if(i<len-1)
            cout<<' ';
    }
    cout<<endl;
    return 0;
}   