#ifndef DELTA_CONTROLLER_BUILDER_HPP
#define DELTA_CONTROLLER_BUILDER_HPP

/// STL Includes
#include <memory>

/// Third-Party Includes
#include <yaml-cpp/yaml.h>

/// Project includes
#include "controller.hpp"
#include "_linear.hpp"
#include "_optimal.hpp"
#include "_model/_simple_delta_model.hpp"
#include "_motor/_uservo_sg90.hpp"
#include "_hw/_hw_jetson.hpp"
#include "_hw/_hw_mock.hpp"

class ControllerBuilder {
public:
    virtual ~ControllerBuilder() = default;
    virtual std::unique_ptr<Controller> get_product() = 0;
    virtual void reset() = 0;
    virtual void build_model() = 0;
    virtual void build_motors() = 0;
};

class LinearControllerBuilder : public ControllerBuilder {
private:
    std::unique_ptr<LinearController> linear_controller_;
    YAML::Node config_;

public:
    explicit LinearControllerBuilder(const YAML::Node& config) : linear_controller_(std::make_unique<LinearController>()), config_(config) {}
    void reset() override;
    void build_model() override;
    void build_motors() override;
    std::unique_ptr<Controller> get_product() override;
};

class OptimalControllerBuilder : public ControllerBuilder {
private:
    std::unique_ptr<OptimalController> optimal_controller_;
    YAML::Node config_;

public:
    explicit OptimalControllerBuilder(const YAML::Node& config) : optimal_controller_(std::make_unique<OptimalController>()), config_(config) {}
    void reset() override;
    void build_model() override;
    void build_motors() override;
    std::unique_ptr<Controller> get_product() override;
};

#endif // DELTA_CONTROLLER_BUILDER_HPP
