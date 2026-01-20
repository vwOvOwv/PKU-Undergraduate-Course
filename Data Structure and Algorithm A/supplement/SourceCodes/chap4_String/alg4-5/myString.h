#include <string.h> 
#include <assert.h>
using namespace std;
class String { 
private:
      char *str;
      int size;
public:
	String() {
		size = 0 ;  
		str = NULL;
	}
      String(char *s) {
         size = strlen(s) ;  
         str = new char [size +1];
         assert(str != '\0');       // 当开辟动态区域不成功时，运行异常退出
         strcpy(str, s);
      }
      String Substr(int pos,int n ){
           int i;
           int left = size - pos ; 					// 自下标pos向右计数到串尾查看剩余长度
           String tmp;
           char *p, *q;
    // 以下7行语句，全是为取出子串做准备工作
           if (pos >= size) 						// 若下标pos值超过本串实际串长，则返回空串
              return NULL;
           if (n > left ) 						// 若n超过自pos以右的子串长度，则把n变小
              n = left;
           delete [] tmp.str; 					// 释放原来的存储空间
           tmp.str = new char[n+1];
           assert(tmp.str != NULL) ;				// 若开辟动态存储空间失败，则退出
           p = tmp.str;  						//  p指向暂无内容的字符串
           q = &str[pos]; 						//  q指向本实例串str数组下标pos处
           for (i =0; i < n; i++)
               *p++ = *q++;  					// 将q所指的内容赋值给 p，并同时后移
           *p = '\0';  						// 循环结束后，让temp.str的结尾为'\0'
           tmp.size = n;
           return tmp;
    }
	friend ostream& operator << (ostream& os, const String& s);  
};

ostream& operator << (ostream& os, const String& s){ 
	os << s.str; 
	return os; 
}; 

