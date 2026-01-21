#include <cstdlib>
#include <iostream>
using namespace std;
int strlen(const char * s)
{	int i = 0;
	for(; s[i]; ++i);
	return i;
}
void strcpy(char * d,const char * s)
{
	int i = 0;
	for( i = 0; s[i]; ++i)
		d[i] = s[i];
	d[i] = 0;

}
int strcmp(const char * s1,const char * s2)
{
	for(int i = 0; s1[i] && s2[i] ; ++i) {
		if( s1[i] < s2[i] )
			return -1;
		else if( s1[i] > s2[i])
			return 1;
	}
	return 0;
}
void strcat(char * d,const char * s)
{
	int len = strlen(d);
	strcpy(d+len,s);
}
class MyString
{
	//
private:
	char str[100];
	int size;
public:
	MyString(const char s[100]) {
		strcpy(str, s);
		size = strlen(s);
	}
	MyString(){
		size = 0;
		str[0] = '\0';
	}
	MyString(const MyString& a) {
		for (int i = 0; i < a.size; i++) {
			str[i] = a.str[i];
		}
		size = a.size;
		str[size] = '\0';
	}
	friend ostream& operator<<(ostream& o, MyString& b) {
		if (b.str)
			o << b.str;
		return o;
	}
	MyString& operator=(MyString b) {
		for (int i = 0; i < b.size; i++) {
			str[i] = b.str[i];
		}
		size = b.size;
		str[size] = '\0';
		return *this;
	}
	MyString& operator=(const char s[100]) {
		for (int i = 0; i < strlen(s); i++) {
			str[i] = s[i];
		}
		size = strlen(s);
		str[size] = '\0';
		return *this;
	}
	MyString operator+(MyString& b) {
		MyString tmp(*this);
		strcat(tmp.str, b.str);
		tmp.size += b.size;
		return tmp;
	}
	friend MyString operator+(const char s[100], MyString &b) {
		MyString tmp(s);
		strcat(tmp.str, b.str);
		tmp.size += b.size;
		return tmp;
	}
	MyString operator+(const char s[100]) {
		strcat(str, s);
		size += strlen(s);
		return*this;
	}
	char& operator[](int n) {
		return str[n];
	}
	MyString& operator+=(const char s[100]) {
		strcat(str, s);
		size += strlen(s);
		return *this;
	}
	char* operator()(int start, int length) {
		char *p=str;
		int j = 0;
		for (int i = start; i < start+length; i++) {
			p[j] = str[i];
			j++;
		}
		p[j] = '\0';
		return p;
	}
	friend int operator<(MyString s1,MyString s2) {
		if (strcmp(s1.str, s2.str) == -1) {
			return 1;
		}
		else
			return 0;
	}
	friend int operator>(MyString s1, MyString s2) {
		if (strcmp(s1.str, s2.str) == 1) {
			return 1;
		}
		else
			return 0;
	}
	friend int operator==(MyString s1, MyString s2) {
		if (s1.size != s2.size)
			return 0;
		for (int i = 0; s1[i] && s2[i]; ++i) {
			if (s1[i] != s2[i])
				return 0;
		}
		return 1;
	}
	//
};


int CompareString( const void * e1, const void * e2)
{
	MyString * s1 = (MyString * ) e1;
	MyString * s2 = (MyString * ) e2;
	if( * s1 < *s2 )
	return -1;
	else if( *s1 == *s2)
	return 0;
	else if( *s1 > *s2 )
	return 1;
}
int main()
{
	MyString s1("abcd-"),s2,s3("efgh-"),s4(s1);
	MyString SArray[4] = {"big","me","about","take"};
	cout << "1. " << s1 << s2 << s3<< s4<< endl;
	s4 = s3;
	s3 = s1 + s3;
	cout << "2. " << s1 << endl;
	cout << "3. " << s2 << endl;
	cout << "4. " << s3 << endl;
	cout << "5. " << s4 << endl;
	cout << "6. " << s1[2] << endl;
	s2 = s1;
	s1 = "ijkl-";
	s1[2] = 'A' ;
	cout << "7. " << s2 << endl;
	cout << "8. " << s1 << endl;
	s1 += "mnop";
	cout << "9. " << s1 << endl;
	s4 = "qrst-" + s2;
	cout << "10. " << s4 << endl;
	s1 = s2 + s4 + " uvw " + "xyz";
	cout << "11. " << s1 << endl;
	qsort(SArray,4,sizeof(MyString),CompareString);
	for( int i = 0;i < 4;i ++ )
	cout << SArray[i] << endl;
	//s1的从下标0开始长度为4的子串
	cout << s1(0,4) << endl;
	//s1的从下标5开始长度为10的子串
	cout << s1(5,10) << endl;
	return 0;
}
