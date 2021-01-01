#include "_app/_app_state_shutdown.hpp"

bool AppStateShutdown::handle() {
    context_->quit_rt_loop();
    if (context_->rt_loop_thread_.joinable()) {
        context_->rt_loop_thread_.join();
    }
    return false;
}
