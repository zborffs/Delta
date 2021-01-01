#include "_control/_optimal.hpp"

void OptimalController::actuate(int reference, int est_state_vec) {
    int e = reference - est_state_vec;
    for (int i = 0; i < motors_.size(); i++) {
        motors_[i]->move();
    }
}
