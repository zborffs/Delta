#ifndef DELTA__SIMPLE_DELTA_MODEL_HPP
#define DELTA__SIMPLE_DELTA_MODEL_HPP

#include "model.hpp"
#include <spdlog/spdlog.h>

class SimpleDeltaModel : public Model {
public:
    explicit SimpleDeltaModel() {
        spdlog::get("delta_logger")->info("Created SimpleDeltaModel");
    }
    ~SimpleDeltaModel() {
        spdlog::get("delta_logger")->info("Destroyed SimpleDeltaModel");
    }
    int step(int state_vector, int control_vector) override;
};

#endif // DELTA__SIMPLE_DELTA_MODEL_HPP
