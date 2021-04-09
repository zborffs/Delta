#include "path_plan.hpp"

std::optional<Reference> PlanBSplinesTrajectory::plan_path(ChessMove best_move) {
    spdlog::get("delta_logger")->info("Determining end-effector position values from best move and logic about the order of picking up and placing pieces.");
    spdlog::get("delta_logger")->info("Adding additional constraints so that the path moves over the pieces not through them.");
    spdlog::get("delta_logger")->info("Estimated the time it would take to get to each not point given constraints on motor actuation and how that translates to end-effector position");
    spdlog::get("delta_logger")->info("Evaluating the time splines at the knot points to build the M matrix");
    spdlog::get("delta_logger")->info("Inverting the M matrix by the end-effector position matrix to get the end-effector coefficients over the matrix");
    spdlog::get("delta_logger")->info("Sampling the spline at fixed time intervals corresponding to the duty cycle of the motors to determine the accerlation, velocity, and position at each time");
    spdlog::get("delta_logger")->info("Transforming such reference values into a single Reference object to be used by the control loop");
    return std::nullopt;
}