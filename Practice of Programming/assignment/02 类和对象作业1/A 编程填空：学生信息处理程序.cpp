#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <cstdlib>
using namespace std;

class Student {
	//
private:
	int grade[4],age,id;
	double average;
	char name[20];
public:
	void input()
	{
		cin.getline(name, 20, ',');
		cin >> age;
		cin.get();
		cin >> id;
		cin.get();
		cin >> grade[0];
		cin.get();
		cin >> grade[1];
		cin.get();
		cin >> grade[2];
		cin.get();
		cin >> grade[3];
	}
	void calculate()
	{
		int sum = 0;
		for (int i = 0; i < 4; i++)
		{
			sum += grade[i];
		}
		average = sum / 4.0;
	}
	void output()
	{
		cout << name << ',' << age << ',' << id << ',' << average << endl;
	}
	//
};

int main() {
	Student student;        // 定义类的对象
	student.input();        // 输入数据
	student.calculate();    // 计算平均成绩
	student.output();       // 输出数据
}
