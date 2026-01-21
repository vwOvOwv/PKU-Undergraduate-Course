#include <cstdio>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;
class weapon;
class city;
class Base;
class warrior {
   public:
    int id;              // 战士编号
    int elements;        // 生命值
    int force;           //攻击力
    string part;         // 阵营
    string name;         //名称
    int location;        //所在城市
    int cur_weapon_ind;  //当前使用武器的序号
    vector<weapon*> weapons;
    warrior(int id_,
            int elements_,
            string part_,
            string name_,
            int location_,
            int force_)
        : id(id_),
          elements(elements_),
          part(part_),
          name(name_),
          location(location_),
          force(force_) {
        weapons = vector<weapon*>(10, nullptr);
    }
    warrior(warrior& w) {
        id = w.id;
        elements = w.elements;
        force = w.force;
        part = w.part;
        name = w.name;
        location = w.location;
        // todo  weapons = move(w.weapons);
        weapons = move(w.weapons);
    }

    virtual void march();
    void reach_headquarter();
    void sort_weapons(bool used_first);
    //数有多少可用武器，在clear和sort之后调用
    int count_weapons();
    //清除不能再使用的武器
    void clear_weapons();
    //战斗结束缴获武器
    void take_weapons_after_fight(warrior* enemy);

    bool is_alive();
    //寻找合适的可用武器
    weapon* find_proper_weapon();
    //攻击敌人
    void attack(warrior* enemy);
};
class dragon : public warrior {
   public:
    void yell();
    dragon(warrior& w) : warrior(w) {}
};
class lion : public warrior {
   public:
    int loyalty;
    lion(warrior& w, int loyalty_) : warrior(w), loyalty(loyalty_) {}
    virtual void march();
    bool escape();
};

class iceman : public warrior {
   public:
    virtual void march();
    iceman(warrior& w) : warrior(w) {}
};

class wolf : public warrior {
   public:
    void rob_weapons(warrior* enemy);
    wolf(warrior& w) : warrior(w) {}
};

class ninja : public warrior {
   public:
    ninja(warrior& w) : warrior(w) {}
};
class weapon {
   public:
    string name;
    weapon(string name_) : name(name_) {}
    virtual void attack(warrior* attacker, warrior* injure) {}
    virtual bool is_usable() { return false; }
};

class sword : public weapon {
   public:
    virtual void attack(warrior* attacker, warrior* injure);
    sword(string name_) : weapon(name_) {}
    virtual bool is_usable();
};
class bomb : public weapon {
   public:
    bool usable;
    bomb(string name_) : weapon(name_), usable(true) {}
    virtual void attack(warrior* attacker, warrior* injure);
    virtual bool is_usable();
};
class arrow : public weapon {
   public:
    int use_count;
    arrow(string name_, int use_count_)
        : weapon(name_), use_count(use_count_) {}
    virtual void attack(warrior* attacker, warrior* injure);
    virtual bool is_usable();
};
unordered_map<string, int> part_warrior_cnt;  //映射某派战士数量,用于生成id
unordered_map<string, int>
    warrior_element;  //映射某种战士的生命值，用于保存输入数据
unordered_map<string, int> warrior_force;  //映射武士攻击力
unordered_map<string, vector<string>>
    warrior_build_order;  //映射某派的战士制造顺序
vector<city> cities;      //城市向量
sword* sw;                //唯一的共用的宝剑
vector<string> red_order = {"iceman", "lion", "wolf", "ninja", "dragon"};
vector<string> blue_order = {"lion", "dragon", "ninja", "iceman", "wolf"};
vector<string> weapon_kinds = {"sword", "bomb", "arrow"};

int loyalty_decrease, time_limit, city_count;

enum fight_result { BOTH_DEAD, BOTH_ALIVE, BLUE_WIN, RED_WIN };

