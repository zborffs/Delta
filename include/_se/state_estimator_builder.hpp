//
// Created by Zach Bortoff on 12/30/20.
//

#ifndef DELTA_STATE_ESTIMATOR_BUILDER_HPP
#define DELTA_STATE_ESTIMATOR_BUILDER_HPP

#include "state_estimator.hpp"
#include "_unscented_kalman.hpp"
#include "_extended_kalman.hpp"
#include "_pose/_simple_pose_strategy.hpp"
#include "_camera/camera.hpp"
#include "_camera/_adafruit_ptc08.hpp"
#include "_imu/_hillcrest_fsm300.hpp"
#include "_hw/_hw_jetson.hpp"
#include "_hw/_hw_mock.hpp"
#include <yaml-cpp/yaml.h>
#include <spdlog/spdlog.h>

class StateEstimatorBuilder {
public:
    virtual ~StateEstimatorBuilder() = default;
    virtual void reset() = 0;

#ifdef JETSON_BUILD
    virtual void build_pose(std::vector<std::shared_ptr<Camera<HWJetson> > >& cameras) = 0;
#else
    virtual void build_pose(std::vector<std::shared_ptr<Camera<HWMock> > >& cameras) = 0;
#endif // JETSON_BUILD
    virtual void build_imu() = 0;
    virtual std::unique_ptr<StateEstimator> get_product() = 0;
};

class UnscentedKalmanFilterBuilder : public StateEstimatorBuilder {
private:
    std::unique_ptr<UnscentedKalmanFilter> unscented_kf_;
    YAML::Node config_;

public:
    explicit UnscentedKalmanFilterBuilder(const YAML::Node& config) : unscented_kf_(std::make_unique<UnscentedKalmanFilter>()), config_(config) {}
    void reset() override;
#ifdef JETSON_BUILD
    void build_pose(std::vector<std::shared_ptr<Camera<HWJetson> > >& cameras) override;
#else
    void build_pose(std::vector<std::shared_ptr<Camera<HWMock> > >& cameras) override;
#endif // JETSON_BUILD
    void build_imu() override;
    std::unique_ptr<StateEstimator> get_product() override;
};

class ExtendedKalmanFilterBuilder : public StateEstimatorBuilder  {
private:
    std::unique_ptr<ExtendedKalmanFilter> extended_kf_;
    YAML::Node config_;

public:
    explicit ExtendedKalmanFilterBuilder(const YAML::Node& config) : extended_kf_(std::make_unique<ExtendedKalmanFilter>()), config_(config) {}
    void reset() override;
#ifdef JETSON_BUILD
    void build_pose(std::vector<std::shared_ptr<Camera<HWJetson> > >& cameras) override;
#else
    void build_pose(std::vector<std::shared_ptr<Camera<HWMock> > >& cameras) override;
#endif // JETSON_BUILD
    void build_imu() override;
    std::unique_ptr<StateEstimator> get_product() override;
};

#endif // DELTA_STATE_ESTIMATOR_BUILDER_HPP
