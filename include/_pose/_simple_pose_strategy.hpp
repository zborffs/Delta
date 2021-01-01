#ifndef DELTA__SIMPLE_POSE_STRATEGY_HPP
#define DELTA__SIMPLE_POSE_STRATEGY_HPP

#include "pose.hpp"
#include <spdlog/spdlog.h>

class SimplePoseStrategy : public Pose {
public:
#ifdef JETSON_BUILD
    explicit SimplePoseStrategy(std::vector<std::shared_ptr<Camera<HWJetson>>>& cameras) {
        spdlog::get("delta_logger")->info("Created SimplePoseStrategy");
        for (const auto & camera : cameras) {
            cameras_.push_back(camera);
        }
    }
#else
    explicit SimplePoseStrategy(std::vector<std::shared_ptr<Camera<HWMock>>>& cameras) {
        spdlog::get("delta_logger")->info("Created SimplePoseStrategy");
        for (const auto & camera : cameras) {
            cameras_.push_back(camera);
        }
    }
#endif // JETSON_BUILD
    ~SimplePoseStrategy() {
        spdlog::get("delta_logger")->info("Destroyed SimplyPoseStrategy");
    }

    int position() override;
};

#endif //DELTA__SIMPLE_POSE_STRATEGY_HPP