class city {
   public:
    // first是红战士指针，second是蓝战士指针
    warrior* first;
    warrior* second;
    city(warrior* red, warrior* blue) : first(red), second(blue) {}
};
//事件系统，统筹战斗的所有时间
class event_system {
   public:
    static int clock;
    static bool conqured_red, conqured_blue;
    static void happen00_warrior_born(Base& red, Base& blue);
    static void warrior_born(warrior& w);
    static void time_go();
    static string get_format_clock();
    static void happen05_lion_escape();
    static void happen10_warrior_march();
    static void happen35_wolf_rob();
    static fight_result fight(warrior* r, warrior* b);
    static void happen40_war();
    static void happen50_report_base(Base& red, Base& blue);
    static void report_base(Base& base);
    static void report_warrior(warrior* w);
    static void happen55_report_warriors();
};
int event_system::clock = 0;
bool event_system::conqured_blue = false;
bool event_system::conqured_red = false;

class Base {
   public:
    string part;                //阵营
    int cur_life_value;         //当前剩余生命值
    static int red_build_ind;   //制造第几个红战士
    static int blue_build_ind;  //制造第几个蓝战士
    static void increase_build_inds(string part, int bound);
    Base(string part_, int cur_life_value_)
        : part(part_), cur_life_value(cur_life_value_) {}

    warrior* make_warrior();
    //制造武士时为其分配武器
    weapon* make_weapon(string name);
    warrior* further_process(warrior* p);
    void make_warriors();
};
int Base::blue_build_ind = 0;
int Base::red_build_ind = 0;

void warrior::march() {
    int direction = part == "red" ? 1 : -1;  //红军为1向右走，-1向左走
    location += direction;
    cout << event_system::get_format_clock() << " " << part << " " << name
         << " " << id << " marched to city " << location << " with " << elements
         << " elements and force " << force << endl;
}

void warrior::reach_headquarter() {
    if (name == "iceman") {
        int decrease_elements = elements / 10;
        elements -= decrease_elements;
    }
    string enemy_part = part == "blue" ? "red" : "blue";

    cout << event_system::get_format_clock() << " " << part << " " << name
         << " " << id << " reached " << enemy_part << " headquarter with "
         << elements << " elements and force " << force << endl;
    cout << event_system::get_format_clock() << " " << enemy_part
         << " headquarter was taken\n";
    if (enemy_part == "blue")
        event_system::conqured_blue = true;
    else
        event_system::conqured_red = true;
}

void warrior::sort_weapons(bool used_first) {
    // used_first: 使用过的arrow靠前还是靠后
    // 排序方法：桶排序
    int sword_cnt = 0, bomb_cnt = 0, new_arrow_cnt = 0, used_arrow_cnt = 0;
    for (int i = 0; i < 10; i++) {
        weapon* p = weapons[i];
        if (p == nullptr)
            continue;
        if (p->name == "sword") {
            sword_cnt++;
        } else if (p->name == "bomb") {
            bomb* b = dynamic_cast<bomb*>(p);
            if (b->is_usable()) {
                bomb_cnt++;
            }
            delete p;
        } else {
            arrow* a = dynamic_cast<arrow*>(p);
            if (a->use_count == 0) {
                new_arrow_cnt++;
            } else if (a->use_count == 1) {
                used_arrow_cnt++;
            }
            delete p;
        }
        weapons[i] = nullptr;
    }
    int ind = 0;
    while (sword_cnt--)
        weapons[ind++] = sw;
    while (bomb_cnt--)
        weapons[ind++] = new bomb("bomb");
    if (!used_first) {
        //新arrow排在前
        while (new_arrow_cnt--)
            weapons[ind++] = new arrow("arrow", 0);
        while (used_arrow_cnt--)
            weapons[ind++] = new arrow("arrow", 1);
    } else {
        //新的arrow排在后
        while (used_arrow_cnt--)
            weapons[ind++] = new arrow("arrow", 1);
        while (new_arrow_cnt--)
            weapons[ind++] = new arrow("arrow", 0);
    }

    cur_weapon_ind = 0;
}

int warrior::count_weapons() {
    int cnt = 0;
    for (weapon* p : weapons) {
        if (p != nullptr)
            cnt++;
        else
            break;
    }
    return cnt;
}

