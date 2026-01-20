#include <iostream>
#include <vector>

/******************* Environment Definition *******************/
class TicTacToe{
    public:
        static constexpr const int PLAYER_NONE = 0b00, PLAYER_X = 0b01, PLAYER_O = 0b10;
        static constexpr const char* PLAYER_NAME = "_XO#";

        bool verbose;
        
        struct Action{
            // x: column number, y: row number
            int x, y;
            // test whether action is in range [0,3)x[0,3)
            bool valid(void) const;
            Action(int, int);
            Action(void);
        };
        struct State{
            int turn;
            int board;

            // test whether the location is occupied 
            bool unoccupied(TicTacToe::Action) const;

            // test whether the board is fully occupied
            bool full(void) const;

            // place a piece
            void put(TicTacToe::Action);

            // return the player that place the piece
            int get_piece(TicTacToe::Action) const;
            
            // clear board, set turn to X
            void reset(void);

            // test whether last step player wins at current state
            bool test_win(void) const;

            // get current action space
            std::vector<Action> action_space(void) const;

            // output state
            void print(void) const;
            State(void);
            State(const State&);
        };
        
        State get_state(void) const;
        
        // clear state and history
        void reset(void);
        
        // cancel the last step taken
        bool step_back(void);
        
        bool step(Action);
        
        // find the winner of the current state
        // if there's no winner, return PLAYER_NONE
        int winner(void) const;
        
        // test whether the game is over (game ties or someone wins)
        bool done(void) const;
        
        // output game state
        void print(void) const;
        
        // verbose?
        TicTacToe(bool);
    private: 
        TicTacToe::State state;
        std::vector<TicTacToe::State> history;
};

/******************* Environment implementation *******************/

/***** Environment api *****/

TicTacToe::State TicTacToe::get_state() const {
    return state;
}

void TicTacToe::reset(){
    state.reset();
    history.clear();
    history.push_back(state);
    if (verbose){
        std::cout << "Game reset." << std::endl;
        print();
    }
}

bool TicTacToe::step_back(){
    if (history.size() <= 1) {
        if (verbose){
            std::cout << "Error: History empty, can not step back!" << std::endl;
        }
        return false;
    }
    history.pop_back();
    state = history.back();
    if (verbose){
        std::cout << "Step back to: " << std::endl;
        print();
    }
    return true;
}
bool TicTacToe::step(TicTacToe::Action action){
    int x = action.x, y = action.y;
    if (not action.valid()){
        if (verbose){
            std::cout << "Error: Action (" << x << "," << y << ") out of range!" << std::endl;
        }
        return false;
    }
    if (not state.unoccupied(action)){
        if (verbose){
            std::cout << "Error: Location (" << x << "," << y << ") occupied with " 
                << TicTacToe::PLAYER_NAME[state.get_piece(action)] << "!" << std::endl;
        }
        return false;
    }
    state.put(action);
    history.push_back(state);
    if (verbose){
        std::cout << "Action (" << x << "," << y << ") taken." << std::endl;
        print(); 
    }
    return true;
}

int TicTacToe::winner() const {
    if (state.test_win()){
        int winner = 0b11 - state.turn;
        if (verbose){
            std::cout << "Winner: " << TicTacToe::PLAYER_NAME[winner] << std::endl << std::endl;
        }
        return winner;
    }
    if (verbose){
        std::cout << "Winner not found." << std::endl << std::endl;
    }
    return TicTacToe::PLAYER_NONE;
}

bool TicTacToe::done() const {
    return winner() != TicTacToe::PLAYER_NONE or state.full();
}

void TicTacToe::print() const {
    std::cout << "Board: " << std::endl;
    state.print();
}

TicTacToe::TicTacToe(bool verbose){
    this->verbose = verbose;
    reset();
}

/***** State api *****/

bool TicTacToe::State::unoccupied(TicTacToe::Action action) const {
    return get_piece(action) == TicTacToe::PLAYER_NONE;
}

bool TicTacToe::State::full() const {
    for (int i = 0; i < 9; ++ i){
        if (((board >> (i * 2)) & 0b11) == 0){
            return false;
        }
    }
    return true;
}

void TicTacToe::State::put(TicTacToe::Action action){
    int p = (action.y * 3 + action.x) * 2;
    board |= (turn << p);
    turn = 0b11 - turn;
}

int TicTacToe::State::get_piece(TicTacToe::Action action) const {
    int p = (action.y * 3 + action.x) * 2;
    return (board >> p) & 0b11;
}

void TicTacToe::State::reset(){
    board = 0;
    turn = TicTacToe::PLAYER_X;
}

bool TicTacToe::State::test_win() const {
    int mask;
    int last_piece = 0b11 - turn;
    // row
    mask = last_piece | (last_piece << 2) | (last_piece << 4);
    for (int i = 0; i < 3; ++ i){
        if ((board & mask) == mask){
            return true;
        }
        mask <<= 6;
    }
    // column;
    mask = last_piece | (last_piece << 6) | (last_piece << 12);
    for (int i = 0; i < 3; ++ i){
        if ((board & mask) == mask){
            return true;
        }
        mask <<= 2;
    }
    // diagonals;
    mask = last_piece | (last_piece << 8) | (last_piece << 16);
    if ((board & mask) == mask){
        return true;
    }
    mask = (last_piece << 4) | (last_piece << 8) | (last_piece << 12);
    if ((board & mask) == mask){
        return true;
    }
    return false;
}

std::vector<TicTacToe::Action> TicTacToe::State::action_space() const {
    std::vector<TicTacToe::Action> actions;
    for (int y = 0; y < 3; ++ y){
        for (int x = 0; x < 3; ++ x){
            Action action(y, x);
            if (unoccupied(action)){
                actions.push_back(action);
            }
        }
    }
    return actions;
}

void TicTacToe::State::print() const {
    for (int y = 0; y < 3; ++ y){
        for (int x = 0; x < 3; ++ x){
            std::cout << "\t" << TicTacToe::PLAYER_NAME[get_piece(TicTacToe::Action(y, x))];
        }
        std::cout << std::endl;
    }
    std::cout << "Next turn: " << TicTacToe::PLAYER_NAME[turn] << std::endl << std::endl;
}

TicTacToe::State::State(){
    reset();
}

TicTacToe::State::State(const TicTacToe::State& _state){
    board = _state.board;
    turn = _state.turn;
}

/***** Action api *****/

bool TicTacToe::Action::valid() const {
    return x >= 0 and x < 3 and y >= 0 and y < 3;
}

TicTacToe::Action::Action(int x, int y) : x(x), y(y) {}
TicTacToe::Action::Action() : x(0), y(0) {}