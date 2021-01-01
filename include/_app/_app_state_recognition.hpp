#ifndef DELTA__APP_STATE_RECOGNITION_HPP
#define DELTA__APP_STATE_RECOGNITION_HPP

/// Third-Party Includes
#include <spdlog/spdlog.h>

/// Project Includes
#include "app.hpp"
#include "_app_state_eng.hpp"
#include "_app_state_shutdown.hpp"

/**
 * The AppStateRecognition class is responsible for coloring the Application's functionality when in the "Recognition"
 * state.
 * - it transforms a null input into a (legal) chess position
 * - it is reached from either the Initialization state, or the Waiting state
 * - it makes use of the Recognition component and the Engine component
 * - if successful, it transitions to the Engine state
 */
class AppStateRecognition : public AppState {
public:
    explicit AppStateRecognition() {
        spdlog::get("delta_logger")->info("Created AppStateRecognition");
    }

    ~AppStateRecognition() {
        spdlog::get("delta_logger")->info("Destroyed AppStateRecognition");
    }
    bool handle() override;
};

#endif // DELTA__APP_STATE_RECOGNITION_HPP
