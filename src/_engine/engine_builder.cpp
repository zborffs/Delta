#include "_engine/engine_builder.hpp"

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

    if (config_["book"].IsDefined()) {
        uci_engine_->set_book(config_["book"].as<bool>());
    }
}

std::unique_ptr<EngineInterface> UCIEngineBuilder::get_product() {
    return std::move(uci_engine_);
}
