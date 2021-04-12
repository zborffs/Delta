#ifndef DELTA_STATE_ESTIMATOR_HPP
#define DELTA_STATE_ESTIMATOR_HPP

/// 3rd party includes
#include <spdlog/spdlog.h>

/// internal includes
#include "utils.hpp"

class StateEstimator {
public:
    virtual ~StateEstimator() = default;
    virtual EstimatedState estimate_state(const SensorReadings& sensor_readings) = 0;
};

class UnscentedKalmanFilter : public StateEstimator {
public:
    explicit UnscentedKalmanFilter() {
        spdlog::get("delta_logger")->info("Created UnscentedKalmanFilter");
    }

    ~UnscentedKalmanFilter() {
        spdlog::get("delta_logger")->info("Destroyed UnscentedKalmanFilter");
    }

    EstimatedState estimate_state(const SensorReadings& sensor_readings) override;
};

class ExtendedKalmanFilter : public StateEstimator {
public:
    explicit ExtendedKalmanFilter() {
        spdlog::get("delta_logger")->info("Created ExtendedKalmanFilter object.");
    }

    ~ExtendedKalmanFilter() {
        spdlog::get("delta_logger")->info("Destroyed ExtendedKalmanFilter object.");
    }

    EstimatedState estimate_state(const SensorReadings& sensor_readings) override;
};

#endif // DELTA_STATE_ESTIMATOR_HPP
