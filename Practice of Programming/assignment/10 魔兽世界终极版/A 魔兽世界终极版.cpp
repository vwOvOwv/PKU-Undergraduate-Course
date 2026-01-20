/*思路梳理：
* 在世界内建立城市和司令部作为游戏进行的沙盘
* 司令部制造武士，武士拥有武器
* 司令部通过自身将武士传入沙盘，从而建立起城市和武士之间的联系，从而可以通过遍历城市操作所有武士及其拥有的武器
* 之后对各个过程进行模拟，即可实现魔兽世界
*/
#include<iostream>
#include<vector>
#include<string>
#include<climits>
#include<iomanip>
#include<algorithm>
#include<stdio.h>
#include<string>
using namespace std;

class Warrior;
class Weapon;
class Headquarter;
class World;
class City;
class Dragon;
class Ninja;
class Iceman;
class Wolf;
class Lion;

//魔兽世界 包含司令部与城市
class World {
public:
	Headquarter* Red;
	Headquarter* Blue;
	vector<City*>myCity;
	void run(int);//游戏运行
	void lion_escape(int);//5分钟判断各城市的狮子是否逃跑
	void warrior_march(int);//10分钟所有武士前进
	void city_add_element(int);//20分钟各城市产生生命元
	void get_city_element(int);//30分钟时司令部收取各城市生命元
	void arrow_attack(int);//35分钟判断各城市武士是否放箭
	void warrior_judge_bomb(int);//38分钟判断各城市武士是否用炸弹
	void warrior_battle(int);//40分钟的战斗过程，分为战斗前夕，战斗中和战斗结束后
	void lion_update_init_blood();//战斗开始前各个城市的狮子更新初始血量并记录
	void battle_on(int t);//各城市战斗过程
	void after_battle(int t);//战斗结束后收尾工作
	void clear_up();//战斗结束后清理战场
	void report_element(int);//50分钟司令部报告生命元数量
	void warrior_report_weapon(int);//55分钟各城市武士报告武器情况
	World(Headquarter* _Red, Headquarter* _Blue) :Red(_Red), Blue(_Blue) {}
};
//城市 储存战士与战斗情况以及产出的生命元
class City {
public:
	string win;//记录该城市本场战斗的胜负情况
	int ele;//记录生命元
	string flag;//旗帜颜色
	int cnt_red_win;//红方连胜计数
	int cnt_blue_win;//蓝方连胜计数
	Warrior* red;//该城市的红方武士
	Warrior* blue;//该城市的蓝方武士
	City() {
		win = '\0';//未发生战斗记为空
		red = nullptr;
		blue = nullptr;//无武士记为空
		cnt_red_win = 0;
		cnt_blue_win = 0;
		flag = '\0';//没有旗帜记为空
		ele = 0;
	}
};
//司令部 可以生成武士
class Headquarter {
public:
	Headquarter* enemy_Headquarter;//敌方司令部
	City* position;//在世界中的位置，通过该指针将生成的武士传入世界
	int tmp_element;//暂时储存武士通过战斗为司令部夺到的生命元(先奖励武士后收取生命元)
	int element;//当下的生命元
	int now_id;//已生成战士计数
	int cityNo;//所在位置编号
	string color;//阵营颜色
	bool conquered;//记录是否被占领
	Headquarter(int _element,string _color,int _cityNo):
		element(_element),color(_color),cityNo(_cityNo),now_id(0),conquered(false),tmp_element(0),position(nullptr),enemy_Headquarter(nullptr){}
	void warrior_born(int);//生成武士
};
//战士类 作为各类武士的基类
class Warrior{
public:
	vector<Weapon*>weapons;//武器池
	Headquarter*part;//所属阵营
	string name;//名称，以此判断武士类型
	int id;//编号
	int cityNo;//所在城市位置
	int blood;//血量
	int force;//攻击力
	void get_weapon();//根据编号和属性得到武器
	virtual void march();//前进一步，iceman有特殊的前进方式
	virtual void born_report();//被司令部生成后报告基本信息，不同类型武士报告内容不同
	void sort_weapon();//把自己的武器按字典序排好序，便于输出武器情况
	void attack(Warrior*, int);//主动攻击敌人
	void fight_back(Warrior*, int);//反击
	void win(Warrior*,int);//战胜对方后相关事宜处理
	void killed(int);//被杀死后输出情况
	void even();//和敌人打平后相关事宜处理
	void judge_yell(int t);//判断是否是dragon以及是否需要欢呼
	Warrior(int _blood, int _force, Headquarter* _part, string _name):blood(_blood), force(_force), part(_part), id(part->now_id), cityNo(part->cityNo), name(_name) {
		weapons.push_back(nullptr);//初始化为一个空指针，避免出现vector为空迭代器越界错误
	}
};
class Dragon :public Warrior {
public:
	static int mem_blood;//输入的dragon血量
	static int mem_force;//输入的dragon攻击力
	double morale;//士气
	Dragon(int _blood,int _force,Headquarter*part,string _name):Warrior(_blood, _force, part,_name){
		part->element -= blood;//生成时司令部生命元减少
		morale = double(part->element) / blood;//要用变化后的司令部生命元
	}
	void yell(int);//欢呼
	virtual void born_report();//dragon被生成时需要报告士气初始值
};
class Ninja :public Warrior {
public:
	static int mem_blood;//输入的ninja血量
	static int mem_force;//输入的ninja攻击力
	Ninja(int _blood, int _force, Headquarter* part,string _name) :Warrior(_blood, _force, part,_name) {
		part->element -= blood;//生成时司令部生命元减少
	}
};
class Iceman :public Warrior {
public:
	static int mem_blood;//输入的iceman血量
	static int mem_force;//输入的iceman攻击力
	int cnt_march;//前进步数记录
	Iceman(int _blood, int _force, Headquarter* part,string _name) :Warrior(_blood, _force, part, _name),cnt_march(0) {
		part->element -= blood;
	}
	virtual void march();//iceman前进过程中血量与攻击力可能改变
};
class Lion :public Warrior {
public:
	static int mem_blood;//输入的lion血量
	static int mem_force;//输入的lion攻击力
	static int lion_k;//输入的lion忠诚度下降值
	int loyalty;//忠诚度
	int init_blood;//每小时战斗开始前初始血量记录
	Lion(int _blood, int _force, Headquarter* part,string _name) :Warrior(_blood, _force, part,_name),init_blood(_blood){
		part->element -= _blood;
		loyalty = part->element;//忠诚度等于变化后的司令部生命元
	}
	void update_init_blood();//更新初始血量
	void blood_trans(Warrior*);//若战死则把自己的初始血量转移给敌人
	virtual void born_report();//被生成时要报告初始忠诚度
};
class Wolf :public Warrior {
public:
	static int mem_blood;//输入的wolf血量
	static int mem_force;//输入的wolf攻击力
	Wolf(int _blood, int _force, Headquarter* part,string _name) :Warrior(_blood, _force, part,_name) {
		part->element -= blood;
	}
	void get_enemy_weapon(Warrior*);//战胜之后要缴获敌人武器
};
//武器类 作为各种武器的基类
class Weapon {
public:
	string name;//名称，以此判断武器类型
	Warrior* master;//拥有者
	int weapon_force;//武器攻击力
	Weapon(int k, Warrior* _master, string _name) :weapon_force(k), master(_master), name(_name) {}
	virtual void attack(Warrior*, int);//攻击敌人，不同武器攻击方式不同
	virtual void lost();//丢弃武器
};
class Arrow :public Weapon {
public:
	static int arrow_r;//输入的arrow的攻击力
	int use_cnt = 0;//使用次数计数
	virtual void attack(Warrior*, int);//攻击敌人
	Arrow(Warrior*_master,string _name):Weapon(arrow_r,_master,_name){}
};
class Bomb :public Weapon {
public:
	virtual void attack(Warrior*,int);//攻击敌人
	Bomb(Warrior*_master,string _name):Weapon(INT_MAX,_master,_name){}//Bomb无攻击力的概念，可以杀死双方，故初始化攻击力为无限大
};
class Sword:public Weapon {
public:
	Sword(Warrior* _master, string _name):Weapon(_master->force*2/10,_master,_name) {}
};

