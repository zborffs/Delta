#include "_app_state_wait.hpp"

bool AppStateWait::handle() {
    /// wait for button to be pressed

    /// once button is pressed, return to recognition state
    context_->transition_to(new AppStateRecognition);
    return true; // return true if successful
}