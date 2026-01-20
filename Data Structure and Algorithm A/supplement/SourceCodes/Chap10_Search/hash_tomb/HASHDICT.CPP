#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "Int.h"
#include "../COMPARE.H"

#include "HASHDICT.H"



// 检索关键码值为k的记录。假定每个关键码的探查序列中
//至少有一个槽是空的，否则它们就会进入一个无限循环中。
template <class Key, class Elem, class KEComp, class EEComp>
bool hashdict<Key, Elem, KEComp, EEComp>::
hashSearch(const Key& K, Elem& e) const {
    int home= h(K);      // home 保存K的基地址
    int i=0;    
    int pos = home;     // 探查序列的初始位置
    while(!EEComp::eq(EMPTY, HT[pos])){
	  if (KEComp::eq(K, HT[pos])) {  // 发现目标
          e = HT[pos];
         return true;
	  }
      i++;
	  pos = (home + p(K, i)) % M;
  }
   return false;    
}







// 将数据元素e插入到散列表 HT
template <class Key, class Elem, class KEComp, class EEComp>
bool hashdict<Key, Elem, KEComp, EEComp>::hashInsert(const Elem &e)
  {
    int home= h(getkey(e));        // home记录基位置
    int i=0;    
    int pos = home;               // 探查序列的初始位置
    while(!EEComp::eq(EMPTY, HT[pos])
		&& !EEComp::eq(TOMB, HT[pos])){
	  if (KEComp::eq(getkey(e), HT[pos]))
		  return false;           //不允许重复关键码
	  i++;
	  pos = (home + p(getkey(e), i)) % M;
  }
    HT[pos]=e;                    //插入e
    return true;
}


// 删除函数
template <class Key, class Elem, class KEComp, class EEComp>
Elem hashdict<Key,Elem,KEComp,EEComp>::hashDelete(const Key& K)
{
    int home= h(K);         // home记录基位置
    int i=0;    
    int pos = home;        // 探查序列的初始位置
    Elem temp;
    while(!EEComp::eq(EMPTY, HT[pos])){
	   if (KEComp::eq(K, HT[pos])){
		   temp = HT[pos];
          HT[pos] = TOMB; //设置墓碑
          return temp;    //返回目标
		}
	   i++;
	   pos = (home + p(K, i)) % M;
	}
    return EMPTY; 
}



  
int getkey(Int* e)
 {
	return e->key();
 }


char* getkey(char* e)
 { 
	return e;
 }



int Intkey(Int& e) 
{
	return e.key();
 }


char* charkey(char*& e) 
{
	return e;
 }

int main(int argc, char** argv) {
   hashdict<int, Int*, intIntsCompare, IntsIntsCompare> dict(100, new Int(-1),new Int(-2));
  Int* val;

  dict.hashInsert(new Int(10));
  if (dict.hashSearch(10, val))
    cout << "发现值 " << val << " 匹配关键码10\n";
  else
    cout << "没有发现匹配关键码10的元素\n";
  return 0;
}
