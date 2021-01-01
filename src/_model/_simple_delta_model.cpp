#include "_model/_simple_delta_model.hpp"

int SimpleDeltaModel::step(int state_vector, int control_vector) {
    int x = state_vector + control_vector;
    return 0;
}