#ifndef DELTA_MODEL_HPP
#define DELTA_MODEL_HPP

#include <spdlog/spdlog.h>
#include "utils.hpp"

// we literally need a differential equations solver?

class ModelBuilder {

};

class LagrangianDeltaRobotModelBuilder : public ModelBuilder {

};

class Model {
public:
    virtual ~Model() = default;
    virtual State solve(State state, MotorTorques torques) = 0;
};

class LagrangianDeltaRobot : public Model {
public:
    LagrangianDeltaRobot() {
        spdlog::get("delta_logger")->info("Created LagrangianDeltaRobot object.");
    }

    ~LagrangianDeltaRobot() {
        spdlog::get("delta_logger")->info("Destroyed LagrangianDeltaRobot object.");
    }

    State solve(State state, MotorTorques torques) override;
};

#endif //DELTA_MODEL_HPP