void warrior::clear_weapons() {
    for (int i = 0; i < 10; i++) {
        if (weapons[i] != nullptr && !weapons[i]->is_usable()) {
            delete weapons[i];
            weapons[i] = nullptr;
        }
    }
}

void warrior::take_weapons_after_fight(warrior* enemy) {
    if (this == enemy)
        return;
    if (elements <= 0)
        return;
    enemy->clear_weapons();
    enemy->sort_weapons(false);
    clear_weapons();
    sort_weapons(true);
    int tot_weapon = count_weapons();
    int ind = 0;
    for (int i = tot_weapon; i < 10; i++) {
        weapons[i] = enemy->weapons[ind++];
    }
    sort_weapons(true);
}

bool warrior::is_alive() {
    return elements > 0;
}
weapon* warrior::find_proper_weapon() {
    weapon* res = weapons[cur_weapon_ind];
    if (res != nullptr && res->is_usable()) {
        cur_weapon_ind = (cur_weapon_ind + 1) % 10;
        return res;
    } else {
        int weapon_ind = (cur_weapon_ind + 1) % 10;
        while (weapon_ind != cur_weapon_ind) {
            res = weapons[weapon_ind];
            if (res != nullptr && res->is_usable()) {
                cur_weapon_ind = (weapon_ind + 1) % 10;
                return res;
            } else {
                weapon_ind = (weapon_ind + 1) % 10;
            }
        }
    }
    return nullptr;
}
void warrior::attack(warrior* enemy) {
    weapon* w = find_proper_weapon();
    if (w == nullptr) {
        return;
    }
    w->attack(this, enemy);
}

void dragon::yell() {
    cout << event_system::get_format_clock() << " " << part << " " << name
         << " " << id << " yelled in city " << location << endl;
}

void lion::march() {
    int direction = part == "red" ? 1 : -1;  //红军为1向右走，-1向左走
    location += direction;
    loyalty -= loyalty_decrease;
    cout << event_system::get_format_clock() << " " << part << " " << name
         << " " << id << " marched to city " << location << " with " << elements
         << " elements and force " << force << endl;
}

bool lion::escape() {
    // 狮子的逃跑函数，返回是否逃跑
    if (loyalty > 0)
        return false;
    else {
        cout << event_system::get_format_clock() << " " << part << " lion "
             << id << " ran away\n";
        return true;
    }
}

void iceman::march() {
    int direction = part == "red" ? 1 : -1;  //红军为1向右走，-1向左走
    location += direction;
    int decrease_elements = elements / 10;
    elements -= decrease_elements;
    cout << event_system::get_format_clock() << " " << part << " " << name
         << " " << id << " marched to city " << location << " with " << elements
         << " elements and force " << force << endl;
}

void wolf::rob_weapons(warrior* enemy) {
    if (enemy->name == "wolf")
        return;
    if (enemy == this)
        return;
    enemy->clear_weapons();
    enemy->sort_weapons(false);
    clear_weapons();
    sort_weapons(true);

    weapon* first_weapon = enemy->weapons[0];
    if (first_weapon == nullptr)
        return;

    string first_weapon_name = first_weapon->name;
    int self_ind = count_weapons(), enemy_ind = 0;
    for (weapon* p : enemy->weapons) {
        if (p == nullptr || p->name != first_weapon_name)
            break;
        weapons[self_ind++] = p;
        enemy->weapons[enemy_ind++] = nullptr;
    }
    enemy->sort_weapons(false);
    sort_weapons(true);
    cout << event_system::get_format_clock() << " " << part << " " << name
         << " " << id << " took " << enemy_ind << " " << first_weapon_name
         << " from " << enemy->part << " " << enemy->name << " " << enemy->id
         << " in city " << location << endl;
}

void sword::attack(warrior* attacker, warrior* injure) {
    int attack = (attacker->force * 2) / 10;
    injure->elements -= attack;
}

bool sword::is_usable() {
    //宝剑一定是可用的
    return true;
}

void bomb::attack(warrior* attacker, warrior* injure) {
    if (!usable)
        return;
    int attack = (attacker->force * 4) / 10;
    int self_attack = attacker->name == "ninja" ? 0 : attack / 2;
    attacker->elements -= self_attack;
    injure->elements -= attack;
    usable = false;
}

