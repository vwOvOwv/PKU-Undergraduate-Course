#include <iomanip>
#include <utility>
#include <cstdlib>
#include <iostream>

using namespace std;
class GridWorld{
    public:
        static const int 
            NORTH=0, SOUTH=1, EAST=2, WEST=3;
        static const char ACTION_NAME[][16];
        typedef pair<int, int> State;
        bool verbose;
        State state(){
            return make_pair(x, y);
        }
        void set_state(int x, int y){
            this->x = x;
            this->y = y;
            if (verbose){
                cout << "State reset: (" << x << "," << y << ")" << endl;
            }
        }
        void reset(){
            set_state(0, 0);
        }
        pair<State, double> step(int action){
            State old_state = state();
            double reward = state_transition(action);
            if (verbose){
                cout << "State: (" << old_state.first << "," << old_state.second << ")" << endl;
                cout << "Action: " << ACTION_NAME[action] << endl;
                cout << "Reward: " << reward << endl;
                cout << "New State: (" << x << "," << y << ")" << endl << endl;
            }
            return make_pair(state(), reward);  // return new state and reward
        }
        int sample_action(){
            return rand() % 4;
        }
        GridWorld(int x=0, int y=0, bool verbose=false){
            this->verbose = verbose;
            set_state(x, y);
        }
        
    private:
        int x, y;
        double state_transition(int action){
            if (state() == make_pair(1, 0)){
                x = 1;
                y = 4;
                return 10.0;
            }
            if (state() == make_pair(3, 0)){
                x = 3;
                y = 2;
                return 5.0;
            }
            if (action == NORTH and y == 0 or
                action == SOUTH and y == 4 or
                action == EAST and x == 4 or
                action == WEST and x == 0){
                return -1.0; 
            }
            switch (action){
                case NORTH:
                    y --; break;
                case SOUTH:
                    y ++; break;
                case EAST:
                    x ++; break;
                case WEST:
                    x --; break;
            }
            return 0.0;
        }
};
const char GridWorld::ACTION_NAME[][16] = {"NORTH(0,-1)", "SOUTH(0,1)", "EAST:(1,0)", "WEST:(-1,0)"};

int H = 5, W = 5;
double gamma = 0.9;
int num_iters = 5e7;
double alpha = 0.001;
unordered_map<int, double> value_tab;
unordered_map<int, double> value_tab_snap_shot = value_tab;
unordered_map<int, double> visit_cnt;


int coor2key(int x, int y){
    return y * H + x;
}

pair<int, int> key2coor(int key){
    int y = key / H;
    int x = key % H;
    return make_pair(x, y);
}

int main(){
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            value_tab[coor2key(x, y)] = 0.0;
            value_tab_snap_shot[coor2key(x, y)] = 0.0;
            visit_cnt[coor2key(x, y)] = 0;
        }
    }
    int iter = 0;
    GridWorld env = GridWorld(0, 0, false);
    while (true){
        int action = env.sample_action();
        auto old_state = env.state();
        auto new_state_reward = env.step(action);
        iter++;
        int old_state_key = coor2key(old_state.first, old_state.second);
        int new_state_key = coor2key(new_state_reward.first.first, new_state_reward.first.second);
        visit_cnt[old_state_key]++;
        // estimate expectation through many samples
        // use alpha when we don't know env's dynamics (function p), which means a model-free method
        // value_tab[old_state_key] += (new_state_reward.second + gamma * value_tab[new_state_key] - value_tab[old_state_key]) / visit_cnt[old_state_key];
        value_tab[old_state_key] += alpha * (new_state_reward.second + gamma * value_tab[new_state_key] - value_tab[old_state_key]);
        if(iter > num_iters)
            break;
    }
    
    for(int y = 0; y < H; y++){
        for(int x = 0; x < W; x++){
            cout << fixed << setprecision(2) << value_tab[coor2key(x, y)];
            if(x < W - 1)
                cout << '\t';
        }
        cout << endl;
    }
    return 0;
}