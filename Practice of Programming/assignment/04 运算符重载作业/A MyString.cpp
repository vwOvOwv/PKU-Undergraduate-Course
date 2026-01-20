#include <iostream>
#include <string>
#include <cstring>
using namespace std;
class MyString {
	char * p;
public:
	MyString(const char * s) {
		if( s) {
			p = new char[strlen(s) + 1];
			strcpy(p,s);
		}
		else
			p = NULL;

	}
	~MyString() { if(p) delete [] p; }
	//
	MyString(MyString& s) {
		if (s.p) {
			p = new char[strlen(s.p) + 1];
			strcpy(p, s.p);
		}
		else
			p = NULL;
	}
	void Copy(const char* w) {
		if (w) {
			p = new char[strlen(w) + 1];
			strcpy(p, w);
		}
		else
			p = NULL;
	}
	friend ostream& operator<<(ostream& o, MyString& s) {
		o << s.p;
		return o;
	}
	MyString& operator=(const char* w) {
		if (w) {
			p = new char[strlen(w) + 1];
			strcpy(p, w);
		}
		else
			p = NULL;
		return *this;
	}
	MyString& operator=(MyString& s) {
		if (s.p) {
			p = new char[strlen(s.p) + 1];
			strcpy(p, s.p);
		}
		else
			p = NULL;
		return *this;
	}
	//
};
int main()
{
	char w1[200],w2[100];
	while( cin >> w1 >> w2) {
		MyString s1(w1),s2 = s1;
		MyString s3(NULL);
		s3.Copy(w1);
		cout << s1 << "," << s2 << "," << s3 << endl;

		s2 = w2;
		s3 = s2;
		s1 = s3;
		cout << s1 << "," << s2 << "," << s3 << endl;

	}
}
