#include <ctime>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

#include "tictactoe.hpp"

using namespace std;


struct PairHasher {
    std::size_t operator()(const std::pair<int, int>& p) const {
        auto hash1 = std::hash<int>{}(p.first);
        auto hash2 = std::hash<int>{}(p.second);
        return hash1 ^ (hash2 << 1);
    }
};

int num_samples = 10000000;
double alpha = 0.01;
bool alpha_decay = false;
double eps = 0.3;
bool train = true;
unordered_map< pair<int, int>, double, PairHasher> value_tab;

class TicTacToePolicyBase{
    public:
        virtual TicTacToe::Action operator()(const TicTacToe::State& state) const = 0;
};

// randomly select a valid action for the step.
class TicTacToePolicyRandom : public TicTacToePolicyBase{
    public:
        TicTacToe::Action operator()(const TicTacToe::State& state) const {
            vector<TicTacToe::Action> actions = state.action_space();
            int n_action = actions.size();
            int action_id = rand() % n_action;
            if (state.turn == TicTacToe::PLAYER_X){
                return actions[action_id];
            } else {
                return actions[action_id];
            }
        }
        TicTacToePolicyRandom(){
            srand(time(nullptr));
        }
};

// select the first valid action.
class TicTacToePolicyDefault : public TicTacToePolicyBase{
    public:
        TicTacToe::Action operator()(const TicTacToe::State& state) const {
            vector<TicTacToe::Action> actions = state.action_space();
            int n_action = actions.size();
            TicTacToe::Action action;
            if (state.turn == TicTacToe::PLAYER_X){
                // TODO
                TicTacToe::Action action;
                if(train && ((rand() % 10000) * 0.0001 <= eps)){
                    TicTacToe::State next_state;
                    int action_id = rand() % n_action;
                    action = actions[action_id];
                }
                else{
                    double max_reward = -2.0;
                    vector<int> max_reward_idx;
                    for(int i = 0; i < n_action; i++){
                        TicTacToe::State next_state = state;
                        next_state.put(actions[i]);
                        pair<int, int> key = make_pair(next_state.board, next_state.turn);
                        if(value_tab.find(key) == value_tab.end())
                            value_tab[key] = 0.5;

                        if(value_tab[key] == max_reward)
                            max_reward_idx.push_back(i);
                        else if(value_tab[key] > max_reward){
                            max_reward = value_tab[key];
                            max_reward_idx.clear();
                            max_reward_idx.push_back(i);
                        }
                    }
                    action = actions[max_reward_idx[rand() % int(max_reward_idx.size())]];
                }
                return action;
            }
            else{
                return actions[0];
            }
        }
        TicTacToePolicyDefault(){}
};


#include <chrono>
#include <thread>

// randomly select action
// int main(){
//     bool done = false;
//     // set verbose true
//     TicTacToe env(true);
//     // TicTacToePolicyDefault policy;
//     TicTacToePolicyRandom policy;
//     while (not done){
//         TicTacToe::State state = env.get_state();
//         TicTacToe::Action action = policy(state);
//         env.step(action);
//         done = env.done();
//         // env.step_back();
//         std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//     }
//     int winner = env.winner();
//     return 0;
// };

// train and validate our policy
int main(){
    srand(time(nullptr));
    train = true;
    int win_cnt = 0;
    TicTacToe env(false);
    TicTacToePolicyDefault policy;
    cout << "Training starts" << endl;
    for(int i = 0; i < num_samples; i++){
        env.reset();
        TicTacToe::State state;
        TicTacToe::Action action;
        bool done = false;
        if(alpha_decay)
            alpha = 1.0 / (i + 1);
        while(not done){
            state = env.get_state();
            action = policy(state);
            env.step(action);
            done = env.done();
        }
        int winner = env.winner();
        pair<int, int> key = make_pair(state.board, state.turn);
        if(winner == TicTacToe::PLAYER_X){
            value_tab[key] = 1.0;
            win_cnt++;
        }
        else if(winner == TicTacToe::PLAYER_O)
            value_tab[key] = 0.0;
        else
            value_tab[key] = 0.5;

        if ((i + 1) % 100000 == 0) {
            cout << "Episode " << i + 1 << ", Win rate: " << (win_cnt * 100.0) / (i + 1) << "%" << endl;
        }
        
        TicTacToe::State next_state = state;
        while(env.step_back()){
            state = env.get_state();
            pair<int, int> state_key = make_pair(state.board, state.turn);
            pair<int, int> next_state_key = make_pair(next_state.board, next_state.turn);
            if (value_tab.find(state_key) == value_tab.end()) {
                value_tab[state_key] = 0.5;
            }
            value_tab[state_key] += alpha * (value_tab[next_state_key] - value_tab[state_key]);
            next_state = state;
        }
    }
    cout << "Training finished, validation starts" << endl;

    train = false;
    env.verbose = true;
    env.reset();
    bool done = false;
    while (not done){
        TicTacToe::State state = env.get_state();
        TicTacToe::Action action = policy(state);
        env.step(action);
        done = env.done();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    int winner = env.winner();
    return 0;
}