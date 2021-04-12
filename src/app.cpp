#include "app.hpp"

/**
 * resets the internal state of the App object to a new State and updates the context thereof to *this
 * @param state new AppState* object
 */
void App::transition_to(AppState* state) {
    state_.reset(state);
    state_->set_context(this);
}

/**
 * the real-time control loop responsible for estimating the state and computing the control signals from sensor
 * readings and reference signals
 */
void App::rt_loop() {
    int i = 0;
    std::chrono::milliseconds period{100};
    while (i < 1000 && !quit()) { // arbitrary upper limit, just a mock example
#ifdef JETSON_BUILD
#else
        auto start = std::chrono::high_resolution_clock::now();
#endif // JETSON_BUILD

        ++i;


#ifdef JETSON_BUILD
#else
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        if (elapsed < period) {
            std::this_thread::sleep_for(period - elapsed); // enforces 1kHz loop
        } else {
            spdlog::get("delta_logger")->warn("rt_loop slower than frequency of {} [kHz]", 1.0 / period.count());
        }

#endif // JETSON_BUILD
    }
}

/**
 * returns the value of the member variable quit_ subject to synchronization bugs
 * @return true if we should quit, false if we shouldn't quit
 */
bool App::quit() {
    std::lock_guard<std::mutex> guard(mu_quit_);
    spdlog::get("delta_logger")->info("Reading \"quit_\"");
    return quit_;
}

/**
 * sets the quit_ member variable to true so that the real-time loop quits
 */
void App::quit_rt_loop() {
    std::lock_guard<std::mutex> guard(mu_quit_);
    spdlog::get("delta_logger")->info("Quitting RT Loop!");
    quit_ = true;
}

/**
 * operator for starting App and making it a functor for multithreading purposes
 */
void App::operator()() {
    while (state_->handle());
}

Reference App::reference() {
    return reference_;
}

void App::set_reference(Reference& reference) {
    reference_ = reference;
}