bool bomb::is_usable() {
    return usable;
}

void arrow::attack(warrior* attacker, warrior* injure) {
    if (use_count >= 2)
        return;
    int attack = (attacker->force * 3) / 10;
    injure->elements -= attack;
    use_count++;
}

bool arrow::is_usable() {
    return use_count < 2;
}

void event_system::happen00_warrior_born(Base& red, Base& blue) {
    red.make_warriors();
    blue.make_warriors();
}

void event_system::warrior_born(warrior& w) {
    cout << get_format_clock() << " " << w.part << " " << w.name << " " << w.id
         << " born\n";
    if (w.name == "lion") {
        lion* l = dynamic_cast<lion*>(&w);
        cout << "Its loyalty is " << l->loyalty << endl;
    }
}

void event_system::time_go() {
    event_system::clock += 5;
}

string event_system::get_format_clock() {
    int hour = clock / 60;
    int minute = clock % 60;
    ostringstream ost;
    ost << setw(3) << setfill('0') << hour << ":";
    ost << setw(2) << setfill('0') << minute;
    return move(ost.str());
}

void event_system::happen05_lion_escape() {
    for (int i = 0; i < cities.size(); i++) {
        warrior *first = cities[i].first, *second = cities[i].second;
        if (first != nullptr && first->name == "lion") {
            lion* l = dynamic_cast<lion*>(first);
            if (l->escape()) {
                delete (cities[i].first);
                cities[i].first = nullptr;
            }
        }

        if (second != nullptr && second->name == "lion") {
            lion* l = dynamic_cast<lion*>(second);
            if (l->escape()) {
                delete (cities[i].second);
                cities[i].second = nullptr;
            }
        }
    }
}
void event_system::happen10_warrior_march() {
    warrior* b = cities[1].second;
    if (b != nullptr) {
        b->reach_headquarter();
        delete (cities[1].second);
        cities[1].second = nullptr;
    }

    warrior* next_move_red = cities[0].first;
    cities[0].first = nullptr;

    for (int i = 1; i < cities.size() - 1; i++) {
        warrior *r = next_move_red, *b = cities[i + 1].second;
        next_move_red = cities[i].first;
        if (r != nullptr) {
            r->march();
        }
        cities[i].first = r;

        if (b != nullptr) {
            b->march();
        }
        cities[i].second = b;
    }
    cities[cities.size() - 1].second = nullptr;
    if (next_move_red != nullptr) {
        next_move_red->reach_headquarter();
        delete next_move_red;
    }
}
void event_system::happen35_wolf_rob() {
    for (int i = 1; i < cities.size() - 1; i++) {
        warrior *r = cities[i].first, *b = cities[i].second;
        if (r == nullptr || b == nullptr)
            continue;
        if (r->name == "wolf") {
            wolf* w = dynamic_cast<wolf*>(r);
            w->rob_weapons(b);
        }
        if (b->name == "wolf") {
            wolf* w = dynamic_cast<wolf*>(b);
            w->rob_weapons(r);
        }
    }
}

fight_result event_system::fight(warrior* first, warrior* second) {
    int still_epoch = 0;
    while (first->is_alive() && second->is_alive()) {
        int first_elements = first->elements,
            second_elements = second->elements;
        if (first->is_alive())
            first->attack(second);

        if (second->is_alive())
            second->attack(first);

        if (first_elements == first->elements &&
            second_elements == second->elements) {
            still_epoch++;
        }

        if (still_epoch >= 12)
            return BOTH_ALIVE;
    }
    warrior* alived_warrior = first->is_alive() ? first : second;
    if (!alived_warrior->is_alive())
        return BOTH_DEAD;
    else {
        if (alived_warrior->part == "red")
            return RED_WIN;
        else
            return BLUE_WIN;
    }
}

