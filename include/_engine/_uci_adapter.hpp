#ifndef DELTA__UCI_ADAPTER_HPP
#define DELTA__UCI_ADAPTER_HPP

/// STL includes
#include <string>
#include <optional>

/// Third-party includes
#include <spdlog/spdlog.h>

/// project includes
#include "engine_interface.hpp"

class UCIAdapter : public EngineInterface {
    int tt_size_;
    int threads_;
    bool book_;

public:
    explicit UCIAdapter() : tt_size_(16), threads_(1), book_(false) {
        spdlog::get("delta_logger")->info("Created UCIAdapter object (tt_size: {}, threads: {}, book: {})", tt_size_, threads_, book_);
    }
    ~UCIAdapter() {
        spdlog::get("delta_logger")->info("Destroyed UCIAdapter");
    }

    std::optional<int> best_move(const std::string& fen) override;
    bool is_move_legal(const std::string& fen) override;
    inline void set_tt_size(int tt_size) {
        tt_size_ = tt_size;
    }
    inline void set_threads(int threads) {
        threads_ = threads;
    }
    inline void set_book(int book) {
        book_ = book;
    }
};

#endif //DELTA__UCI_ADAPTER_HPP
