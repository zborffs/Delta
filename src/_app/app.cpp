#include "_app/app.hpp"

/**
 * resets the internal state of the App object to a new State and updates the context thereof to *this
 * @param state new AppState* object
 */
void App::transition_to(AppState* state) {
    state_.reset(state);
    state_->set_context(this);
}

void App::rt_loop() {
    int i = 0;
    std::chrono::milliseconds period{100};
    while (i < 1000 && !quit()) {
#ifdef JETSON_BUILD
#else
        auto start = std::chrono::high_resolution_clock::now();
#endif // JETSON_BUILD

        ++i;
        spdlog::get("delta_logger")->info("rt_loop({})", i);

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

bool App::quit() {
    std::lock_guard<std::mutex> guard(mu_quit_);
    spdlog::get("delta_logger")->info("Reading \"quit_\"");
    return quit_;
}

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

/**
 * interface function for gRPC server
 * @return some engine data
 */
int App::engine_data() {
    return 0;
}
