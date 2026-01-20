#include<iostream>
#include<cstring>
#include<iomanip>
using namespace std;

class Headquarters;
class Sodier
{
public:
	Sodier(int n , int m );
	~Sodier();
private:
	int strength;//武士生命值
	int attack;//武士攻击力
	friend Headquarters;
};
Sodier::Sodier(int n = 0, int m = 0)
{
	strength = n;
	attack = m;
}
Sodier::~Sodier()
{
}

class Headquarters
{
public:
	Headquarters(int n);
	~Headquarters();
	void get_order(Sodier a, Sodier b, Sodier c, Sodier d, Sodier e);//获取武士生成顺序
	void get_name_order(string a, string b, string c, string d, string e);
	void create(int &turn, int r,bool color,bool &judge);//武士生成
	int turn = 0;
	bool judge = true;
private:
	int blood;//司令部生命元
	int cnt=1;//武士编号计数
	Sodier order[5];//武士生成顺序
	string name_order[5];
	int count[5] = { 1,1,1,1,1 };//每种武士个数

};
Headquarters::Headquarters(int n=0)
{
	blood = n;
}
Headquarters::~Headquarters()
{
}
void Headquarters::get_order(Sodier a, Sodier b, Sodier c, Sodier d, Sodier e) {
	order[0] = a; order[1] = b; order[2] = c; order[3] = d; order[4] = e;
}
void Headquarters::get_name_order(string a, string b, string c, string d, string e) {
	name_order[0] = a; name_order[1] = b; name_order[2] = c; name_order[3] = d; name_order[4] = e;
}
void Headquarters::create(int &turn, int r,bool color,bool &judge){
	if (judge == false)
		return;
	cout << setw(3) << setfill('0') << r << ' ';
	int flag = 0, i;
	for (i = turn; i <= 4; i++)
	{
		if (blood >= order[i].strength)
		{
			if (color == true)
				cout << "red " << name_order[i] <<' '<< cnt << " born with strength " << order[i].strength << ',' << count[i] << ' ' << name_order[i] << " in red headquarter" << endl;
			else
				cout << "blue " << name_order[i] << ' '<<cnt << " born with strength " << order[i].strength << ',' << count[i] << ' ' << name_order[i] << " in blue headquarter" << endl;
			blood -= order[i].strength;
			turn = (i + 1) % 5;
			cnt++; count[i]++;
			break;
		}
		if ((i == turn && flag == 1)||blood==0) {
			if (color == true)
				cout << "red headquarter stops making warriors" << endl;
			if (color == false)
				cout << "blue headquarter stops making warriors" << endl;
			judge = false;
			break;
		}
		if (i == 4)
		{
			flag = 1;
			i = -1;
			continue;
		}
	}
}

int t;int M;int s1, s2, s3, s4, s5;
int main()
{
	cin >> t;
	for(int i=1;i<=t;i++)
	{
		cout << "Case:" << i << endl;
		cin >> M;
		Headquarters Red(M), Blue(M);
		cin >> s1 >> s2 >> s3 >> s4 >> s5;
		Sodier dragon(s1), ninja(s2), iceman(s3), lion(s4), wolf(s5);
		Red.get_order(iceman, lion, wolf, ninja, dragon);
		Blue.get_order(lion, dragon, ninja, iceman, wolf);
		Red.get_name_order("iceman", "lion", "wolf", "ninja", "dragon");
		Blue.get_name_order("lion", "dragon", "ninja", "iceman", "wolf");
		for (int r = 0;r<=999; r++)
		{
			if (Red.judge == true)
				Red.create(Red.turn, r, true, Red.judge);
			if (Blue.judge == true)
				Blue.create(Blue.turn, r, false, Blue.judge);
			if (Red.judge == false && Blue.judge == false)
				break;
		}
	}
	return 0;
}