void event_system::happen40_war() {
    for (int i = 1; i < cities.size() - 1; i++) {
        warrior *r = cities[i].first, *b = cities[i].second;
        if (r == nullptr || b == nullptr) {
            continue;
        }
        r->clear_weapons();
        r->sort_weapons(true);
        b->clear_weapons();
        b->sort_weapons(true);
        fight_result res;
        if (i % 2)
            res = fight(r, b);
        else
            res = fight(b, r);
        dragon* rd = dynamic_cast<dragon*>(r);
        dragon* bd = dynamic_cast<dragon*>(b);
        if (res == BOTH_ALIVE) {
            cout << event_system::get_format_clock() << " both red " << r->name
                 << " " << r->id << " and blue " << b->name << " " << b->id
                 << " were alive in city " << r->location << endl;
            if (rd != nullptr)
                rd->yell();
            if (bd != nullptr)
                bd->yell();
        } else if (res == BOTH_DEAD) {
            cout << event_system::get_format_clock() << " both red " << r->name
                 << " " << r->id << " and blue " << b->name << " " << b->id
                 << " died in city " << r->location << endl;
        } else if (res == RED_WIN) {
            r->take_weapons_after_fight(b);
            cout << event_system::get_format_clock() << " red " << r->name
                 << " " << r->id << " killed blue " << b->name << " " << b->id
                 << " in city " << r->location << " remaining " << r->elements
                 << " elements\n";
            if (rd != nullptr)
                rd->yell();
        } else {
            b->take_weapons_after_fight(r);
            cout << event_system::get_format_clock() << " blue " << b->name
                 << " " << b->id << " killed red " << r->name << " " << r->id
                 << " in city " << b->location << " remaining " << b->elements
                 << " elements\n";
            if (bd != nullptr)
                bd->yell();
        }
        r->clear_weapons();
        r->sort_weapons(true);
        b->clear_weapons();
        b->sort_weapons(true);
        if (!r->is_alive()) {
            delete (cities[i].first);
            cities[i].first = nullptr;
        }
        if (!b->is_alive()) {
            delete (cities[i].second);
            cities[i].second = nullptr;
        }
    }
}

void event_system::happen50_report_base(Base& red, Base& blue) {
    report_base(red);
    report_base(blue);
}

void event_system::report_base(Base& base) {
    cout << event_system::get_format_clock() << " " << base.cur_life_value
         << " elements in " << base.part << " headquarter\n";
}

void event_system::report_warrior(warrior* w) {
    if (w == nullptr)
        return;
    int sword_cnt = 0, bomb_cnt = 0, arrow_cnt = 0;
    w->clear_weapons();
    w->sort_weapons(true);
    for (weapon* p : w->weapons) {
        if (p == nullptr)
            continue;
        if (p->name == "sword")
            sword_cnt++;
        if (p->name == "bomb")
            bomb_cnt++;
        if (p->name == "arrow")
            arrow_cnt++;
    }
    cout << event_system::get_format_clock() << " " << w->part << " " << w->name
         << " " << w->id << " has " << sword_cnt << " sword " << bomb_cnt
         << " bomb " << arrow_cnt << " arrow and " << w->elements
         << " elements\n";
}

void event_system::happen55_report_warriors() {
    for (int i = 1; i < cities.size() - 1; i++) {
        warrior *r = cities[i].first, *b = cities[i].second;
        report_warrior(r);
        report_warrior(b);
    }
}

void Base::increase_build_inds(string part, int bound) {
    // bound 战士数目
    if ("red" == part) {
        Base::red_build_ind = (Base::red_build_ind + 1) % bound;
    } else {
        Base::blue_build_ind = (Base::blue_build_ind + 1) % bound;
    }
}

warrior* Base::make_warrior() {
    warrior* ret = nullptr;
    int warrior_id = part_warrior_cnt[part] + 1;
    int ind = -1, location = -1;
    ind = part == "red" ? red_build_ind : blue_build_ind;
    location = part == "red" ? 0 : city_count + 1;
    int warrior_element_ready = warrior_element[warrior_build_order[part][ind]];
    if (warrior_element_ready <= cur_life_value) {
        part_warrior_cnt[part]++;
        cur_life_value -= warrior_element_ready;
        increase_build_inds(part, 5);
        warrior* p = new warrior(warrior_id, warrior_element_ready, part,
                                 warrior_build_order[part][ind], location,
                                 warrior_force[warrior_build_order[part][ind]]);
        ret = p;
    }
    return ret;
}

