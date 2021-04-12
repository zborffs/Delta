#ifndef DELTA__APP_STATE_INIT_HPP
#define DELTA__APP_STATE_INIT_HPP

/// Third-Party Includes
#include <spdlog/spdlog.h>

/// Project Includes
#include "app.hpp"
#include "_app_state_recognition.hpp" // next state

/**
 * this class contains high-level logic for the initialization of the application
 * - always the first state
 * - is never returned to
 * - continues onto Recognition state
 * - makes use of component's builders to build complex component objects from configuration file data
 * - updates internal state of App object with component objects (Controllers, StateEstimators, etc.)
 */
class AppStateInit : public AppState {
private:
    void init_cameras();
    void init_imus();
    void init_motors();
    void init_model();
    void init_state_estimator();
    void init_engine();
    void init_plan();
    void init_controller();
    void init_recognition();
public:
    explicit AppStateInit() {
        spdlog::get("delta_logger")->info("Created AppStateInit.");
    }

    ~AppStateInit() {
        spdlog::get("delta_logger")->info("Destroyed AppStateInit.");
    }

    bool handle() override;
};

#endif // DELTA__APP_STATE_INIT_HPP