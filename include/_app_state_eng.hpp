#ifndef DELTA__APP_STATE_ENG_HPP
#define DELTA__APP_STATE_ENG_HPP

/// Third-party includes
#include <spdlog/spdlog.h>

/// Project includes
#include "app.hpp"
#include "_app_state_plan.hpp"
#include "_app_state_shutdown.hpp"

/**
 * The AppStateEng is responsible for coloring the Application's functionality when in the Engine state
 * - it transforms a std::string FEN into a ChessMove representing the best move from this position
 * - it is reached from the Recognition state
 * - it makes use of the Engine component
 * - if successful, it transitions to the Plan state
 */
class AppStateEng : public AppState {
    std::string fen_;

public:
    explicit AppStateEng(std::string&& fen) : fen_(fen) {
        spdlog::get("delta_logger")->info("Created AppStateEng (fen: {})", fen_);
    }

    ~AppStateEng() {
        spdlog::get("delta_logger")->info("Destroyed AppStateEng");
    }
    bool handle() override;
};

#endif // DELTA__APP_STATE_ENG_HPP
