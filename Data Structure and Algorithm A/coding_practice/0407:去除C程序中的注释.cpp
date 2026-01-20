#include<iostream>
#include<vector>
#include<algorithm>
#include<string>
#include<unordered_map>
#include<set>
#include<map>

using namespace std;

int main(){
    string ans;
    char c,before='\0';
    bool isInString=false,isInNote=false;
    while((c=cin.get())!=EOF){
        if(c=='\"'){
            if(!isInString&&!isInNote){
                isInString=true;
                ans.push_back(c);
            }
            else if(isInString&&before!='\\'&&!isInNote){
                isInString=false;
                ans.push_back(c);
            }
            else if(!isInNote)
                ans.push_back(c);
        }
        else if(c=='*'){
            if(!isInString&&!isInNote&&before=='/'){
                ans.pop_back();
                isInNote=true;
            }
            else if(!isInNote)
                ans.push_back(c);
        }
        else if(c=='/'){
            if(!isInString&&isInNote&&before=='*'){
                isInNote=false;
            }
            else if(!isInNote)
                ans.push_back(c);
        }
        else if(!isInNote)
            ans.push_back(c);
        before=c;
    }
    cout<<ans;
}