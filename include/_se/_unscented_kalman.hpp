#ifndef DELTA__UNSCENTED_KALMAN_HPP
#define DELTA__UNSCENTED_KALMAN_HPP

#include "state_estimator.hpp"
#include <spdlog/spdlog.h>

class UnscentedKalmanFilter : public StateEstimator {
public:
    double estimated_state() override;
    explicit UnscentedKalmanFilter() {
        spdlog::get("delta_logger")->info("Created UnscentedKalmanFilter");
    }
    ~UnscentedKalmanFilter() {
        spdlog::get("delta_logger")->info("Destroyed UnscentedKalmanFilter");
    }
};

#endif // DELTA__UNSCENTED_KALMAN_HPP
