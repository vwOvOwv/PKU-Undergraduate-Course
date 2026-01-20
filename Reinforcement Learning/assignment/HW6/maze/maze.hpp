#include <iostream>
#include <cstring>
#include <utility>
#include <cassert>
#include <vector>
#include <cstdlib>

using namespace std;

class MazeEnv{
    public:
        const static int EMPTY=0, WALL=1;
        const static int LEFT=0, RIGHT=1, UP=2, DOWN=3;
        
        const int max_x, max_y;
        const int start_x, start_y, goal_x, goal_y;
        const double step_reward, goal_reward;

        typedef int* Maze;
        typedef pair<int, int> State;
        typedef struct {State next_state; double reward; bool done;} StepResult;
        
        MazeEnv(void* m, 
            int m_x, int m_y, 
            int s_x, int s_y, 
            int g_x, int g_y, 
            double step_r=0.0, double goal_r=1.0) :
            max_x(m_x), max_y(m_y), 
            start_x(s_x), start_y(s_y), 
            goal_x(g_x), goal_y(g_y), 
            step_reward(step_r), goal_reward(goal_r){
            maze = new int[m_x * m_y];
            memcpy(maze, m, sizeof(int) * m_x * m_y);
            assert(is_valid_state(State(s_x, s_y)));
            assert(is_valid_state(State(g_x, g_y)));
        }
        MazeEnv(const MazeEnv& m): MazeEnv(
            m.maze,
            m.max_x, m.max_y,
            m.start_x, m.start_y,
            m.goal_x, m.goal_y,
            m.step_reward, m.goal_reward
        ){}

        ~MazeEnv(){
            delete []maze;
        }

        inline int locate(int x, int y) const {
            return y * max_x + x;
        }

        State state() const {
            return make_pair(x, y);
        }

        bool is_valid_state(const State& state) const {
            return state.first >= 0 and state.first < max_x
                and state.second >= 0 and state.second < max_y
                and maze[locate(state.first, state.second)] == EMPTY;
        }

        inline bool is_goal_state(const State& state) const {
            return state.first == goal_x and state.second == goal_y;
        }
        
        inline bool is_start_state(const State& state) const {
            return state.first == start_x and state.second == start_y;
        }
        
        bool done() const {
            return is_goal_state(state());
        }

        StepResult step(int action){
            int new_x = x, new_y = y;
            switch(action){
                case LEFT:
                    -- new_x;
                    break;
                case RIGHT:
                    ++ new_x;
                    break;
                case UP:
                    ++ new_y;
                    break;
                case DOWN:
                    -- new_y;
                    break;
            }
            new_x = max(0, new_x);
            new_x = min(new_x, max_x-1);
            new_y = max(0, new_y);
            new_y = min(new_y, max_y-1);
            if (maze[locate(new_x, new_y)] == WALL){
                new_x = x;
                new_y = y;
            }
            x = new_x;
            y = new_y;
            bool is_done = done();
            StepResult result = {state(), is_done ? goal_reward : step_reward, is_done};
            return result;
        }
        
        State reset(){
            x = start_x;
            y = start_y;
            return state();
        }
        
        void set_state(const State& state){
            assert(is_valid_state(state));
            x = state.first;
            y = state.second;
        }

        void render() const {
            print_maze();
        }

    private:
        Maze maze;
        int x, y;

        void print_maze() const {
            for (int i = 0; i < max_y; ++ i){
                for (int j = 0; j < max_x; ++ j){
                    if (i == y and j == x){
                        cout << "*";
                    } else if (is_start_state(State(j, i))){
                        cout << "S";
                    } else if (is_goal_state(State(j, i))){
                        cout << "G";
                    } else {
                        cout << maze[locate(j, i)];
                    }
                }
                cout << endl;
            }
            cout << endl;
        }
};