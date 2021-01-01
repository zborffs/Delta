#include "_app/_app_state_plan.hpp"

/**
 * the Plan AppState transforms a ChessMove best_move->std::vector<Vector3d> coordinates in the world frame
 * 1.)
 * @return
 */
bool AppStatePlan::handle() {
    /// ask the Plan component for the set of coordinates (in the world frame) to make this move
    std::optional<int> coordinates = context_->plan_->plan_route(best_move_);

    /// If the returned value is std::nullopt, then the Plan component failed to determine the coordinates. So log the
    /// failure and leave the state with a 'false' value to leave the app's 'main' loop
    if (!coordinates.has_value()) {
        spdlog::get("delta_logger")->error("Failed to determine coordinates from best_move. Quitting.");
        context_->transition_to(new AppStateShutdown);
        return true;
    }

    /// If the returned value is NOT std::nullopt, then the Plan component succeeded in determining the coordinates.
    /// Transition to the next state, passing coordinates, then return true to continue along  app's 'main' loop

    // push coordinates
    context_->transition_to(new AppStateWait);
    return true;
}