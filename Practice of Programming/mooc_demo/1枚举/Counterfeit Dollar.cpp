#include<iostream>
#include<cstring>
#include<string>
using namespace std;

string Left[3],Right[3],Result[3];

bool is_fake(char c,bool light){
	for(int i=0;i<3;i++){
			if(light==true){
				if(Left[i].find(c)!=-1){
					if(Result[i]!="down")
						return false;
				}
			else if(Right[i].find(c)!=-1){
					if(Result[i]!="up")
						return false;
				}
			else{
				if(Result[i]!="even")
					return false;
				}
			}
			else{
				if(Left[i].find(c)!=-1){
					if(Result[i]!="up")
						return false;
				}
			else if(Right[i].find(c)!=-1){
					if(Result[i]!="down")
						return false;
				}
			else{
				if(Result[i]!="even")
					return false;
				}
			}
		}
	return true;
}

int main()
{
	int n;
	cin>>n;
	while(n--){
	for(int i=0;i<3;i++){
		cin>>Left[i]>>Right[i]>>Result[i];
	}
	for(char i='A';i<='Z';i++){
		if(is_fake(i,true)){
			cout<<i<<" is the counterfeit coin and it is light."<<endl;
			break;
		}
		if(is_fake(i,false)){
			cout<<i<<" is the counterfeit coin and it is heavy."<<endl;
			break;
		}
	}
	}
	return 0;
}
