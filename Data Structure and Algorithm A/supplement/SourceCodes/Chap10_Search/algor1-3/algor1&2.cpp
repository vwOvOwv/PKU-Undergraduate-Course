#include "iostream"


#include <vector>
#include "time.h"
using namespace std;



template  <class Type>  class Item {
	public: 
		Item(Type value):key(value) {}
        Type getKey() {
			return key;
		}         //获取关键码值;
        void setKey(Type k){
			key=k;
		}
	private:
		Type  key;                           //关键码域
		//...
		//其它域;                              //其它域
};

 



 template  <class Type>  int SeqSearch (vector<Item<Type>*>& datavector,int length, Type k)  {
    int i=length;
   datavector[0]->setKey (k);        //将第0个元素设为待检索值
  while(datavector[i]->getKey()!=k)
	  i--;     //从后往前逐个比较
    return i;           //返回元素位置
}


 template  <class Type>  int SimpleSearch (vector<Item<Type>*>& datavector,int length, Type k)  {
    int i=length;
  
  while(datavector[i]->getKey()!=k&&i>0)
	  i--;     //从后往前逐个比较
    return i;           //返回元素位置
}


template  <class Type>  int BinSearch (vector<Item<Type>*>& datavector,int length,Type k)  {
   int low=1,high=length,mid;   //low, high分别记录数组首尾位置
   while (low<=high){
	   mid=(low+high)/2;
	   if (k<datavector[mid]->getKey()) 
		   high = mid-1;               //右缩检索区间
	   else if (k>datavector[mid]->getKey())
		   low = mid+1;           //左缩检索区间
       else
		   return mid;                            //检索成功，返回元素位置
   } 
	   return 0;                                 //检索失败，返回0
}


// 时间变量与时间函数
clock_t tstart = 0;  


void Settime()
  { 
	tstart = clock(); 
}


double Gettime()
  {
	return (double)((double)clock() - (double)tstart)/(double)CLOCKS_PER_SEC; 
}




int main()
{ 
vector<Item<int>*> datavector(200001);
 	
Item<int>* darray[200001];

  for(int j=0;j<200001;j++)
  darray[j]=new Item<int>(j);
          
  vector<Item<int>*>::iterator outputIterator = datavector.begin();
  copy(darray, darray +200001, outputIterator);
 
  

	int k;




    Settime();//开始计时
      k= SeqSearch(datavector, datavector.size()-1,7);
    
	
	cout << "-->监视哨顺序检索耗时 : " ;
	cout << Gettime()<< endl ;
    


 
   Settime();//开始计时
      k= SimpleSearch(datavector, datavector.size()-1,7);
	cout << "-->一般顺序检索耗时 : " ;
   cout << Gettime()<< endl ;
  

    
return 0;   
}
       