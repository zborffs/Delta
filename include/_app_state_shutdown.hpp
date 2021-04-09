#ifndef DELTA__APP_STATE_SHUTDOWN_HPP
#define DELTA__APP_STATE_SHUTDOWN_HPP

#include "app.hpp"

class AppStateShutdown : public AppState {
public:
    explicit AppStateShutdown() {
        spdlog::get("delta_logger")->info("Created AppStateShutdown");
    }
    ~AppStateShutdown() {
        spdlog::get("delta_logger")->info("Destroyed AppStateShutdown");
        if (context_->rt_loop_thread_.joinable()) {
            context_->rt_loop_thread_.join();
        }
    }
    bool handle() override;
};

#endif // DELTA__APP_STATE_SHUTDOWN_HPP
