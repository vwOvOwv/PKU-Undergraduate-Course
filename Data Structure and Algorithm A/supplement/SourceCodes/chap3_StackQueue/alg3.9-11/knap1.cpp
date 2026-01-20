
#include <iostream>

#include "arrStack.h"

using namespace std;

bool knap(int s, int n);
bool nonRecKnap(int s, int n);
bool nonRecKnapOpt(int s, int n);

enum rdType {a, b, c}; 

class knapNode {
      friend  bool nonRecKnap(int s, int n);
      friend  bool nonRecKnapOpt(int s, int n);
       int 		s, n;  	      // 背包的承重量和物品的数目
       rdType	rd; 		  // 返回地址
       bool		k;		      // 结果单元	
}; 
// 引入两个与栈中结点类型相同的变量tmp和x作为进出栈的缓冲：
class knapNode 	tmp, x;

// 定义一个栈变量：
//Stack<knapNode> st(20); 


int w[5] = {2,4,4,5,7};

int main() {
    int s;
    
    cin >> s; 
    
    cout << "recursion" << endl; 
    if (knap(s, 5))  
        cout << "Have solution" << endl;
    else  cout << "No solution" << endl;
       
       
    cout << "non recursion" << endl;  
    if (nonRecKnap(s, 5))  
        cout << "Have solution" << endl;
    else  cout << "No solution" << endl;
   
    cout << "non recursion optimization" << endl;     
    if (nonRecKnapOpt(s, 5))  
        cout << "Have solution" << endl;
    else  cout << "No solution" << endl;

cin >> s; 
   
    return 0;
} 


//递归版的背包问题 
bool knap(int s, int n) {
		if (s == 0)  
			return true;
		if ((s < 0)||(s>0 && n <1))
			return false;
 		if (knap(s-w[n-1], n-1)) {
			cout << w[n-1];
			return true;
		} 
		else knap(s, n-1); 
}
// 非递归的机械版 
bool nonRecKnap(int s, int n) {
     arrStack<knapNode> st(20);  
		tmp.s = s,  tmp.n = n,  tmp.rd = a; 	// 非递归调用入口
		st.push(tmp);
  label0:								 // 递归调用入口
		st.pop(&tmp); 
		if (tmp.s == 0 ) { 	
			tmp.k = true;				// 修改栈顶的结果单元k
			st.push(tmp);
            goto label3;
		}
        if ((tmp.s < 0) || (tmp.s > 0 && tmp.n < 1)) {
			tmp.k = false;				// 修改栈顶的结果单元k
			st.push(tmp);
            goto label3;
		}
		st.push(tmp);
        x.s = tmp.s-w[tmp.n-1]; 			// 按照规则1进行压栈处理
		x.n = tmp.n -1;
		x.rd = b;
		st.push(x);
		goto label0;
	label1: 							// 规则1对应的返回处理
		st.pop(&x);					// 查看栈顶，根据其内容分情况处理
        if (tmp.k == true) {			// 若某层的结果单元为true
			x.k = true;					// 把true结果上传给调用方
			st.push(x);
			cout << w[x.n-1]<<endl;		// 并输出对应的物品
			goto label3;
		}
		st.push(x);					// 若某层的结果单元为false
        tmp.s = x.s;					// 当前物品的选择不合适，回溯，调用规则2
		tmp.n = x.n - 1;					// 按照规则2进行压栈处理
		tmp.rd = c;
		st.push(tmp);
		goto label0;
  	 label2: 							// 规则2对应的返回处理
		st.pop(&x);
		x.k = tmp.k;					// 结果单元k的内容上传给调用方
		st.push(x);
     label3:							// 递归出口处理
		st.pop(&tmp);
		switch (tmp.rd) {
			case a:  	return tmp.k; 		// 算法结束并返回结果
			case b:	goto label1;		// 转向规则1的返回处理处
		 	case c:	goto label2;		// 转向规则2的返回处理处
		}
}	 

// 非递归的优化版 
bool nonRecKnapOpt(int s, int n) {
	int 	t, n0 = n;
    bool 	k = false;
    arrStack<knapNode> st(20);  
    
	tmp.s = s, tmp.rd = a; 
	st.push(tmp);

	while (!st.isEmpty()) {
		t = st.top;
		st.getTop(&tmp);
		while ((tmp.s >= 0) && (tmp.s <= 0 || n0 > t)){  //处理栈顶元素，以判断是否满足递归出口条件
              if (tmp.s == 0 ) { 
				k = true;
				break;
			  } else {						// 尚未递达归出口前，按规则1进行压栈操作
				x.s = tmp.s - w[n0 - 1 - t];
				x.rd = b;
				st.push(x);
			  }
			  t = st.top;
			  st.getTop(&tmp);
		}
		while (!st.isEmpty()) {				// 返回处理			
              st.pop(&tmp);
              t = st.top;
              if (tmp.rd == 0) 					// 算法结束
                 return k; 
              if (tmp.rd == 1) 					// 从规则1返回
		         if (k == true) 				// 结果为真则打印对应的物品
                       cout << w[n0 - 1 - t]<<endl;	
                 else {						// 否则回溯，采用规则2进栈
                      st.getTop(&x);
                      tmp.s = x.s;
                      tmp.rd = c;
                      st.push(tmp);
                      break;
				}
		}
	}
}	 


