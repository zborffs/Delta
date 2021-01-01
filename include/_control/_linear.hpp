#ifndef DELTA__LINEAR_HPP
#define DELTA__LINEAR_HPP

#include "controller.hpp"
#include <spdlog/spdlog.h>

class LinearController : public Controller {
private:
    double P_, I_, D_;

public:
    explicit LinearController() : P_(0), I_(0), D_(0) {
        spdlog::get("delta_logger")->info("Creating LinearController object (P: {}, I: {}, D: {})", P_, I_, D_);
    }
    void actuate(int reference, int est_state_vec) override;
};

#endif //DELTA__LINEAR_HPP
