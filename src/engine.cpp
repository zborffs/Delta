#include "engine.hpp"

void UCIEngineBuilder::reset() {
    uci_engine_ = std::make_unique<UCIAdapter>();
}

void UCIEngineBuilder::startup_engine() {
    if (!config_.IsDefined()) {
        return; // "Engine" tag not in YAML
    }

    if (config_["tt_size"].IsDefined()) {
        uci_engine_->set_tt_size(config_["tt_size"].as<int>());
    }

    if (config_["threads"].IsDefined()) {
        uci_engine_->set_threads(config_["threads"].as<int>());
    }
}

std::unique_ptr<EngineInterface> UCIEngineBuilder::get_product() {
    return std::move(uci_engine_);
}

std::optional<ChessMove> UCIAdapter::best_move(const FEN& fen) {
    return std::nullopt;
}

bool UCIAdapter::is_move_legal(const FEN& fen) {
    return false;
}
