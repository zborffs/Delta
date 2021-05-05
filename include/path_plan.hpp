#ifndef DELTA_PATH_PLAN_HPP
#define DELTA_PATH_PLAN_HPP

/// STL includes
#include <optional>

/// 3rd party includes
#include <spdlog/spdlog.h>

/// internal includes
#include "utils.hpp"
#include "bsplines.hpp"

/// Cloak includes
#include "chess_move.hpp"

class PathPlanner {
public:
    virtual ~PathPlanner() = default;
    virtual std::optional<Reference> plan_path(ChessMove best_move) = 0;
};

class PlanBSplinesTrajectory : public PathPlanner {
public:
    PlanBSplinesTrajectory() {
        spdlog::get("delta_logger")->info("Created PlanBSplinesTrajectory object.");
    }

    ~PlanBSplinesTrajectory() {
        spdlog::get("delta_logger")->info("Destroyed PlanBSplinesTrajectory object.");
    }

    std::optional<Reference> plan_path(ChessMove best_move) override;
};

#endif //DELTA_PATH_PLAN_HPP
