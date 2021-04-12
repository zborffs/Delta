#include "_app_state_recognition.hpp"

/**
 * the Recognition AppState transforms nothing->(legal) chess position
 * 1.) take a picture with each of the cameras
 * 2.) make a request of the recognition component to determine the position from the given photograph
 * 3.) Appraise the legality of the FEN by passing the FEN into the EngineInterface component
 * 4.) Transition to the AppStateEngine state if the move is legal
 * @return whether or not the handling function failed (false) or succeeded (true)
 */
bool AppStateRecognition::handle() {
    /// Take picture with each camera, try to determine the position from the camera's picture
    std::optional<FEN> fen{std::nullopt};
    for (auto & camera : context_->cameras_) {
        auto photo = camera->take_picture(); // take picture with camera
        fen = context_->recognition_->determine_position(photo); // try to get FEN from the photo

        // if we found the fen, then just quit
        if (fen.has_value()) {
            break;
        }
    }

    /// if the returned value is std::nullopt, then the Recognition algorithm failed to discern the position,
    /// log the failure and leave the state with a 'false' value to leave the app's 'main' loop
    if (!fen.has_value()) {
        spdlog::get("delta_logger")->error("Failed to determine position from photos. Quitting.");
        context_->transition_to(new AppStateShutdown);
        return true;
    }

    /// If the returned value is NOT std::nullopt, then the Recognition algorithm succeeded in discerning the position.
    /// Now, pass the FEN into the EngineInterface, to determine if the move is legal.
    bool is_legal = context_->engine_->is_move_legal(fen.value());

    /// If the returned value is false, then the move was illegal. Log the failure and leave the state with a 'false'
    /// value to leave the apps's 'main' loop
    if (!is_legal) {
        spdlog::get("delta_logger")->error(R"("Player's move was illegal. Quitting.")");
        context_->transition_to(new AppStateShutdown);
        return true;
    }

    /// If the returned value is true, then the move was legal. Transition to the next state, passing in the current
    /// position, then return true to continue along app's 'main' loop
    context_->transition_to(new AppStateEng(std::move(fen.value())));
    return true; // return true if successful
}