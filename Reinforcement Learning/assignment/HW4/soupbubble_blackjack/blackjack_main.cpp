#include <cfloat>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <algorithm>
#include <type_traits>
#include <vector>
#include "blackjack.hpp"

class BlackjackPolicyBase{
    public:
        virtual int operator() (const Blackjack::State& state)=0;
};

class BlackjackPolicyDefault : public BlackjackPolicyBase{
    public:
        int operator() (const Blackjack::State& state){
            if (state.turn == Blackjack::PLAYER){
                return state.player_sum >= 20 ? Blackjack::STICK : Blackjack::HIT;
            } else {
                return state.dealer_sum >= 17 ? Blackjack::STICK : Blackjack::HIT;
            }
        }
};

class BlackjackPolicyLearnableDefault : public BlackjackPolicyBase{
    static constexpr const char *ACTION_NAME = "HS";
    public:
        int operator() (const Blackjack::State& state){
            if (state.turn == Blackjack::DEALER){
                return state.dealer_sum >= 17 ? Blackjack::STICK : Blackjack::HIT;
            } else {
                return policy[state.dealer_shown][state.player_ace][state.player_sum];
            }
        }
        void update_policy(){
            // TODO
            // simply take argmax
            for (int dealer_shown = 1; dealer_shown <= 10; ++ dealer_shown){
                for (int player_ace = 0; player_ace <= 1; ++ player_ace){
                    for (int player_sum = 0; player_sum <= 21; ++ player_sum){
                        if(player_sum < 11)
                            policy[dealer_shown][player_ace][player_sum] = Blackjack::HIT;
                        else if(player_sum == 21)
                            policy[dealer_shown][player_ace][player_sum] = Blackjack::STICK;
                        else{
                            if(value[dealer_shown][player_ace][player_sum][Blackjack::HIT] > \
                                value[dealer_shown][player_ace][player_sum][Blackjack::STICK])
                                policy[dealer_shown][player_ace][player_sum] = Blackjack::HIT;
                            else
                                policy[dealer_shown][player_ace][player_sum] = Blackjack::STICK;
                        }
                    }
                }
            }
            
        }
        // n iterations for each start (initial state and first action), no need to modify.
        void update_value(Blackjack& env, int n=10000){
            // TODO
            // REMEMBER to call set_value_initial() at the beginning
            set_value_initial();
            // simulate from every possible initial state:(dealer_shown, player_ace, player_sum) \
                (call Blackjack::reset(,,) to do this) and player's every possible first action
            for(int i = 0; i < n; i++){ // n iterations, each itertaion goes across each state as initial state 
                for (int dealer_shown = 1; dealer_shown <= 10; ++ dealer_shown){
                    for (int player_ace = 0; player_ace <= 1; ++ player_ace){
                        for (int player_sum = 0; player_sum <= 21; ++ player_sum){
                            for(int first_action = 0; first_action < 2; first_action++){
                                env.reset(dealer_shown, player_ace, player_sum);
                                Blackjack::StepResult result;
                                episode.clear();
                                episode.push_back(EpisodeStep(env.state(), first_action));  // PLAYER first
                                result = env.step(first_action);
                                bool done = result.done;
                                while(not done){
                                    Blackjack::State state = env.state();
                                    int next_action;
                                    if(state.turn == Blackjack::DEALER){
                                        if(state.dealer_sum >= 17)
                                            next_action = Blackjack::STICK;
                                        else
                                            next_action = Blackjack::HIT;
                                    }
                                    else{
                                        // if(state.player_sum < 11)
                                        //     next_action = Blackjack::HIT;
                                        // else if(state.player_sum == 21)
                                        //     next_action = Blackjack::STICK;
                                        // else
                                        next_action = policy[state.dealer_shown][state.player_ace][state.player_sum];
                                        episode.push_back(EpisodeStep(state, next_action));
                                    }
                                    result = env.step(next_action);
                                    done = result.done;
                                }
                                double episode_return = result.player_reward;
                                // update state-action value
                                // BE AWARE only use player's steps (rather than dealer's) to update value estimation. 
                                int episode_len = episode.size();
                                int first_visited[11][2][22][2];
                                fill_n(&first_visited[0][0][0][0], 11 * 2 * 22 * 2, 1);
                                for(int k = 0; k < episode_len; k++){
                                    int dealer_shown = episode[k].dealer_shown;
                                    int player_ace = episode[k].player_ace;
                                    int player_sum = episode[k].player_sum;
                                    int action = episode[k].action;
                                    if(first_visited[dealer_shown][player_ace][player_sum][action]){
                                        first_visited[dealer_shown][player_ace][player_sum][action] = 0;
                                        double average = value[dealer_shown][player_ace][player_sum][action];
                                        int count = ++state_action_count[dealer_shown][player_ace][player_sum][action];
                                        // discount factor = 1, no middle reward
                                        value[dealer_shown][player_ace][player_sum][action] = average + (episode_return - average) / count;
                                    }
                                }
                                // update policy after each iteration (all states have been initial states)
                                update_policy();
                            }
                        }
                    }
                }
            }
        }
        void print_policy() const {
            cout << setw(10) << "Player Without Ace" << "\t\t" << "Player With Usable Ace." << endl;
            for (int player_sum = 21; player_sum >= 11; -- player_sum){
                for (int dealer_shown = 1; dealer_shown <= 10; ++ dealer_shown){
                    cout << ACTION_NAME[policy[dealer_shown][0][player_sum]];
                }
                cout << "\t\t\t\t" ;
                for (int dealer_shown = 1; dealer_shown <= 10; ++ dealer_shown){
                    cout << ACTION_NAME[policy[dealer_shown][1][player_sum]];
                }
                cout << endl;
            }
            cout << endl;
        }
        void print_value() const {
            cout << setw(40) << "Player Without Ace" << setw(20) <<"\t\t\t" << "Player With Usable Ace." << endl;
            for (int player_sum = 21; player_sum >= 11; -- player_sum){
                for (int dealer_shown = 1; dealer_shown <= 10; ++ dealer_shown){
                    cout << fixed << setprecision(2) << setw(6) << value[dealer_shown][0][player_sum][
                        policy[dealer_shown][0][player_sum]];
                }
                cout << "\t";
                for (int dealer_shown = 1; dealer_shown <= 10 ; ++ dealer_shown){
                    cout << fixed << setprecision(2) << setw(6) << value[dealer_shown][1][player_sum][
                        policy[dealer_shown][1][player_sum]];
                }
                cout << endl;
            }
            cout << endl;
        }
        
