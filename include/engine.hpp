#ifndef DELTA_ENGINE_HPP
#define DELTA_ENGINE_HPP

/// STL includes
#include <memory>
#include <optional>
#include <string>

/// 3rd party includes
#include <yaml-cpp/yaml.h>
#include <spdlog/spdlog.h>

/// Cloak includes
#include "chess_move.hpp"

/// Internal includes
#include "utils.hpp"


class EngineInterface {
public:
    virtual ~EngineInterface() = default;
    virtual std::optional<ChessMove> best_move(const FEN& fen) = 0;
    virtual bool is_move_legal(const FEN& fen) = 0;
};

class UCIAdapter : public EngineInterface {
    int tt_size_;
    int threads_;
    std::vector<std::string> move_strings_;

public:
    explicit UCIAdapter() : tt_size_(16), threads_(1) {
        spdlog::get("delta_logger")->info("Created UCIAdapter object (tt_size: {}, threads: {})", tt_size_, threads_);

        // spin the sub-process running cloak
        // have the
    }
    ~UCIAdapter() {
        spdlog::get("delta_logger")->info("Destroyed UCIAdapter");
    }

    std::optional<ChessMove> best_move(const FEN& fen) override;
    bool is_move_legal(const FEN& fen) override;
    inline void set_tt_size(int tt_size) {
        tt_size_ = tt_size;
    }
    inline void set_threads(int threads) {
        threads_ = threads;
    }
};

class EngineBuilder {
public:
    virtual ~EngineBuilder() = default;
    virtual void reset() = 0;
    virtual void startup_engine() = 0;
    virtual std::unique_ptr<EngineInterface> get_product() = 0;
};

class UCIEngineBuilder : public EngineBuilder {
private:
    YAML::Node config_;
    std::unique_ptr<UCIAdapter> uci_engine_;

public:
    explicit UCIEngineBuilder(const YAML::Node& config) : config_(config), uci_engine_(std::make_unique<UCIAdapter>()) {}
    void reset() override;
    void startup_engine() override;
    std::unique_ptr<EngineInterface> get_product() override;
};

#endif //DELTA_ENGINE_HPP
