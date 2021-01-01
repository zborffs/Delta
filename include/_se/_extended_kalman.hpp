#ifndef DELTA__EXTENDED_KALMAN_HPP
#define DELTA__EXTENDED_KALMAN_HPP

#include "state_estimator.hpp"
#include <spdlog/spdlog.h>

class ExtendedKalmanFilter : public StateEstimator {
public:
    double estimated_state() override;
    explicit ExtendedKalmanFilter() {
        spdlog::get("delta_logger")->info("Created ExtendedKalmanFilter");
    }
    ~ExtendedKalmanFilter() {
        spdlog::get("delta_logger")->info("Destroyed ExtendedKalmanFilter");
    }
};

#endif // DELTA__SE_EXTENDEDKALMAN_HPP
