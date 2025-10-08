#include <ctime>
#include <iomanip>
#include <random>
#include <utility>
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

class JackCarRental{
    static poisson_distribution<int>
        request_1, request_2, return_1, return_2;
    public:
        static const int
            MAX_CAR_1,
            MAX_CAR_2,
            MOVE_LIMIT;
        static const double
            MOVE_COST,
            RENT_PRICE,
            MEAN_REQUEST_1,
            MEAN_REQUEST_2,
            MEAN_RETURN_1,
            MEAN_RETURN_2;
        typedef pair<int, int> State;
        bool verbose;
        State state(){
            return make_pair(car_1, car_2);
        }
        void set_state(int car_1, int car_2){
            if (verbose){
                cout << "State set to (" << car_1 << ", " << car_2 << ")" << endl;
            }
            this->car_1 = car_1;
            this->car_2 = car_2;
        }
        void reset(){
            if (verbose){
                cout << "Environment reset." << endl;
            }
            day = 0;
            car_1 = 0;
            car_2 = 0;
        }
        pair<State, double> step(int action){
            day ++;
            if (verbose){
                cout << "\nDay: " << day 
                    << " State: (" << car_1 << ", " << car_2 << ")" << endl;
            }
            double reward = state_transition(action);
            if (verbose){
                cout << "\tReward: " << reward << endl;
            }
            return make_pair(state(), reward);
        }
        int sample_action(){
            int action_low = max(-car_2, -MOVE_LIMIT);
            int action_high = min(car_1, MOVE_LIMIT);
            uniform_int_distribution<int> random_action(action_low, action_high);
            int action = random_action(e);
            if (verbose){
                cout << "\tAction " << action 
                    << " sampled from uniform[" << action_low << ", " << action_high << "]" << endl;
            }
            return action;
        }
        JackCarRental(int car_1=0, int car_2=0, bool verbose=false){
            this->day = 0;
            this->verbose = verbose;
            set_state(car_1, car_2);
            e.seed(time(nullptr));
        }
    private:
        int car_1, car_2, day;
        default_random_engine e;
        
        double state_transition(int action){
            // Move car at each morning
            car_1 = min(car_1 - action, MAX_CAR_1);
            car_2 = min(car_2 + action, MAX_CAR_2);
            double total_move_cost = abs(action) * MOVE_COST;
            if (verbose){
                cout << "\tMove: (" << -action << ", " << action 
                    << "), cost: " << total_move_cost << endl; 
                cout << "\tAfter movement, state: (" << car_1 << ", " << car_2 << ")" << endl;
            }
            // Rent car at each evening
            int req_1 = this->request_1(e);
            int req_2 = request_2(e);
            if (verbose){
                cout << "\tRental request: (" << req_1 << ", " << req_2 << ")" << endl; 
            } 
            int rent_1 = min(car_1, req_1);
            int rent_2 = min(car_2, req_2);
            double total_income = (rent_1 + rent_2) * RENT_PRICE;
            car_1 -= rent_1;
            car_2 -= rent_2;
            if (verbose){
                cout << "\tRent: (" << rent_1 << ", " << rent_2 
                    << "), income: " << total_income << endl;
                cout << "\tAfter rent, state: (" << car_1 << ", " << car_2 << ")" << endl;
            }
            int ret_1 = return_1(e);
            int ret_2 = return_2(e);
            if (verbose){
                cout << "\tCars to return: (" << ret_1 << ", " << ret_2 << ")" << endl;
            }
            // Cars return at the end of the day
            car_1 = min(car_1 + ret_1, MAX_CAR_1);
            car_2 = min(car_2 + ret_2, MAX_CAR_2);
            if (verbose){
                cout << "\tAfter return, state: (" << car_1 << ", " << car_2 << ")" << endl;
            }
            return total_income - total_move_cost;
        }
};

const int
    JackCarRental::MAX_CAR_1 = 20,
    JackCarRental::MAX_CAR_2 = 20,
    JackCarRental::MOVE_LIMIT = 5;
const double 
    JackCarRental::MOVE_COST = 2.0,
    JackCarRental::RENT_PRICE = 10.0,
    JackCarRental::MEAN_REQUEST_1 = 3.0,
    JackCarRental::MEAN_REQUEST_2 = 4.0,
    JackCarRental::MEAN_RETURN_1 = 3.0,
    JackCarRental::MEAN_RETURN_2 = 2.0;
poisson_distribution<int> 
    JackCarRental::request_1(JackCarRental::MEAN_REQUEST_1),
    JackCarRental::request_2(JackCarRental::MEAN_REQUEST_2),
    JackCarRental::return_1(JackCarRental::MEAN_RETURN_1),
    JackCarRental::return_2(JackCarRental::MEAN_RETURN_2);

double factorial(int n) {
    if (n <= 1) return 1;
    double result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}
