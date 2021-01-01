#include "_control/_linear.hpp"

void LinearController::actuate(int reference, int est_state_vec) {
    auto e = reference - est_state_vec;
}
