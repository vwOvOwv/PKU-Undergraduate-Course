#include <cstring>
#include <ctime>
#include <utility>
#include <vector>
#include "maze.hpp"

class MazePolicyBase{
    public:
        virtual int operator()(const MazeEnv::State& state) const = 0;
};

class MazePolicyQLearning : public MazePolicyBase{
    public:
        int operator()(const MazeEnv::State& state) const {
            int best_action = 0;
            double best_value = q[locate(state, 0)];
            double q_s_a;
            for (int action = 1; action < 4; ++ action){
                q_s_a = q[locate(state, action)];
                if (q_s_a > best_value){
                    best_value = q_s_a;
                    best_action = action;
                }
            }
            return best_action;
        }

        MazePolicyQLearning(const MazeEnv& e) : env(e) {
            epsilon = 0.1;
            alpha = 0.1;
            gamma = 0.95;
            q = new double[e.max_x * e.max_y * 4];
            srand(2022);
            for (int i = 0; i < e.max_x * e.max_y * 4; ++ i){
                q[i] = 1.0 / (rand() % (e.max_x * e.max_y) + 1);
            }
        }

        ~MazePolicyQLearning(){
            // env.~MazeEnv();
            delete []q;
        }

        void learn(int iter=10000, int verbose_freq=1){
            bool done;
            int action, next_action;
            double reward;
            int episode_step;
            MazeEnv::State state, next_state;
            MazeEnv::StepResult step_result;

            for (int i = 0; i < iter; ++ i){
                state = env.reset();
                done = false;
                episode_step = 0;
                while (not done){
                    action = epsilon_greedy(state);
                    step_result = env.step(action);
                    next_state = step_result.next_state;
                    reward = step_result.reward;
                    done = step_result.done;
                    ++ episode_step;
                    next_action = (*this)(next_state);
                    q[locate(state, action)] += alpha * (gamma * q[locate(next_state, next_action)] + reward - q[locate(state, action)]);
                    state = next_state;
                }
                if (i % verbose_freq == 0){
                    cout << "episode_step: " << episode_step << endl;
                }
            }
        }

        int epsilon_greedy(MazeEnv::State state) const {
            if (rand() % 100000 < epsilon * 100000) {
                return rand() % 4;
            }
            return (*this)(state);
        }

        inline int locate(MazeEnv::State state, int action) const {
            return state.second * env.max_x * 4 + state.first * 4 + action;
        }

        void print_policy() const {
            static const char action_vis[] = "<>v^";
            int action;
            MazeEnv::State state;
            for (int i = 0; i < env.max_y; ++ i){
                for (int j = 0; j < env.max_x; ++ j){
                    state = MazeEnv::State(j, i);
                    if (not env.is_valid_state(state)){
                        cout << "#";
                    } else if (env.is_goal_state(state)){
                        cout << "G";
                    } else {
                        action = (*this)(MazeEnv::State(j, i));
                        cout << action_vis[action];
                    }
                }
                cout << endl;
            }
            cout << endl;
        }

    private:
        MazeEnv env;
        double *q;
        double epsilon, alpha, gamma;
};

class MazePolicyDynaQ : public MazePolicyBase{
    public:
        int operator()(const MazeEnv::State& state) const {
            int best_action = 0;
            double best_value = q[locate(state, 0)];
            double q_s_a;
            for (int action = 1; action < 4; ++ action){
                q_s_a = q[locate(state, action)];
                if (q_s_a > best_value){
                    best_value = q_s_a;
                    best_action = action;
                }
            }
            return best_action;
        }

        MazePolicyDynaQ(const MazeEnv& e, int planning_steps) : env(e), planning_steps(planning_steps) {
            epsilon = 0.1;
            alpha = 0.1;
            gamma = 0.95;
            q = new double[e.max_x * e.max_y * 4];
            model = new pair<MazeEnv::State, double>[e.max_x * e.max_y * 4];
            srand(2022);
            for (int i = 0; i < e.max_x * e.max_y * 4; ++ i){
                q[i] = 1.0 / (rand() % (e.max_x * e.max_y) + 1);
                model[i].first = make_pair(-1, -1);
                model[i].second = -1;
            }
        }

        ~MazePolicyDynaQ(){
            // env.~MazeEnv();
            delete []q;
            delete []model;
        }

