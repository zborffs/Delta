#ifndef DELTA__APP_STATE_PLAN_HPP
#define DELTA__APP_STATE_PLAN_HPP

/// third-party includes
#include <spdlog/spdlog.h>

/// project includes
#include "app.hpp"
#include "_app_state_shutdown.hpp"
#include "_app_state_wait.hpp"

/**
 * The AppStatePlan is responsible for coloring the Application's functionality when in the Plan state
 * - it transforms a ChessMove representing the best move into a series of coordinates in the world frame
 * - it is reached from the Engine state
 * - it makes use of the Plan component
 * - if successful, transitions to wait state
 */
class AppStatePlan : public AppState {
    int best_move_;
public:
    explicit AppStatePlan(int best_move) : best_move_(best_move) {
        spdlog::get("delta_logger")->info("Created AppStatePlan (best_move: {})", best_move_);
    }
    ~AppStatePlan() {
        spdlog::get("delta_logger")->info("Destroyed AppStatePlan");
    }
    bool handle() override;
};

#endif // DELTA__APP_STATE_PLAN_HPP
