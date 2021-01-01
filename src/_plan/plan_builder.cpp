#include "_plan/plan_builder.hpp"

void SimplePlanAlgorithmBuilder::reset() {
    simple_plan_alg_ = std::make_unique<SimplePlanAlgorithm>();
}

std::unique_ptr<PlanInterface> SimplePlanAlgorithmBuilder::get_product() {
    return std::move(simple_plan_alg_);
}
