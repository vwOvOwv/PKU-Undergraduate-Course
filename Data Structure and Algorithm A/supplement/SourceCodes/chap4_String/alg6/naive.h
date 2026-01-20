#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

int NaiveStrMatching(string T, string P)  { 
    int i = 0;							// 模式的下标变量
    int j = 0;							// 目标的下标变量
    int pLen = P.length( );             		// 模式的长度
    int tLen = T.length( );					// 目标的长度

	if  (tLen < pLen) 				    // 如果目标比模式短，匹配无法成功
        return (-1);          			
    while ( i < pLen  &&  j < tLen)  {  		// 反复比较对应字符来开始匹配
   		if (T[j] == P[i]) 
			i++,  j++;
		else  { 
 			j = j - i + 1; 
			i = 0;
		}
	}
	if ( i >= pLen)
		return (j - pLen+1);
	else return (-1);
 }
