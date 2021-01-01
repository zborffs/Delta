#ifndef DELTA__USERVO_SG90_HPP
#define DELTA__USERVO_SG90_HPP

/// Third Party Includes
#include <spdlog/spdlog.h>

/// Project Includes
#include "motor.hpp"
#include "_hw/_hw_mock.hpp"

template <class THardware>
class uServoSG90 : public Motor<THardware> {
    inline static THardware hw_{};
    double x_{0.0};
public:
    explicit uServoSG90(double x) : x_(x) {
        spdlog::get("delta_logger")->info("Created uServoSG90 object (x: {})", x_);
    }
    ~uServoSG90() {
        spdlog::get("delta_logger")->info("Destroyed uServoSG90");
    }

    void move() override;
};

/**
 *
 * @tparam THardware
 */
template <class THardware>
void uServoSG90<THardware>::move() {
    hw_.speak();
}

#endif //DELTA__USERVO_SG90_HPP
