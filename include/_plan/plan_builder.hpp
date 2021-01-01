#ifndef DELTA_PLAN_BUILDER_HPP
#define DELTA_PLAN_BUILDER_HPP

#include "plan_interface.hpp"
#include "_simple_plan_alg.hpp"
#include <yaml-cpp/yaml.h>
#include <memory>

class PlanBuilder {
public:
    virtual ~PlanBuilder() = default;
    virtual void reset() = 0;
    virtual std::unique_ptr<PlanInterface> get_product() = 0;
};

class SimplePlanAlgorithmBuilder : public PlanBuilder {
private:
    YAML::Node config_;
    std::unique_ptr<SimplePlanAlgorithm> simple_plan_alg_;

public:
    explicit SimplePlanAlgorithmBuilder(const YAML::Node& config) : config_(config), simple_plan_alg_(std::make_unique<SimplePlanAlgorithm>()) {}
    void reset() override;
    std::unique_ptr<PlanInterface> get_product() override;
};

#endif //DELTA_PLAN_BUILDER_HPP
