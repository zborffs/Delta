#include "state_estimator.hpp"

EstimatedState UnscentedKalmanFilter::estimate_state(const SensorReadings& sensor_readings) {
    return EstimatedState();
}

EstimatedState ExtendedKalmanFilter::estimate_state(const SensorReadings &sensor_readings) {
    return EstimatedState();
}