int Dragon::mem_blood = 0;
int Dragon::mem_force = 0;
int Ninja::mem_blood = 0;
int Ninja::mem_force = 0;
int Iceman::mem_blood = 0;
int Iceman::mem_force = 0;
int Lion::mem_blood = 0;
int Lion::mem_force = 0;
int Lion::lion_k = 0;
int Wolf::mem_blood = 0;
int Wolf::mem_force = 0;
int Arrow::arrow_r = 0;

int get_hour(int t) { return t / 60; }//获取当前小时数
int get_minute(int t) { return t % 60; }//获取当前分钟数
void output_time(int t) { cout << setw(3) << setfill('0') << get_hour(t) << ':' << setw(2) << setfill('0') << get_minute(t); }//输出当前时间
bool Compare(const Weapon* x1, const Weapon* x2)
{
	if (x1->name < x2->name)
		return true;
	else
		return false;
}//武器排序函数的排序规则——按字典序排序
vector<Weapon*>new_weapon_to_delete;
vector<Warrior*>new_warrior_to_delete;

void World::run(int T) {
	int time_in_minute = 0;//记录以分钟为单位的时间
	while (1) {
		if (time_in_minute > T)//超过时间限制，游戏结束
			break;
		int time_remain = time_in_minute % 60;//该小时的分钟数
		switch (time_remain)
		{
		case 0:
			Red->warrior_born(time_in_minute);
			Blue->warrior_born(time_in_minute);
			break;
		case 5:
			lion_escape(time_in_minute);
			break;
		case 10:
			warrior_march(time_in_minute);
			if (Red->conquered || Blue->conquered)//武士前进后可能占领敌方司令部，需要判断
				return;
			break;
		case 20:
			city_add_element(time_in_minute);
			break;
		case 30:
			get_city_element(time_in_minute);
			break;
		case 35:
			arrow_attack(time_in_minute);
			break;
		case 38:
			warrior_judge_bomb(time_in_minute);
			break;
		case 40:
			warrior_battle(time_in_minute);
			break;
		case 50:
			report_element(time_in_minute);
			break;
		case 55:
			warrior_report_weapon(time_in_minute);
			break;
		}
		//时间控制
		if (time_remain == 35)
			time_in_minute += 3;
		else if (time_remain == 38)
			time_in_minute += 2;
		else
			time_in_minute += 5;
	}
}
void World::lion_escape(int t) {
	auto i = myCity.begin();
	for (; i != myCity.end(); ++i) {
		if ((*i)->red != nullptr && (*i)->red->name == "lion" && (*i)->red->cityNo != Blue->cityNo) {//抵达敌方司令部的lion不会逃跑
			auto j = (Lion*)((*i)->red);
			if (j->loyalty <= 0) {
				output_time(t);
				cout << ' ' << "red lion " << (*i)->red->id << " ran away" << endl;
				(*i)->red = nullptr;
			}
		}
		if ((*i)->blue != nullptr && (*i)->blue->name == "lion" && (*i)->blue->cityNo != Red->cityNo) {//抵达敌方司令部的lion不会逃跑
			auto j = (Lion*)((*i)->blue);
			if (j->loyalty <= 0) {
				output_time(t);
				cout << ' ' << "blue lion " << (*i)->blue->id << " ran away" << endl;
				(*i)->blue = nullptr;
			}
		}
	}
}
void World::warrior_march(int t) {//事件发生在武士前进的目的地
	auto i = myCity.begin();
	for (; i != myCity.end(); ++i) {
		if (i > myCity.begin() && (*(i - 1))->red != nullptr) {
			if ((*(i - 1))->red->cityNo != (*(i - 1))->red->part->enemy_Headquarter->cityNo) {//未到达敌方司令部
				(*(i - 1))->red->march();
				if ((*(i - 1))->red->cityNo == (*(i - 1))->red->part->enemy_Headquarter->cityNo) {//前进后到达敌方司令部
					output_time(t);
					cout << " red " << (*(i - 1))->red->name << ' '
						<< (*(i - 1))->red->id << " reached blue headquarter with " << (*(i - 1))->red->blood
						<< " elements and force " << (*(i - 1))->red->force << endl;
					(*i)->cnt_red_win++;//到达司令部的战士计数
					if ((*i)->cnt_red_win == 2) {//占领敌方司令部
						Blue->conquered = true;
						output_time(t);
						cout << " blue headquarter was taken" << endl;
					}
				}
				else {//前进后仍未到达敌方司令部
					output_time(t);
					cout << " red " << (*(i - 1))->red->name << ' '
						<< (*(i - 1))->red->id << " marched to city " << (*(i - 1))->red->cityNo << " with " << (*(i - 1))->red->blood <<
						" elements and force " << (*(i - 1))->red->force << endl;
				}
			}
		}
		if (i < --myCity.end() && (*(i + 1))->blue != nullptr) {
			if ((*(i + 1))->blue->cityNo != (*(i + 1))->blue->part->enemy_Headquarter->cityNo) {//未到达敌方司令部
				(*(i + 1))->blue->march();
				if ((*(i + 1))->blue->cityNo == (*(i + 1))->blue->part->enemy_Headquarter->cityNo) {//前进后到达敌方司令部
					output_time(t);
					cout << " blue " << (*(i + 1))->blue->name << ' '
						<< (*(i + 1))->blue->id << " reached red headquarter with " << (*(i + 1))->blue->blood
						<< " elements and force " << (*(i + 1))->blue->force << endl;
					(*i)->cnt_blue_win++;//到达司令部的战士计数
					if ((*i)->cnt_blue_win == 2) {//占领敌方司令部
						Red->conquered = true;
						output_time(t);
						cout << " red headquarter was taken" << endl;
					}
				}
				else {
					output_time(t);
					cout << " blue " << (*(i + 1))->blue->name << ' '
						<< (*(i + 1))->blue->id << " marched to city " << (*(i + 1))->blue->cityNo << " with " << (*(i + 1))->blue->blood <<
						" elements and force " << (*(i + 1))->blue->force << endl;
				}
			}
		}
	}
	i = --myCity.end();
	for (; i != myCity.begin(); --i) {
		if ((*(i - 1))->red != nullptr) {
			(*i)->red = (*(i - 1))->red;
			(*(i - 1))->red = nullptr;
		}
	}
	i = myCity.begin();
	for (; i != --myCity.end(); ++i) {
		if ((*(i + 1))->blue != nullptr) {
			(*i)->blue = (*(i + 1))->blue;
			(*(i + 1))->blue = nullptr;
		}
	}
}
void World::city_add_element(int t) {
	auto i = myCity.begin() + 1;
	for (; i != myCity.end() - 1; ++i) {//司令部不产出生命元
		(*i)->ele += 10;
	}
}
void World::get_city_element(int t) {
	auto i = myCity.begin() + 1;
	for (; i != myCity.end() - 1; ++i) {
		if ((*i)->red != nullptr && (*i)->blue == nullptr) {//只有红方武士在该城市
			Red->element += (*i)->ele;
			output_time(t);
			cout << " red " << (*i)->red->name << ' ' << (*i)->red->id << " earned " << (*i)->ele << " elements for his headquarter" << endl;
			(*i)->ele = 0;
		}
		if ((*i)->red == nullptr && (*i)->blue != nullptr) {//只有蓝方武士在该城市
			Blue->element += (*i)->ele;
			output_time(t);
			cout << " blue " << (*i)->blue->name << ' ' << (*i)->blue->id << " earned " << (*i)->ele << " elements for his headquarter" << endl;
			(*i)->ele = 0;
		}
	}
}
void World::arrow_attack(int t) {
	auto i = myCity.begin();
	for (; i != myCity.end(); ++i) {
		if (i < myCity.end() - 2 && (*i)->red != nullptr && (*(i + 1))->blue != nullptr) {//不能向敌方司令部放箭
			auto j = (*i)->red->weapons.begin();
			for (; j != (*i)->red->weapons.end(); ++j) {
				if ((*j)!=nullptr&&(*j)->name == "arrow") {//查找arrow
					(*j)->attack((*(i + 1))->blue, t);
					break;
				}
			}
		}
		if (i > myCity.begin() + 1 && (*i)->blue != nullptr && (*(i - 1))->red != nullptr) {//不能向敌方司令部放箭
			auto j = (*i)->blue->weapons.begin();
			for (; j != (*i)->blue->weapons.end(); ++j) {
				if ((*j)!=nullptr&&(*j)->name == "arrow") {//查找arrow
					(*j)->attack((*(i - 1))->red, t);
					break;
				}
			}
		}
	}
}
void World::warrior_judge_bomb(int t) {
	auto i = myCity.begin() + 1;
	for (; i < myCity.end() - 1; ++i) {//司令部不会发生战斗
		if ((*i)->red != nullptr && (*i)->blue != nullptr && (*i)->red->blood > 0 && (*i)->blue->blood > 0) {//该城市在40分钟会发生战斗
			int red_force = (*i)->red->force;//红方主动进攻攻击力
			int red_back_force = (*i)->red->force / 2;//红方反击攻击力
			int blue_force = (*i)->blue->force;//蓝方主动进攻攻击力
			int blue_back_force = (*i)->blue->force / 2;//蓝方反击攻击力
			auto j1 = (*i)->red->weapons.begin();
			auto j2 = (*i)->blue->weapons.begin();
			for (; j1 != (*i)->red->weapons.end(); ++j1) {//查找sword
				if ((*j1) != nullptr && (*j1)->name == "sword") {//有则加上sword攻击力
					red_force += (*j1)->weapon_force;
					red_back_force += (*j1)->weapon_force;
				}
			}
			for (; j2 != (*i)->blue->weapons.end(); ++j2) {//查找sword
				if ((*j2) != nullptr && (*j2)->name == "sword") {//有则加上sword攻击力
					blue_force += (*j2)->weapon_force;
					blue_back_force += (*j2)->weapon_force;
				}
			}
			int pos = (*i)->red->cityNo;//获取城市位置
			if ((pos % 2 == 1 && (*i)->flag[0] == '\0') || (*i)->flag == "red") {//红方先手
				j1 = (*i)->red->weapons.begin();
				j2 = (*i)->blue->weapons.begin();
				for (; j1 != (*i)->red->weapons.end(); ++j1) {//查找bomb
					if ((*j1) != nullptr && (*j1)->name == "bomb") {//有bomb
						if (red_force < (*i)->blue->blood && blue_back_force >= (*i)->red->blood && (*i)->blue->name != "ninja") {//被反击死亡则丢炸弹(ninja不反击)
							(*j1)->attack((*i)->blue, t);
							(*i)->blue = nullptr;
							(*i)->red = nullptr;
							break;
						}
					}
				}
				if ((*i)->blue != nullptr)//红方没丢炸弹
					for (; j2 != (*i)->blue->weapons.end(); ++j2) {//查找bomb
						if ((*j2) != nullptr && (*j2)->name == "bomb") {//有bomb
							if (red_force >= (*i)->blue->blood) {//被主动进攻死亡则丢炸弹
								(*j2)->attack((*i)->red, t);
								(*i)->blue = nullptr;
								(*i)->red = nullptr;
								break;
							}
						}
					}
			}
			if ((pos % 2 == 0 && (*i)->flag[0] == '\0') || (*i)->flag == "blue") {//蓝方先手
				j1 = (*i)->red->weapons.begin();
				j2 = (*i)->blue->weapons.begin();
				for (; j2 != (*i)->blue->weapons.end(); ++j2) {//查找bomb
					if ((*j2) != nullptr && (*j2)->name == "bomb") {//有bomb
						if (blue_force < (*i)->red->blood && red_back_force >= (*i)->blue->blood && (*i)->red->name != "ninja") {//被反击死亡则丢炸弹(ninja不反击)
							(*j2)->attack((*i)->red, t);
							(*i)->blue = nullptr;
							(*i)->red = nullptr;
							break;
						}
					}
				}
				if ((*i)->blue != nullptr)
					for (; j1 != (*i)->red->weapons.end(); ++j1) {//查找bomb
						if ((*j1) != nullptr && (*j1)->name == "bomb") {//有bomb
							if (blue_force >= (*i)->red->blood) {//被主动进攻死亡则丢炸弹
								(*j1)->attack((*i)->blue, t);
								(*i)->blue = nullptr;
								(*i)->red = nullptr;
								break;
							}
						}
					}
			}
		}
	}
}
void World::warrior_battle(int t) {
	lion_update_init_blood();
	battle_on(t);
	after_battle(t);
}
void World::lion_update_init_blood() {
	auto i = myCity.begin() + 1;
	for (; i != myCity.end() - 1; ++i) {//司令部无战斗，不用更新血量
		if ((*i)->blue != nullptr && (*i)->blue->name == "lion") {
			Lion* k = (Lion*)((*i)->blue);
			k->update_init_blood();
		}
		if ((*i)->red != nullptr && (*i)->red->name == "lion") {
			Lion* k = (Lion*)((*i)->red);
			k->update_init_blood();
		}
	}
}
void World::battle_on(int t) {//讨论所有可能情况
	auto i = myCity.begin() + 1;
	for (; i != myCity.end() - 1; ++i) {//司令部不会发生战斗
		if ((*i)->red != nullptr && (*i)->blue != nullptr) {//有武士且都不是被炸死的
			if ((*i)->red->blood <= 0 && (*i)->blue->blood <= 0) {}//都被箭射死了，不算战斗，无事发生
			else if ((*i)->red->blood <= 0 && (*i)->blue->blood > 0) {//红方武士被箭射死但蓝方武士存活，蓝方胜
				(*i)->blue->win((*i)->red, t);
				if (((*i)->flag[0] == '\0' && (*i)->blue->cityNo % 2 == 0) || (*i)->flag == "blue")//蓝方先手，可能欢呼
					(*i)->blue->judge_yell(t);
				if ((*i)->ele > 0) {//该城市有生命元则获取
					output_time(t);
					cout << " blue " << (*i)->blue->name << ' ' << (*i)->blue->id << " earned " << (*i)->ele << " elements for his headquarter" << endl;
					Blue->tmp_element += (*i)->ele;
					(*i)->ele = 0;
				}
				(*i)->cnt_blue_win++;
				(*i)->cnt_red_win = 0;
				(*i)->win = "blue";
				if ((*i)->cnt_blue_win % 2 == 0 && (*i)->flag != "blue") {//旗帜升起
					(*i)->flag = "blue";
					output_time(t);
					cout << " blue flag raised in city " << (*i)->blue->cityNo << endl;
				}
			}
			else if ((*i)->red->blood > 0 && (*i)->blue->blood <= 0) {//蓝方武士被箭射死但红方武士存活，红方胜
				(*i)->red->win((*i)->blue, t);
				if (((*i)->flag[0] == '\0' && (*i)->red->cityNo % 2 == 1) || (*i)->flag == "red")//红方先手，可能欢呼
					(*i)->red->judge_yell(t);
				if ((*i)->ele > 0) {//该城市有生命元则获取
					output_time(t);
					cout << " red " << (*i)->red->name << ' ' << (*i)->red->id << " earned " << (*i)->ele << " elements for his headquarter" << endl;
					Red->tmp_element += (*i)->ele;
					(*i)->ele = 0;
				}
				(*i)->cnt_red_win++;
				(*i)->cnt_blue_win = 0;
				(*i)->win = "red";
				if ((*i)->cnt_red_win % 2 == 0 && (*i)->flag != "red") {//旗帜升起
					(*i)->flag = "red";
					output_time(t);
					cout << " red flag raised in city " << (*i)->red->cityNo << endl;
				}
			}
			else {//双方都存活，正式战斗
				int pos = (*i)->blue->cityNo;
				if ((*i)->flag == "red" || ((*i)->flag[0] =='\0' && pos % 2 == 1)) {//红方先手
					(*i)->red->attack((*i)->blue, t);
					if ((*i)->blue->blood <= 0) {//蓝方被杀死则红方胜
						(*i)->blue->killed(t);
						(*i)->red->win((*i)->blue, t);
						(*i)->red->judge_yell(t);//红方先手，可能欢呼
						if ((*i)->ele > 0) {//该城市有生命元则获取
							output_time(t);
							cout << " red " << (*i)->red->name << ' ' << (*i)->red->id << " earned " << (*i)->ele << " elements for his headquarter" << endl;
							Red->tmp_element += (*i)->ele;
							(*i)->ele = 0;
						}
						(*i)->cnt_red_win++;
						(*i)->cnt_blue_win = 0;//蓝方连胜中断
						(*i)->win = "red";
						if ((*i)->cnt_red_win % 2 == 0 && (*i)->flag != "red") {//旗帜升起
							(*i)->flag = "red";
							output_time(t);
							cout << " red flag raised in city " << (*i)->red->cityNo << endl;
						}
					}
					else {//蓝方没被杀死，可能反击
						if ((*i)->blue->name == "ninja") {//ninja不反击
							(*i)->red->even();//平局
							(*i)->red->judge_yell(t);//红方先手，可能欢呼
							(*i)->cnt_blue_win = 0;
							(*i)->cnt_red_win = 0;//双方连胜均中断
							(*i)->win = '\0';//无胜者
						}
						else {//蓝方反击
							(*i)->blue->fight_back((*i)->red, t);
							if ((*i)->red->blood <= 0) {//红方被杀死
								(*i)->red->killed(t);
								(*i)->blue->win((*i)->red, t);
								if ((*i)->ele > 0) {//该城市有生命元则获取
									output_time(t);
									cout << " blue " << (*i)->blue->name << ' ' << (*i)->blue->id << " earned " << (*i)->ele << " elements for his headquarter" << endl;
									Blue->tmp_element += (*i)->ele;
									(*i)->ele = 0;
								}
								(*i)->cnt_blue_win++;
								(*i)->cnt_red_win = 0;//红方连胜中断
								(*i)->win = "blue";
								if ((*i)->cnt_blue_win % 2 == 0 && (*i)->flag != "blue") {//旗帜升起
									(*i)->flag = "blue";
									output_time(t);
									cout << " blue flag raised in city " << (*i)->blue->cityNo << endl;
								}
							}
							else {
								(*i)->red->even();
								(*i)->blue->even();//平局
								(*i)->red->judge_yell(t);//红方先手，可能欢呼
								(*i)->cnt_blue_win = 0;
								(*i)->cnt_red_win = 0;//双方连胜均中断
								(*i)->win = '\0';
							}
						}
					}
				}
				else {//蓝方先手(过程与上类似)
					(*i)->blue->attack((*i)->red, t);
					if ((*i)->red->blood <= 0) {
						(*i)->red->killed(t);
						(*i)->blue->win((*i)->red, t);
						(*i)->blue->judge_yell(t);
						if ((*i)->ele > 0) {
							output_time(t);
							cout << " blue " << (*i)->blue->name << ' ' << (*i)->blue->id << " earned " << (*i)->ele << " elements for his headquarter" << endl;
							Blue->tmp_element += (*i)->ele;
							(*i)->ele = 0;
						}
						(*i)->cnt_blue_win++;
						(*i)->cnt_red_win = 0;
						(*i)->win = "blue";
						if ((*i)->cnt_blue_win % 2 == 0 && (*i)->flag != "blue") {
							(*i)->flag = "blue";
							output_time(t);
							cout << " blue flag raised in city " << (*i)->blue->cityNo << endl;
						}
					}
					else {
						if ((*i)->red->name == "ninja") {
							(*i)->blue->even();
							(*i)->blue->judge_yell(t);
							(*i)->cnt_blue_win = 0;
							(*i)->cnt_red_win = 0;
							(*i)->win = '\0';
						}
						else {
							(*i)->red->fight_back((*i)->blue, t);
							if ((*i)->blue->blood <= 0) {
								(*i)->blue->killed(t);
								(*i)->red->win((*i)->blue, t);
								if ((*i)->ele > 0) {
									output_time(t);
									cout << " red " << (*i)->red->name << ' ' << (*i)->red->id << " earned " << (*i)->ele << " elements for his headquarter" << endl;
									Red->tmp_element += (*i)->ele;
									(*i)->ele = 0;
								}
								(*i)->cnt_red_win++;
								(*i)->cnt_blue_win = 0;
								(*i)->win = "red";
								if ((*i)->cnt_red_win % 2 == 0 && (*i)->flag != "red") {
									(*i)->flag = "red";
									output_time(t);
									cout << " red flag raised in city " << (*i)->red->cityNo << endl;
								}
							}
							else {
								(*i)->red->even();
								(*i)->blue->even();
								(*i)->blue->judge_yell(t);
								(*i)->cnt_blue_win = 0;
								(*i)->cnt_red_win = 0;
								(*i)->win = '\0';
							}
						}
					}
				}
			}
		}
	}
}
void World::after_battle(int t) {
	auto i = myCity.end() - 2;
	for (; i > myCity.begin(); --i) {//红方自东向西奖励武士
		if ((*i)->win == "red") {
			if (Red->element >= 8) {
				Red->element -= 8;
				(*i)->red->blood += 8;
			}
		}
	}
	Red->element += Red->tmp_element;//获取武士夺得的生命元
	Red->tmp_element = 0;
	i = myCity.begin() + 1;
	for (; i < myCity.end() - 1; ++i) {//蓝方自西向东奖励武士
		if ((*i)->win == "blue") {
			if (Blue->element >= 8) {
				Blue->element -= 8;
				(*i)->blue->blood += 8;
			}
		}
	}
	Blue->element += Blue->tmp_element;//获取武士夺取的生命元
	Blue->tmp_element = 0;
	clear_up();//清理战场
}
void World::warrior_report_weapon(int t) {//自西向东，先红后蓝
	auto i = myCity.begin();
	int cnt = 0;
	for (; i != myCity.end(); ++i) {
		if ((*i)->red != nullptr) {
			cnt = 0;//区分输出的是第几把武器
			if ((*i)->red->weapons.size() > 1) {//有武器
				auto j = ++(*i)->red->weapons.begin();
				for (; j != (*i)->red->weapons.end(); ++j) {
					if (cnt == 0) {//之前没有输出
						output_time(t);
						cout << " red " << (*i)->red->name << ' ' << (*i)->red->id << " has " << (*j)->name;
						cnt++;
					}
					else {
						cout << ',' << (*j)->name;
					}
					if ((*j)->name == "arrow") {
						Arrow* k = (Arrow*)(*j);
						cout << '(' << 3 - k->use_cnt << ')';//输出可用次数
					}
					if ((*j)->name == "sword") {
						cout << '(' << (*j)->weapon_force << ')';//输出攻击力
					}
				}
				cout << endl;
			}
			else {//没武器
				output_time(t);
				cout << " red " << (*i)->red->name << ' ' << (*i)->red->id << " has no weapon" << endl;
			}
		}
	}
	//蓝方与上类似
	i = myCity.begin();
	for (; i != myCity.end(); ++i) {
		if ((*i)->blue != nullptr) {
			cnt = 0;
			if ((*i)->blue->weapons.size() > 1) {
				auto j = ++(*i)->blue->weapons.begin();
				for (; j != (*i)->blue->weapons.end(); ++j) {
					if (cnt == 0) {
						output_time(t);
						cout << " blue " << (*i)->blue->name << ' ' << (*i)->blue->id << " has " << (*j)->name;
						cnt++;
					}
					else {
						cout << ',' << (*j)->name;
					}
					if ((*j)->name == "arrow") {
						Arrow* k = (Arrow*)(*j);
						cout << '(' << 3 - k->use_cnt << ')';
					}
					if ((*j)->name == "sword") {
						cout << '(' << (*j)->weapon_force << ')';
					}
				}
				cout << endl;
			}
			else {
				output_time(t);
				cout << " blue " << (*i)->blue->name << ' ' << (*i)->blue->id << " has no weapon" << endl;
			}
		}
	}
}
void World::clear_up() {
	auto i = myCity.begin()+1;
	for (; i != myCity.end()-1; ++i) {//将战死的战士指针置空
		if ((*i)->blue != nullptr && (*i)->blue->blood <= 0)
			(*i)->blue = nullptr;
		if ((*i)->red != nullptr && (*i)->red->blood <= 0)
			(*i)->red = nullptr;
		(*i)->win = '\0';//胜者记录置空
	}
}
void World::report_element(int t) {
	output_time(t);
	cout <<' '<< Red->element << " elements in red headquarter" << endl;
	output_time(t);
	cout <<' '<< Blue->element << " elements in blue headquarter" << endl;
}
void Headquarter::warrior_born(int t) {//双方生成武士的顺序不同，因此分开讨论
	if (color == "red") {
		int mod_red = now_id % 5;//生成武士有顺序
		switch (mod_red)
		{
		case 0:
			if (element >= Iceman::mem_blood) {
				now_id++;
				Warrior* p = new Iceman(Iceman::mem_blood, Iceman::mem_force, this, "iceman");
				new_warrior_to_delete.push_back(p);//new出来的指针要delete
				p->get_weapon();
				position->red = p;
				output_time(t);
				p->born_report();
			}
			break;
		case 1:
			if (element >= Lion::mem_blood) {
				now_id++;
				Warrior* p = new Lion(Lion::mem_blood, Lion::mem_force, this, "lion");
				new_warrior_to_delete.push_back(p);
				p->get_weapon();
				position->red = p;
				output_time(t);
				p->born_report();
			}
			break;
		case 2:
			if (element >= Wolf::mem_blood) {
				now_id++;
				Warrior* p = new Wolf(Wolf::mem_blood, Wolf::mem_force, this, "wolf");
				new_warrior_to_delete.push_back(p);
				p->get_weapon();
				position->red = p;
				output_time(t);
				p->born_report();
			}
			break;
		case 3:
			if (element >= Ninja::mem_blood) {
				now_id++;
				Warrior* p = new Ninja(Ninja::mem_blood, Ninja::mem_force, this, "ninja");
				new_warrior_to_delete.push_back(p);
				p->get_weapon();
				position->red = p;
				output_time(t);
				p->born_report();
			}
			break;
		case 4:
			if (element >= Dragon::mem_blood) {
				now_id++;
				Warrior* p = new Dragon(Dragon::mem_blood, Dragon::mem_force, this, "dragon");
				new_warrior_to_delete.push_back(p);
				p->get_weapon();
				position->red = p;
				output_time(t);
				p->born_report();
			}
			break;
		}
	}
	else {
		int mod_blue = now_id % 5;//生成武士有顺序
		switch (mod_blue)
		{
		case 0:
			if (element >= Lion::mem_blood) {
				now_id++;
				Warrior* p = new Lion(Lion::mem_blood, Lion::mem_force, this, "lion");
				new_warrior_to_delete.push_back(p);
				p->get_weapon();
				position->blue = p;
				output_time(t);
				p->born_report();
			}
			break;
		case 1:
			if (element >= Dragon::mem_blood) {
				now_id++;
				Warrior* p = new Dragon(Dragon::mem_blood, Dragon::mem_force, this, "dragon");
				new_warrior_to_delete.push_back(p);
				p->get_weapon();
				position->blue = p;
				output_time(t);
				p->born_report();
			}
			break;
		case 2:
			if (element >= Ninja::mem_blood) {
				now_id++;
				Warrior* p = new Ninja(Ninja::mem_blood, Ninja::mem_force, this, "ninja");
				new_warrior_to_delete.push_back(p);
				p->get_weapon();
				position->blue = p;
				output_time(t);
				p->born_report();
			}
			break;
		case 3:
			if (element >= Iceman::mem_blood) {
				now_id++;
				Warrior* p = new Iceman(Iceman::mem_blood, Iceman::mem_force, this, "iceman");
				new_warrior_to_delete.push_back(p);
				p->get_weapon();
				position->blue = p;
				output_time(t);
				p->born_report();
			}
			break;

		case 4:
			if (element >= Wolf::mem_blood) {
				now_id++;
				Warrior* p = new Wolf(Wolf::mem_blood, Wolf::mem_force, this, "wolf");
				new_warrior_to_delete.push_back(p);
				p->get_weapon();
				position->blue = p;
				output_time(t);
				p->born_report();
			}
			break;
		}
	}
}
void Warrior::get_weapon() {
	int weapon1_id = id % 3;
	int weapon2_id = (id + 1) % 3;//ninja有两把武器
	Weapon* p1;
	Weapon* p2;
	switch (name[0]) {
	case 'd':
		switch (weapon1_id) {
		case 0:
			p1 = new Sword(this, "sword");
			new_weapon_to_delete.push_back(p1);
			if (p1->weapon_force == 0)//sword攻击力为0视为没有sword
				break;
			weapons.push_back(p1);
			break;
		case 1:
			p1 = new Bomb(this, "bomb");
			new_weapon_to_delete.push_back(p1);
			weapons.push_back(p1);
			break;
		case 2:
			p1 = new Arrow(this, "arrow");
			new_weapon_to_delete.push_back(p1);
			weapons.push_back(p1);
			break;
		}
		break;
	case 'n':
		switch (weapon1_id) {
		case 0:
			p1 = new Sword(this, "sword");
			new_weapon_to_delete.push_back(p1);
			if (p1->weapon_force == 0)//sword攻击力为0视为没有sword
				break;
			weapons.push_back(p1);
			break;
		case 1:
			p1 = new Bomb(this, "bomb");
			new_weapon_to_delete.push_back(p1);
			weapons.push_back(p1);
			break;
		case 2:
			p1 = new Arrow(this, "arrow");
			new_weapon_to_delete.push_back(p1);
			weapons.push_back(p1);
			break;
		}
		switch (weapon2_id) {
		case 0:
			p2 = new Sword(this, "sword");
			new_weapon_to_delete.push_back(p2);
			if (p2->weapon_force == 0)//sword攻击力为0视为没有sword
				break;
			weapons.push_back(p2);
			break;
		case 1:
			p2 = new Bomb(this, "bomb");
			new_weapon_to_delete.push_back(p2);
			weapons.push_back(p2);
			break;
		case 2:
			p2 = new Arrow(this, "arrow");
			new_weapon_to_delete.push_back(p2);
			weapons.push_back(p2);
			break;
		}
		break;
	case 'i':
		switch (weapon1_id) {
		case 0:
			p1 = new Sword(this, "sword");
			new_weapon_to_delete.push_back(p1);
			if (p1->weapon_force == 0)//sword攻击力为0视为没有sword
				break;
			weapons.push_back(p1);
			break;
		case 1:
			p1 = new Bomb(this, "bomb");
			new_weapon_to_delete.push_back(p1);
			weapons.push_back(p1);
			break;
		case 2:
			p1 = new Arrow(this, "arrow");
			new_weapon_to_delete.push_back(p1);

			weapons.push_back(p1);
			break;
		}
		break;
	case 'l':
		break;
	case 'w':
		break;
	}
	sort_weapon();//获得武器后整理武器
}
void Warrior::march() {
	if (part->color == "red") {
		cityNo++;
	}
	else
		cityNo--;
}
void Warrior::born_report() {
	cout << ' ' << part->color << ' ' << name << ' ' << id << " born" << endl;
}
void Warrior::win(Warrior* enemy, int t) {
	if (name == "dragon") {//dragon战胜士气增加
		Dragon* k = (Dragon*)this;
		k->morale += 0.2;
	}
	if (name == "wolf") {//wolf战胜缴获敌人武器
		Wolf* k = (Wolf*)this;
		k->get_enemy_weapon(enemy);
	}
	if (enemy->name == "lion") {//lion战败转移生命值
		Lion* k = (Lion*)enemy;
		k->blood_trans((Lion*)(this));
	}
}
void Warrior::even() {
	if (name == "dragon") {//dragon打平士气下降
		Dragon* k = (Dragon*)this;
		k->morale -= 0.2;
	}
	if (name == "lion") {//lion打平忠诚度下降
		Lion* k = (Lion*)this;
		k->loyalty -= k->lion_k;
	}
}
void Warrior::killed(int t) {
	output_time(t);
	cout << ' ' << part->color << ' ' << name << ' ' << id << " was killed in city " << cityNo << endl;
}
void Warrior::judge_yell(int t) {
	if (name == "dragon") {
		Dragon* k = (Dragon*)this;
		if (k->morale > 0.8)
			k->yell(t);
	}
}
void Warrior::sort_weapon() {
	if (weapons.size() > 1)//有武器才排序
		sort(++weapons.begin(), weapons.end(), Compare);
}
void Warrior::attack(Warrior* enemy, int t) {
	int tmp_force = force;
	auto i = weapons.begin();
	for (; i != weapons.end(); ++i) {
		if ((*i) != nullptr && (*i)->name == "sword") {//有sword要加上sword的攻击力
			tmp_force += (*i)->weapon_force;
			(*i)->weapon_force = (*i)->weapon_force * 4 / 5;//sword会变钝
			if ((*i)->weapon_force == 0)//sword攻击力减为零则丢弃
				(*i)->lost();
			break;
		}
	}
	enemy->blood -= tmp_force;
	output_time(t);
	cout << ' ' << part->color << ' ' << name << ' ' << id << " attacked " << enemy->part->color << ' ' << enemy->name << ' ' << enemy->id << " in city "
		<< cityNo << " with " << blood << " elements and force " << force << endl;
	if (enemy->blood <= 0) {//敌人被杀死
		enemy->blood = 0;
	}
}
void Warrior::fight_back(Warrior* enemy, int t) {
	int tmp_force = force / 2;
	auto i = weapons.begin();
	for (; i != weapons.end(); ++i) {
		if ((*i) != nullptr && (*i)->name == "sword") {//有sword要加上sword的攻击力
			tmp_force += (*i)->weapon_force;
			(*i)->weapon_force = (*i)->weapon_force * 4 / 5;//sword会变钝
			if ((*i)->weapon_force == 0)//sword攻击力减为零则丢弃
				(*i)->lost();
			break;
		}
	}
	enemy->blood -= tmp_force;
	output_time(t);
	cout << ' ' << part->color << ' ' << name << ' ' << id << " fought back against " << enemy->part->color << ' ' << enemy->name << ' ' << enemy->id << " in city "
		<< cityNo << endl;
	if (enemy->blood <= 0) {//敌人被杀死
		enemy->blood = 0;
	}
}
void Dragon::yell(int t) {
	output_time(t);
	cout << ' '<<part->color << " dragon " << id << " yelled in city " << cityNo << endl;
}
void Dragon::born_report() {
	cout << ' ' << part->color << ' ' << name << ' ' << id << " born" << endl;
	cout << "Its morale is " << fixed << setprecision(2) << morale << endl;
}
void Iceman::march() {
	cnt_march++;//前进计数+1
	if (part->color == "red")
		cityNo++;
	else
		cityNo--;
	if (cnt_march % 2 == 0) {//每走两步
		blood -= 9;
		if (blood <= 0)
			blood = 1;//血量不会<=0
		force += 20;
	}
}
void Lion::update_init_blood() {
	init_blood = blood;
}
void Lion::blood_trans(Warrior*enemy) {
	enemy->blood += init_blood;
}
void Lion::born_report() {
	cout << ' ' << part->color << ' ' << name << ' ' << id << " born" << endl;
	cout << "Its loyalty is " << loyalty << endl;
}
void Wolf::get_enemy_weapon(Warrior* enemy) {
	auto i = enemy->weapons.begin();
	for (; i != enemy->weapons.end(); ++i) {
		if ((*i) != nullptr) {//敌人有武器
			auto j = weapons.begin();
			for (; j != weapons.end(); ++j) {//看自己有没有相同武器
				if ((*j) != nullptr && (*j)->name == (*i)->name) {
					break;
				}
			}
			if (j == weapons.end()) {//没有相同武器
				weapons.push_back((*i));
				(*i)->master = this;//改变武器主人
			}
		}
	}
	sort_weapon();//排好武器
}
void Weapon::attack(Warrior*enemy,int t) {
}//实际用不上(sword不需要attack函数)
void Weapon::lost() {
	auto i = find(master->weapons.begin(), master->weapons.end(), this);
	master->weapons.erase(i);//从武器池中删去该武器
}
void Arrow::attack(Warrior* enemy,int t) {
	enemy->blood -= weapon_force;
	output_time(t);
	cout << ' ' << master->part->color << ' ' << master->name << ' ' << master->id << " shot";
	if (enemy->blood <= 0) {//放箭杀死对方
		enemy->blood = 0;
		cout << " and killed " << master->part->enemy_Headquarter->color << ' ' << enemy->name << ' ' << enemy->id;
	}
	cout << endl;
	use_cnt++;//使用次数计数+1
	if (use_cnt==3)//用完丢弃
		lost();
}
void Bomb::attack(Warrior* enemy,int t) {
	enemy->blood = 0;
	master->blood = 0;
	lost();//一次性，用完就没有了
	output_time(t);
	cout << ' '<<master->part->color<<' ' << master->name << ' ' << master->id << " used a bomb and killed " <<enemy->part->color<<' ' << enemy->name << ' ' << enemy->id << endl;
}