        void learn(int iter=10000, int verbose_freq=1){
            bool done;
            int action, next_action;
            double reward;
            int episode_step;
            MazeEnv::State state, next_state;
            MazeEnv::StepResult step_result;

            for (int i = 0; i < iter; ++i){
                state = env.reset();
                done = false;
                episode_step = 0;
                while (not done){
                    action = epsilon_greedy(state);
                    step_result = env.step(action);
                    next_state = step_result.next_state;
                    reward = step_result.reward;
                    done = step_result.done;
                    ++episode_step;
                    next_action = (*this)(next_state);
                    q[locate(state, action)] += alpha * (gamma * q[locate(
                        next_state, next_action)] + \
                         reward - q[locate(state, action)]);
                    model[locate(state, action)] = make_pair(next_state, reward);
                    if(find(visited_state_action.begin(), visited_state_action.end(), 
                    make_pair(state, action)) == visited_state_action.end())
                        visited_state_action.push_back(make_pair(state, action));
                    for(int j = 0; j < planning_steps; j++){
                        pair<MazeEnv::State, int> random_state_action = \
                        visited_state_action[rand() % visited_state_action.size()];

                        MazeEnv::State random_state = random_state_action.first;
                        int random_action = random_state_action.second;

                        // cout << random_state.first << random_state.second << action << endl;

                        pair<MazeEnv::State, double> next_state_reward = \
                        model[locate(random_state, random_action)];
                        
                        MazeEnv::State next_state = next_state_reward.first;
                        double reward = next_state_reward.second;
                        q[locate(random_state, random_action)] += \
                        alpha * (reward + gamma * q[locate(next_state, (*this)(next_state))] - q[locate(random_state, random_action)]);
                    }
                    state = next_state;
                }
                if (i % verbose_freq == 0){
                    cout << "episode_step: " << episode_step << endl;
                }
            }
        }

        int epsilon_greedy(MazeEnv::State state) const {
            if (rand() % 100000 < epsilon * 100000) {
                return rand() % 4;
            }
            return (*this)(state);
        }

        inline int locate(MazeEnv::State state, int action) const {
            return state.second * env.max_x * 4 + state.first * 4 + action;
        }

        void print_policy() const {
            static const char action_vis[] = "<>v^";
            int action;
            MazeEnv::State state;
            for (int i = 0; i < env.max_y; ++ i){
                for (int j = 0; j < env.max_x; ++ j){
                    state = MazeEnv::State(j, i);
                    if (not env.is_valid_state(state)){
                        cout << "#";
                    } else if (env.is_goal_state(state)){
                        cout << "G";
                    } else {
                        action = (*this)(MazeEnv::State(j, i));
                        cout << action_vis[action];
                    }
                }
                cout << endl;
            }
            cout << endl;
        }

    private:
        MazeEnv env;
        double *q;
        pair<MazeEnv::State, double> *model;
        vector<pair<MazeEnv::State, int>> visited_state_action;
        double epsilon, alpha, gamma;
        int planning_steps;
};

class MazePolicyDynaQPlus : public MazePolicyBase{
    public:
        int operator()(const MazeEnv::State& state) const {
            int best_action = 0;
            double best_value = q[locate(state, 0)];
            double q_s_a;
            for (int action = 1; action < 4; ++ action){
                q_s_a = q[locate(state, action)];
                if (q_s_a > best_value){
                    best_value = q_s_a;
                    best_action = action;
                }
            }
            return best_action;
        }

        MazePolicyDynaQPlus(const MazeEnv& e, int planning_steps, double k) : env(e), planning_steps(planning_steps), k(k) {
            epsilon = 0.1;
            alpha = 0.1;
            gamma = 0.95;
            q = new double[e.max_x * e.max_y * 4];
            model = new pair<MazeEnv::State, double>[e.max_x * e.max_y * 4];
            state_action_time_record = new int[e.max_x * e.max_y * 4];
            srand(2022);
            for (int i = 0; i < e.max_x * e.max_y * 4; ++ i){
                q[i] = 1.0 / (rand() % (e.max_x * e.max_y) + 1);
                model[i].first = make_pair(-1, -1);
                model[i].second = -1;
                state_action_time_record[i] = 0;
            }
        }

        ~MazePolicyDynaQPlus(){
            // env.~MazeEnv();
            delete []q;
            delete []model;
            delete []state_action_time_record;
        }