        BlackjackPolicyLearnableDefault(){
            for (int dealer_shown = 1; dealer_shown <= 10; ++ dealer_shown){
                for (int player_ace = 0; player_ace <= 1; ++ player_ace){
                    for (int player_sum = 0; player_sum <= 21; ++ player_sum){
                        int& action = policy[dealer_shown][player_ace][player_sum];
                        action = player_sum >= 20 ? Blackjack::STICK : Blackjack::HIT;
                    }
                }
            }
        }
    private:
        // 11: dealer_shown (A-10); 
        // 2: player_usable_ace (true/false);
        // 22: player_sum (only need to consider 11-20, because HIT when sum<11 and STICK when sum=21 are always best action.)
        int policy[11][2][22];
        // 11:dealer_shown; 2:player_usable_ace; 22:player_sum; 2:action (0:HIT, 1:STICK)
        double value[11][2][22][2];
        int state_action_count[11][2][22][2];

        // record a episode sampled (only player's steps).
        struct EpisodeStep{
            // state: (dealer_shown, player_ace, player_sum)
            int dealer_shown;
            int player_ace;
            int player_sum;
            // the action taken at state
            int action;
            EpisodeStep(const Blackjack::State& state, int action){
                dealer_shown = state.dealer_shown;
                player_ace = int(state.player_ace);
                player_sum = state.player_sum;
                this->action = action;
            }
            EpisodeStep(){}
        };
        vector<EpisodeStep> episode; 

        void set_value_initial(){
            memset(value, 0, sizeof(value));
            memset(state_action_count, 0, sizeof(state_action_count));
        }
};

// Demonstrative play of player & dealer with default policy 
// int main(){
//     Blackjack env(false);
//     BlackjackPolicyDefault policy;
//     bool done;
//     while (true) {
//         done = false;    
//         env.reset();
//         while (not done){
//             Blackjack::State state = env.state();
//             int action = policy(state);
//             Blackjack::StepResult result = env.step(action);
//             done = result.done;
//         }
//         cout << endl;
//         this_thread::sleep_for(chrono::milliseconds(1000));
//     }
//     return 0;
// }

int main(){
    Blackjack env(false);
    BlackjackPolicyLearnableDefault policy;
    policy.update_value(env);
    policy.print_policy();
    return 0;
}