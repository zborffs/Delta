#ifndef DELTA__SIMPLE_PLAN_ALG_HPP
#define DELTA__SIMPLE_PLAN_ALG_HPP

/**
 * Most basic and logical route:
 * 1.) If the move is quiet: (1) position of piece, (2) position of to_sq, (3) home
 * 2.) Capture move or en passant: (1) position of captured, (2) bucket, (3) position of piece (4) goto sq (5) home
 * 3.) Castle: (1) position of king (2) goto king, (3) position of rook (4) goto rook (5) home
 * 4.) Promote: (1) position of pawn, (2) bucket, (3) position of queen (or whatever) (4) goto sq (5) home
 */

#include <string>
#include <spdlog/spdlog.h>
#include "plan_interface.hpp"
#include <optional>

class SimplePlanAlgorithm : public PlanInterface {
public:
    explicit SimplePlanAlgorithm() {
        spdlog::get("delta_logger")->info("Created SimplePlanAlgorithm");
    }
    ~SimplePlanAlgorithm() {
        spdlog::get("delta_logger")->info("Destroyed SimplePlanAlgorithm");
    }

    std::optional<int> plan_route(int best_move) override;
};

#endif // DELTA__SIMPLE_PLAN_ALG_HPP