int main()
{
	int t;
	cin >> t;
	for (int ii = 1; ii <= t; ii++) {
		cout << "Case " << ii << ':' << endl;
		int headquarter_m, city_n, time_t;
		cin >> headquarter_m >> city_n >> Arrow::arrow_r >> Lion::lion_k >> time_t;
		cin >> Dragon::mem_blood >> Ninja::mem_blood >> Iceman::mem_blood >> Lion::mem_blood >> Wolf::mem_blood;
		cin >> Dragon::mem_force >> Ninja::mem_force >> Iceman::mem_force >> Lion::mem_force >> Wolf::mem_force;
		//初始化世界
		Headquarter red(headquarter_m, "red", 0);
		Headquarter blue(headquarter_m, "blue", city_n + 1);
		World myWorld(&red, &blue);
		City _city[25];
		for (int i = 0; i <= city_n + 1; i++) { myWorld.myCity.push_back(&_city[i]); }
		red.enemy_Headquarter = &blue;
		blue.enemy_Headquarter = &red;
		red.position = *(myWorld.myCity.begin());
		blue.position = *(--myWorld.myCity.end());
		//游戏开始
		myWorld.run(time_t);
	}
	//游戏结束后delete所有new出来的指针
	auto i1 = new_weapon_to_delete.begin();
	auto i2 = new_warrior_to_delete.begin();
	for (; i1 != new_weapon_to_delete.end(); ++i1)
		delete(*i1);
	for (; i2 != new_warrior_to_delete.end(); ++i2)
		delete(*i2);
	return 0;
}
