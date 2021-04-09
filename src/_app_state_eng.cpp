#include "_app_state_eng.hpp"

/**
 * the Engine AppState transforms a std::string FEN->ChessMove best_move
 * 1.) Request the best move from the Engine component
 * 2.) Transition to the AppStatePlan state if the best move was determined
 * @return whether or not the handling function failed (false) or succeeded (true)
 */
bool AppStateEng::handle() {
    /// ask the Engine component for the best move from this position
    std::optional<ChessMove> best_move = context_->engine_->best_move(fen_);

    /// if the returned value is std::nullopt, then the Engine component failed to determine the best move. So, log the
    /// failure and leave the state with a 'false' value to leave the app's 'main' loop
    if (!best_move.has_value()) {
        spdlog::get("delta_logger")->error("Failed to determine best move from position. Quitting...");
        context_->transition_to(new AppStateShutdown);
        return true;
    }

    /// If the returned value is NOT std::nullopt, then the Engine component succeeded in determining the best move.
    /// Transition to the next state, passing in the best_move, then return true to continue along app's 'main' loop.
    context_->transition_to(new AppStatePlan(best_move.value()));
    return true;
}