        void learn(int iter=10000, int verbose_freq=1){
            bool done;
            int action, next_action;
            double reward;
            int episode_step;
            MazeEnv::State state, next_state;
            MazeEnv::StepResult step_result;

            for (int i = 0; i < iter; ++i){
                state = env.reset();
                done = false;
                episode_step = 0;
                memset(state_action_time_record, 0, env.max_x * env.max_y * 4 * sizeof(int));
                while (not done){
                    action = epsilon_greedy(state);
                    step_result = env.step(action);
                    next_state = step_result.next_state;
                    reward = step_result.reward;
                    done = step_result.done;
                    ++episode_step;
                    reward += k * sqrt(episode_step - state_action_time_record[locate(state, action)]);

                    next_action = (*this)(next_state);
                    q[locate(state, action)] += alpha * (gamma * q[locate(
                        next_state, next_action)] + \
                         reward - q[locate(state, action)]);

                    state_action_time_record[locate(state, action)] = episode_step;
                    model[locate(state, action)] = make_pair(next_state, reward);
                    if(find(visited_state_action.begin(), visited_state_action.end(), 
                    make_pair(state, action)) == visited_state_action.end())
                        visited_state_action.push_back(make_pair(state, action));

                    for(int j = 0; j < planning_steps; j++){
                        pair<MazeEnv::State, int> random_state_action = \
                        visited_state_action[rand() % visited_state_action.size()];

                        MazeEnv::State random_state = random_state_action.first;
                        int random_action = random_state_action.second;

                        pair<MazeEnv::State, double> next_state_reward = \
                        model[locate(random_state, random_action)];
                        
                        MazeEnv::State next_state = next_state_reward.first;
                        double reward = next_state_reward.second + \
                            k * sqrt(episode_step + 1 - state_action_time_record[locate(next_state, next_action)]);
                        q[locate(random_state, random_action)] += \
                        alpha * (reward + gamma * q[locate(next_state, (*this)(next_state))] - q[locate(random_state, random_action)]);
                    }
                    state = next_state;
                }
                if (i % verbose_freq == 0){
                    cout << "episode_step: " << episode_step << endl;
                }
            }
        }

        int epsilon_greedy(MazeEnv::State state) const {
            if (rand() % 100000 < epsilon * 100000) {
                return rand() % 4;
            }
            return (*this)(state);
        }

        inline int locate(MazeEnv::State state, int action) const {
            return state.second * env.max_x * 4 + state.first * 4 + action;
        }

        void print_policy() const {
            static const char action_vis[] = "<>v^";
            int action;
            MazeEnv::State state;
            for (int i = 0; i < env.max_y; ++ i){
                for (int j = 0; j < env.max_x; ++ j){
                    state = MazeEnv::State(j, i);
                    if (not env.is_valid_state(state)){
                        cout << "#";
                    } else if (env.is_goal_state(state)){
                        cout << "G";
                    } else {
                        action = (*this)(MazeEnv::State(j, i));
                        cout << action_vis[action];
                    }
                }
                cout << endl;
            }
            cout << endl;
        }

    private:
        MazeEnv env;
        double *q;
        int *state_action_time_record;
        pair<MazeEnv::State, double> *model;
        vector<pair<MazeEnv::State, int>> visited_state_action;
        double epsilon, alpha, gamma, k;
        int planning_steps;
};

int main(){
    const int max_x = 9, max_y = 6;
    const int start_x = 0, start_y = 2;
    const int target_x = 8, target_y = 0;
    int maze[max_y][max_x] = {
        {0,0,0,0,0,0,0,1,0},
        {0,0,1,0,0,0,0,1,0},
        {0,0,1,0,0,0,0,1,0},
        {0,0,1,0,0,0,0,0,0},
        {0,0,0,0,0,1,0,0,0},
        {0,0,0,0,0,0,0,0,0}
    };
    MazeEnv env(maze, max_x, max_y, start_x, start_y, target_x, target_y);
    env.reset();
    MazePolicyQLearning QLearning_policy(env);
    QLearning_policy.learn(50, 2);
    cout << "Q-Learning:" << endl;
    QLearning_policy.print_policy();

    env.reset();
    MazePolicyDynaQ DynaQ_policy(env, 5);
    DynaQ_policy.learn(50, 2);
    cout << "DynaQ: (n_step = 5)" << endl;
    DynaQ_policy.print_policy();

    env.reset();
    MazePolicyDynaQPlus DynaQPlus_policy(env, 5, 1e-5);
    DynaQPlus_policy.learn(50, 2);
    cout << "DynaQ+ (n_step = 5, k = 1e-5):" << endl;
    DynaQPlus_policy.print_policy();
    return 0;
}