weapon* Base::make_weapon(string name) {
    if (name == "sword")
        return sw;
    if (name == "bomb")
        return new bomb(name);
    if (name == "arrow")
        return new arrow(name, 0);
    return nullptr;
}

warrior* Base::further_process(warrior* p) {
    if (p == nullptr) {
        return nullptr;
    }
    string name = p->name;
    warrior* ret = nullptr;
    if (name == "dragon") {
        string weapon_name = weapon_kinds[p->id % 3];
        p->weapons[0] = make_weapon(weapon_name);
        ret = new dragon(*p);
    } else if (name == "ninja") {
        string weapon_name1 = weapon_kinds[p->id % 3],
               weapon_name2 = weapon_kinds[(p->id + 1) % 3];

        p->weapons[0] = make_weapon(weapon_name1);
        p->weapons[1] = make_weapon(weapon_name2);
        ret = new ninja(*p);
    } else if (name == "iceman") {
        string weapon_name = weapon_kinds[p->id % 3];
        p->weapons[0] = make_weapon(weapon_name);
        ret = new iceman(*p);
    } else if (name == "lion") {
        string weapon_name = weapon_kinds[p->id % 3];
        p->weapons[0] = make_weapon(weapon_name);
        ret = new lion(*p, cur_life_value);
    } else if (name == "wolf") {
        ret = new wolf(*p);
    }
    return ret;
}

void Base::make_warriors() {
    warrior* p = further_process(make_warrior());
    if (p != nullptr) {
        event_system::warrior_born(*p);
        if (p->part == "red") {
            cities[0].first = p;
        } else {
            cities[cities.size() - 1].second = p;
        }
    }
}

void __init__() {
    event_system::clock = 0;
    event_system::conqured_blue = false;
    event_system::conqured_red = false;
    Base::blue_build_ind = 0;
    Base::red_build_ind = 0;
    warrior_build_order["blue"] = blue_order;
    warrior_build_order["red"] = red_order;
    if (sw == 0)
        sw = new sword("sword");
    for (city& c : cities) {
        if (c.first != nullptr)
            delete c.first;
        if (c.second != nullptr)
            delete c.second;
    }
    cities.clear();
    for (int i = 0; i < city_count + 2; i++) {
        cities.push_back(city(nullptr, nullptr));
    }
    part_warrior_cnt["red"] = 0;
    part_warrior_cnt["blue"] = 0;
}

int main() {
    int tot;
    scanf("%d", &tot);
    for (int c = 1; c <= tot; c++) {
        printf("Case %d:\n", c);
        int tot_elements;
        cin >> tot_elements >> city_count >> loyalty_decrease >> time_limit;
        cin >> warrior_element["dragon"] >> warrior_element["ninja"] >>
            warrior_element["iceman"] >> warrior_element["lion"] >>
            warrior_element["wolf"];
        cin >> warrior_force["dragon"] >> warrior_force["ninja"] >>
            warrior_force["iceman"] >> warrior_force["lion"] >>
            warrior_force["wolf"];
        __init__();
        Base red("red", tot_elements);
        Base blue("blue", tot_elements);
        while (event_system::clock <= time_limit &&
               (!event_system::conqured_blue && !event_system::conqured_red)) {
            if (event_system::clock % 60 == 0) {
                event_system::happen00_warrior_born(red, blue);
            } else if (event_system::clock % 60 == 5) {
                event_system::happen05_lion_escape();
            } else if (event_system::clock % 60 == 10)
                event_system::happen10_warrior_march();
            else if (event_system::clock % 60 == 35)
                event_system::happen35_wolf_rob();
            else if (event_system::clock % 60 == 40)
                event_system::happen40_war();
            else if (event_system::clock % 60 == 50)
                event_system::happen50_report_base(red, blue);
            else if (event_system::clock % 60 == 55) {
                event_system::happen55_report_warriors();
            }
            event_system::time_go();
        }
    }
}
