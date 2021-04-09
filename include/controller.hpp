#ifndef DELTA_CONTROLLER_HPP
#define DELTA_CONTROLLER_HPP

#include <vector>
#include <spdlog/spdlog.h>
#include "utils.hpp"

class Controller {
public:
    virtual ~Controller() = default;
    virtual MotorTorques actuate(const Reference& ref, const EstimatedState& state) = 0;
};

class GravityCompPID : public Controller {
public:
    GravityCompPID() {
        spdlog::get("delta_logger")->info("Created GravityCompPID object.");
    }
    virtual ~GravityCompPID() {
        spdlog::get("delta_logger")->info("Destroyed GravityCompPID object.");
    }

    MotorTorques actuate(const Reference& ref, const EstimatedState& state) override;
};

#endif //DELTA_CONTROLLER_HPP
