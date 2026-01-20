#include <iostream>

#include "arrStack.h"

// Class Declaration 类的说明
class Calculator  { 
private:
      arrStack<double> s;		// 这个栈用于压入保存操作数
      bool GetTwoOperands(double& opd1, double& opd2); 	// 从栈顶弹出两个操作数opd1和opd2
      void Compute(char op);  	// 调用GetTwoOperands，并按op运算对两个操作数进行计算
  public:
//       Calculator(){} ;		// 创建计算器实例，开辟一个空栈
      void Run();		    // 后缀表达式的读入，在遇到符号"="时 ，启动求值计算 
      void Clear();   		// 计算器的清除，为随后的下一次计算做准备  
};

// 计算器类class Calculator中部分成员函数的程序实现
bool Calculator::GetTwoOperands(double& opd1, double& opd2)  { 
   	if (s.isEmpty()) {
    		cerr << "Missing operand!" <<endl;
    		return false;
   	}
     s.pop(&opd1); 							// 右操作数
   	if (s.isEmpty())  {
    		cerr << "Missing operand!" <<endl;
    		return false;
   	}
   	s.pop(&opd2)	;						// 左操作数
   	return true;
}

void Calculator::Compute(char op)  { 
  	bool result;
  	double operand1, operand2;
  	result = GetTwoOperands(operand1, operand2);
  	if (result == true)
    		switch(op)  {
        		case '+' : 	s.push(operand2 + operand1);
                   	break;
        		case '-' : 	s.push(operand2 - operand1);
                   	break;
        		case '*' : 	s.push(operand2 * operand1);
                   	break;
        		case '/' :	if (operand1 == 0.0)  {
                   		cerr << "Divide by 0!" << endl;
                   		s.clear();
                   	}
                   	else
                   		s.push(operand2 / operand1);
                   	break;
    		}
  	else
      	s.clear();
}

void Calculator::Run(void)  { 
 	char c;
 	double newOperand, res;
 	while (cin >> c, c != '=')  {     
    		switch(c)  {
        		case '+' :                         
        		case '-' :
        		case '*' :
        		case '/' :
                    Compute(c);           
                    break;
        		default :                         
                    cin.putback(c);         
                    cin >> newOperand;     
                    s.push(newOperand);     
                    break;
    		}
  	}
 	if (!s.isEmpty()) {
		s.pop(&res);
        cout << res << endl; 					// 印出求值的最后结果
   }		
}

