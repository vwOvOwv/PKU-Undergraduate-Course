#include <ctime>
#include <iostream>
#include <cstdlib>
#include <utility>
#include <algorithm>
#include <iostream>

using namespace std;

class Blackjack{
    public:
        bool verbose;
        // identities
        static const bool DEALER = false, PLAYER = true;
        // actions
        static const int HIT = 0, STICK = 1;
        static constexpr const char *CARD_NAME = " A23456789TJQK";
        struct State{
            // DEALER or PLAYER's turn
            bool turn;
            // if has usable ace then true
            bool player_ace, dealer_ace;
            // 0-21 or >21 (goes bust)
            int player_sum, dealer_sum;
            // dealer's shown card: [A,10]
            int dealer_shown;
        };
        struct StepResult {
            // state after the step
            State state;
            // reward of the step. winner gets 1, loser gets -1, tie or non-terminal step both get 0.
            double player_reward, dealer_reward;
            // whether the game terminates after the step
            bool done;
            StepResult(const State& state, double player_reward, double dealer_reward, bool done){
                this->state = state;
                this->player_reward = player_reward;
                this->dealer_reward = dealer_reward;
                this->done = done;
            }
            StepResult() {}
        };
        // restart the game with cards randomly allocated to player and dealer
        void reset(void);

        // restart the game with given initial state (observation of the player)
        void reset(int dealer_shown, bool player_ace, int player_sum);  
        
        State state(void) const {
            return _state;
        }
        // if sum > 21 then goes bust
        bool goes_bust(void) const {
            return _state.turn == PLAYER ? _state.player_sum > 21 : _state.dealer_sum > 21;
        }
        // action: HIT(0) or STICK(1)
        StepResult step(int action);

        Blackjack(bool verbose=false){
            this->verbose = verbose;
            srand(time(nullptr));
        }
    private:
        // natural means player wins or ties with only 2 cards which sums to 21.
        bool natural;
        // current state of the game
        State _state;
        // sample a card randomly from A to K. J,Q,Ks are regarded as 10.
        int sample_card(){
            return min(rand() % 13 + 1, 10); //[A,2,3,4,5,6,7,8,9,10,J,Q,K]
        }
        // sample a card and give it to player/dealer who HIT.
        void deal_card(int card);
};


void Blackjack::reset(){
    natural = true;
    _state.player_sum = 0;
    _state.dealer_sum = 0;
    _state.player_ace = false;
    _state.dealer_ace = false;
    _state.turn = DEALER;
    int card1 = sample_card(), card2 = sample_card();
    _state.dealer_shown = card1;
    deal_card(card1);
    deal_card(card2);
    if (verbose){
        cout << "Game restarted." << endl;
        cout << "Dealer allocated\n\tPublic: " << CARD_NAME[card1] 
            << "\n\tPrivate: " << CARD_NAME[card2] << endl;
        cout << "Dealer:\n\tSum: " << _state.dealer_sum 
            << "\n\tUsable ace: " << _state.dealer_ace << endl;  
    }            
    _state.turn = PLAYER;
    card1 = sample_card(); 
    card2 = sample_card();
    deal_card(card1);
    deal_card(card2);
    if (verbose){
        cout << "Player allocated\n\t" << 
            CARD_NAME[card1] << "\n\t" << CARD_NAME[card2] << endl;
        cout << "Player:\n\tSum:" << _state.player_sum 
            << "\n\tUsable ace: " << _state.player_ace << endl;
    }
}

void Blackjack::reset(int dealer_shown, bool player_ace, int player_sum){
    natural = true;
    _state.dealer_shown = dealer_shown;
    _state.player_ace = player_ace;
    _state.player_sum = player_sum;
    _state.turn = DEALER;
    int dealer_another_card = sample_card();
    _state.dealer_sum = dealer_shown + dealer_another_card;
    if (dealer_another_card == 1 or dealer_shown == 1){
        _state.dealer_sum += 10;
        _state.dealer_ace = true;
    } else {
        _state.dealer_ace = false;
    }
    _state.turn = PLAYER;
    if (verbose){
        cout << "<State fixed reset:> " << endl;
        cout << "Player" << endl;
        cout << "\n\tSum: " << _state.player_sum << "\n\tAce: " << _state.player_ace << endl;
        cout << "Dealer: " << endl;
        cout << "\n\tSum: " << _state.dealer_sum << "\n\tAce: " 
            << _state.dealer_ace << "\n\tShown: " << _state.dealer_shown << endl;
    }
}

Blackjack::StepResult Blackjack::step(int action){
    if (natural){
        if (_state.player_sum == 21){
            if (_state.dealer_sum == 21){
                if (verbose){
                    cout << "Natural tie." << endl;
                }
                return StepResult(_state, 0.0, 0.0, true);
            } else {
                if (verbose){
                    cout << "Player natural." << endl;
                }
                return StepResult(_state, 1.0, -1.0, true);
            }
        } else {
            natural = false;
        }
    }
    const char *identity = (_state.turn == PLAYER ? "Player" : "Dealer");
    if (action == HIT){  
        int card = sample_card();
        deal_card(card);
        int current_sum = _state.turn == PLAYER ? _state.player_sum : _state.dealer_sum;
        if (verbose){
            cout << identity << " Hit\n\t" << CARD_NAME[card] << endl;
            cout << "After hit: " << identity << "\n\tSum: " << current_sum
                << "\n\tUsable ace: " << (_state.turn == PLAYER ? 
                    _state.player_ace : _state.dealer_ace) << endl;
        }
        if (goes_bust()){
            double player_reward = _state.turn == PLAYER ? -1.0 : 1.0;
            if (verbose){
                cout << identity << " goes bust." << endl;
            }
            return StepResult(_state, player_reward, -player_reward, true);
        } else {
            return StepResult(_state, 0.0, 0.0, false);
        }
    } else {
        switch(_state.turn){
            case PLAYER:
                if (verbose){
                    cout << "Player stick at " << _state.player_sum << ".\nDealer's turn." << endl;
                }
                _state.turn = DEALER;
                return StepResult(_state, 0.0, 0.0, false);
            case DEALER:
                if (verbose){
                    cout << "Dealer stick at " << _state.dealer_sum << ".\nGame done." << endl;
                }
                if (_state.player_sum == _state.dealer_sum){
                    if (verbose){
                        cout << "Game tie, player_sum = dealer_sum = " << _state.player_sum << endl;
                    }
                    return StepResult(_state, 0.0, 0.0, true);
                } else {
                    double player_reward = _state.player_sum > _state.dealer_sum ? 1.0 : -1.0;
                    if (verbose){
                        cout << (player_reward > 0 ? 
                            "Player" : "Dealer") << " win.\n\tPlayer: " << _state.player_sum << "\n\tDealer: " 
                            << _state.dealer_sum << endl; 
                    }
                    return StepResult(_state, player_reward, -player_reward, true);
                }
        }
    }
}

void Blackjack::deal_card(int card){
    bool &useable_ace = _state.turn == PLAYER ? _state.player_ace : _state.dealer_ace;
    int &current_sum = _state.turn == PLAYER ? _state.player_sum : _state.dealer_sum;
    if (card == 1){
        if (current_sum + 11 <= 21){
            useable_ace = true;
            current_sum += 11;
        } else {
            current_sum += 1;
        }
    } else {
        current_sum += card;
        if (goes_bust() and useable_ace){
            useable_ace = false;
            current_sum -= 11;
            current_sum += 1;
        }
    }
}