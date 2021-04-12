#ifndef DELTA__APP_STATE_WAIT_HPP
#define DELTA__APP_STATE_WAIT_HPP

/// 3rd party includes
#include <spdlog/spdlog.h>

/// internal includes
#include "app.hpp"
#include "_app_state_recognition.hpp"


class AppStateWait : public AppState {
public:
    explicit AppStateWait() {
        spdlog::get("delta_logger")->info("Created AppStateWait");
    }

    ~AppStateWait() {
        spdlog::get("delta_logger")->info("Destroyed AppStateWait");
    }

    bool handle() override;
};


#endif // DELTA__APP_STATE_WAIT_HPP
