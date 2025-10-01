#include <iomanip>
#include <unordered_map>
#include <utility>
#include <cstdlib>
#include <iostream>
#include <vector>

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
double theta = 1e-4;
unordered_map<int, double> value_tab;
unordered_map<int, vector<double>> policy;

int coor2key(int x, int y){
    return y * H + x;
}

pair<int, int> key2coor(int key){
    int y = key / H;
    int x = key % H;
    return make_pair(x, y);
}

int main() {
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            value_tab[coor2key(x, y)] = 0.0;
            policy[coor2key(x, y)].assign(4, 0.25); // random policy
        }
    }
    while(true){
        // policy evaluation
        while(true){
            double delta = 0.0;
            unordered_map<int, double> new_value_tab = value_tab;
            for (int y = 0; y < H; y++) {
                for (int x = 0; x < W; x++) {
                    GridWorld::State old_state = make_pair(x, y);
                    int cur_state_key = coor2key(x, y);
                    double cur_value = value_tab[cur_state_key];
                    
                    double expected_value = 0.0;
                    for (int action = 0; action < 4; action++) {
                        double prob = policy[cur_state_key][action];    // policy is changing
                        GridWorld tmp_env = GridWorld(x, y, false);
                        auto new_state_reward = tmp_env.step(action);
                        GridWorld::State new_state = new_state_reward.first;
                        double reward = new_state_reward.second;
                        int new_state_key = coor2key(new_state.first, new_state.second);
                        expected_value += prob * (reward + gamma * value_tab[new_state_key]);
                    }
                    new_value_tab[cur_state_key] = expected_value;
                    delta = max(delta, abs(cur_value - new_value_tab[cur_state_key]));
                }
            }
            value_tab = new_value_tab;
            if (delta < theta) 
                break;
        }
        // policy improvement
        bool policy_diff = false;
        for (int y = 0; y < H; y++) {
            for (int x = 0; x < W; x++) {
                int state_key = coor2key(x, y);
                auto old_policy = policy[state_key];

                int max_reward = 0x80000000;
                vector<int> max_reward_actions; // may have multiple best actions
                for(int action = 0; action < 4; action++){
                    GridWorld tmp_env = GridWorld(x, y, false);
                    auto new_state_reward = tmp_env.step(action);
                    GridWorld::State new_state = new_state_reward.first;
                    double reward = new_state_reward.second;
                    int new_state_key = coor2key(new_state.first, new_state.second);

                    if(reward + gamma * value_tab[new_state_key] > max_reward){
                        max_reward = reward + gamma * value_tab[new_state_key];
                        max_reward_actions.clear();
                        max_reward_actions.push_back(action);
                    }
                    else if(reward + gamma * value_tab[new_state_key] == max_reward){
                        max_reward_actions.push_back(action);
                    }
                }
                policy[state_key].assign(4, 0.0);
                double prob = 1.0 / max_reward_actions.size();
                for(auto action: max_reward_actions){
                    policy[state_key][action] = prob;
                }
                for(int action = 0; action < 4; action++){
                    if(old_policy[action] != policy[state_key][action]){
                        policy_diff = true;
                        break;
                    }
                }
            }
        }
        if(!policy_diff)
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