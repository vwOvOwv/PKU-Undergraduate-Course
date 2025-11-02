#include <ctime>
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
            env.~MazeEnv();
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
    MazePolicyQLearning policy(env);
    policy.learn();
    policy.print_policy();
    return 0;
}