double poisson_probability(double lambda, int k) {
    double numerator = pow(lambda, k) * exp(-lambda);
    double denominator = factorial(k);
    return numerator / denominator;
}
double gamma = 0.9;
double theta = 1e-4;
// unordered_map<int, double> value_tab;   // function Q
// unordered_map<int, int> policy;
vector<double> req_1_prob_tab(JackCarRental::MAX_CAR_1 + 2, 0);
vector<double> req_2_prob_tab(JackCarRental::MAX_CAR_2 + 2, 0);
vector<double> ret_1_prob_tab(JackCarRental::MAX_CAR_1 + 2, 0);
vector<double> ret_2_prob_tab(JackCarRental::MAX_CAR_2 + 2, 0);
double value_tab[JackCarRental::MAX_CAR_1 + 1][JackCarRental::MAX_CAR_2 + 1][2 * JackCarRental::MOVE_LIMIT + 1];
int policy[JackCarRental::MAX_CAR_1 + 1][JackCarRental::MAX_CAR_2 + 1];

int main(){
    // initialize value table and policy
    for (int car_1 = 0; car_1 <= JackCarRental::MAX_CAR_1; car_1++){
        for (int car_2 = 0; car_2 <= JackCarRental::MAX_CAR_2; car_2++){
            for (int action = -JackCarRental::MOVE_LIMIT; 
                action <= JackCarRental::MOVE_LIMIT; action++){
                value_tab[car_1][car_2][action + JackCarRental::MOVE_LIMIT] = 0.0; 
            }
            policy[car_1][car_2] = 0;
        }
    }

    // compute probabilities
    double req1_prob_sum = 0;
    double ret1_prob_sum = 0;
    int i = 0;
    for (;i <= JackCarRental::MAX_CAR_1; i++){
        double req_1_prob = poisson_probability(JackCarRental::MEAN_REQUEST_1, i);
        double ret_1_prob = poisson_probability(JackCarRental::MEAN_RETURN_1, i);
        req1_prob_sum += req_1_prob;
        ret1_prob_sum += ret_1_prob;
        req_1_prob_tab[i] = req_1_prob;
        ret_1_prob_tab[i] = ret_1_prob;
    }
    req_1_prob_tab[i] = 1 - req1_prob_sum;  // residual prob
    ret_1_prob_tab[i] = 1 - ret1_prob_sum;
    
    double req2_prob_sum = 0;
    double ret2_prob_sum = 0;
    i = 0;
    for (;i <= JackCarRental::MAX_CAR_2; i++){
        double req_2_prob = poisson_probability(JackCarRental::MEAN_REQUEST_2, i);
        double ret_2_prob = poisson_probability(JackCarRental::MEAN_RETURN_2, i);
        req2_prob_sum += req_2_prob;
        ret2_prob_sum += ret_2_prob;
        req_2_prob_tab[i] = req_2_prob;
        ret_2_prob_tab[i] = ret_2_prob;
    }
    req_2_prob_tab[i] = 1 - req2_prob_sum;  // residual prob
    ret_2_prob_tab[i] = 1 - ret2_prob_sum;

    int iter = 0;

    // policy iteration
    while (true){
        // policy evaluation
        cout << "Pi" << iter << ':' << endl;
        for (int car_1 = JackCarRental::MAX_CAR_1; car_1 >= 0; car_1--){
            for (int car_2 = 0; car_2 <= JackCarRental::MAX_CAR_2; car_2++){
                cout << setw(3) << policy[car_1][car_2];
            }
            cout << endl;
        }
        iter++;
        while (true){
            double delta = 0.0;
            double new_value_tab[JackCarRental::MAX_CAR_1 + 1]\
                                [JackCarRental::MAX_CAR_2 + 1]\
                                [2 * JackCarRental::MOVE_LIMIT + 1];
            // Note that mean reward of tomorrow is only determined by how many 
            // cars are left today. Since all returns and requirements are 
            // independent, first compute the expected value of Q(s', a'|s_mid), 
            // where s_mid is #cars at today's evening here.
            double expected_next_value_tab[JackCarRental::MAX_CAR_1 + 1][JackCarRental::MAX_CAR_2 + 1] = {0.0};
            for (int car_1_evening = 0; car_1_evening <= JackCarRental::MAX_CAR_1; car_1_evening++) {
                for (int car_2_evening = 0; car_2_evening <= JackCarRental::MAX_CAR_2; car_2_evening++) {
                    double expected_next_value = 0;
                    for (int ret_1 = 0; ret_1 <= JackCarRental::MAX_CAR_1 + 1; ++ret_1) {
                        for (int ret_2 = 0; ret_2 <= JackCarRental::MAX_CAR_2 + 1; ++ret_2) {
                            int next_car_1 = min(car_1_evening + ret_1, JackCarRental::MAX_CAR_1);
                            int next_car_2 = min(car_2_evening + ret_2, JackCarRental::MAX_CAR_2);
                            double prob = ret_1_prob_tab[ret_1] * ret_2_prob_tab[ret_2];
                            int next_action = policy[next_car_1][next_car_2];
                            double next_value = value_tab[next_car_1][next_car_2][next_action + JackCarRental::MOVE_LIMIT];
                            expected_next_value += prob * next_value;
                        }
                    }
                    expected_next_value_tab[car_1_evening][car_2_evening] = expected_next_value;
                }
            }
            for (int car_1 = 0; car_1 <= JackCarRental::MAX_CAR_1; car_1++){
                for (int car_2 = 0; car_2 <= JackCarRental::MAX_CAR_2; car_2++){
                    JackCarRental::State state = make_pair(car_1, car_2);
                    int action_low = max(-car_2, -JackCarRental::MOVE_LIMIT);
                    int action_high = min(car_1, JackCarRental::MOVE_LIMIT); 
                    for (int action = action_low; action <= action_high; action++){
                        int car_1_morning = min(car_1 - action, JackCarRental::MAX_CAR_1);
                        int car_2_morning = min(car_2 + action, JackCarRental::MAX_CAR_2);
                        double total_move_cost = abs(action) * JackCarRental::MOVE_COST;
                        double cur_value = value_tab[car_1][car_2][action + JackCarRental::MOVE_LIMIT];
                        double expected_value = 0.0;
                        for (int req_1 = 0; req_1 <= JackCarRental::MAX_CAR_1 + 1; req_1++){
                            for (int req_2 = 0; req_2 <= JackCarRental::MAX_CAR_2 + 1; req_2++){
                                int rent_1 = min(car_1_morning, req_1);
                                int rent_2 = min(car_2_morning, req_2);
                                double total_income = (rent_1 + rent_2) * JackCarRental::RENT_PRICE;
                                int car_1_evening = car_1_morning - rent_1;
                                int car_2_evening = car_2_morning - rent_2;
                                // for (int ret_1 = 0; ret_1 <= JackCarRental::MAX_CAR_1 + 1; ret_1++){
                                //     for (int ret_2 = 0; ret_2 <= JackCarRental::MAX_CAR_2 + 1; ret_2++){
                                //         int next_car_1 = min(car_1_evening + ret_1, JackCarRental::MAX_CAR_1);
                                //         int next_car_2 = min(car_2_evening + ret_2, JackCarRental::MAX_CAR_2);
                                //         int next_action = policy[next_car_1][next_car_2];
                                //         double prob = req_1_prob_tab[req_1] * req_2_prob_tab[req_2] * ret_1_prob_tab[ret_1] * ret_2_prob_tab[ret_2];
                                //         double reward = total_income - total_move_cost;
                                //         expected_value += prob * (reward + gamma * value_tab[next_car_1][next_car_2][next_action + JackCarRental::MOVE_LIMIT]);
                                //     }
                                // }
                                double reward = total_income - total_move_cost;
                                double prob_request = req_1_prob_tab[req_1] * req_2_prob_tab[req_2];
                                double future_value = expected_next_value_tab[car_1_evening][car_2_evening];
                                expected_value += prob_request * (reward + gamma * future_value);
                            }
                        }
                        new_value_tab[car_1][car_2][action + JackCarRental::MOVE_LIMIT] = expected_value;
                        delta = max(delta, abs(cur_value - expected_value));
                    }
                }
            }
            memcpy(value_tab, new_value_tab, sizeof(value_tab));
            // cout << "delta = " << delta << endl;
            if (delta < theta)
                break;
        }
        // policy improvement
        bool policy_diff = false;
        for (int car_1 = 0; car_1 <= JackCarRental::MAX_CAR_1; car_1++){
            for (int car_2 = 0; car_2 <= JackCarRental::MAX_CAR_2; car_2++){
                JackCarRental::State state = make_pair(car_1, car_2);
                int old_action = policy[car_1][car_2];
                int action_low = max(-car_2, -JackCarRental::MOVE_LIMIT);
                int action_high = min(car_1, JackCarRental::MOVE_LIMIT); 
                double max_reward = -1e10;
                int max_reward_action = 0;
                for (int action = action_low; action <= action_high; action++){
                    double value = value_tab[car_1][car_2][action + JackCarRental::MOVE_LIMIT];
                    if (value > max_reward){
                        max_reward = value;
                        max_reward_action = action;
                    }
                }
                policy[car_1][car_2] = max_reward_action;
                if (max_reward_action != old_action){
                    policy_diff = true;
                }
            }
        }
        if(!policy_diff)
            break;
    }
    cout << "Pi*:" << endl;
    for (int car_1 = JackCarRental::MAX_CAR_1; car_1 >= 0; car_1--){
        for (int car_2 = 0; car_2 <= JackCarRental::MAX_CAR_2; car_2++){
            cout << setw(3) << policy[car_1][car_2];
        }
        cout << endl;
    }
    return 0;
}