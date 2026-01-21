#include<iostream>
#include<vector>
using namespace std;

vector<vector<int> >Graph(5,vector<int>(5));//图

class Dist{//边的信息
public:
	int length;
	int pre;
};

//思路：对邻接矩阵进行n次迭代，每次都用结点i做中间结点更新最小距离信息
void Floyd(int n){
    vector<vector<Dist> >distance(5,vector<Dist>(5));
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(i==j){
                distance[i][j].length=0;
                distance[i][j].pre=i;
            }
            else{
                distance[i][j].length=9999;
                distance[i][j].pre=-1;
            }
        }
    }
    //初始化距离
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(Graph[i][j]){
                distance[i][j].length=Graph[i][j];
                distance[i][j].pre=i;
            }
        }
    }
    for(int v=0;v<n;v++){
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                if(distance[i][j].length>distance[i][v].length+distance[v][j].length){
                    distance[i][j].length=distance[i][v].length+distance[v][j].length;
                    distance[i][j].pre=distance[v][j].pre;//注意前驱更新的细节
                    // distance[i][j].pre=v; 这个也可以，但是不利于恢复路径
                    //用v更新意味着认为j前就是v，即v->j，但实际上，distance[v][j]对应的路径可能是v->...->j
                }
            }
        }
    }

    cout<<"Matrix of Distance"<<endl;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            cout<<distance[i][j].length<<"     ";
        }
        cout<<endl;
    }

    cout<<"Matrix of Pre"<<endl;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            cout<<distance[i][j].pre<<"     ";
        }
        cout<<endl;
    }
}

int main(){
    //  {0, 10,  0, 30, 100},
    //  {0,  0, 50,  0,  0}, 
    //  {0,  0,  0,  0, 10}, 
    //  {0, 10, 20,  0, 60}, 
    //  {0,  0,  0,  0,  0} 
	// Graph[0][1]=Graph[4][1]=Graph[2][4]=10;
	// Graph[3][2]=20;
	// Graph[0][3]=30;
	// Graph[1][2]=50;
	// Graph[3][4]=60;
	// Graph[0][4]=100;

    Graph[0][1]=4,Graph[0][2]=11;
    Graph[1][0]=6,Graph[1][2]=2;
    Graph[2][0]=3;
    Floyd(3);
    return 0;
}