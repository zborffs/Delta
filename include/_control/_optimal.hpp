#ifndef DELTA__OPTIMAL_HPP
#define DELTA__OPTIMAL_HPP

#include <spdlog/spdlog.h>
#include "controller.hpp"
#include "_model/model.hpp"

class OptimalController : public Controller {
public:
    explicit OptimalController() {
        spdlog::get("delta_logger")->info("Created OptimalController");
    }
    ~OptimalController() {
        spdlog::get("delta_logger")->info("Destroyed OptimalController");
    }

    void actuate(int reference, int est_state_vec) override;
};

#endif //DELTA__OPTIMAL_HPP
