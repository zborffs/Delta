#ifndef DELTA__HILLCREST_FSM300_HPP
#define DELTA__HILLCREST_FSM300_HPP

#include "imu.hpp"
#include <typeinfo>
#include <spdlog/spdlog.h>

template <class THardware>
class HillcrestFSM300 : public IMU<THardware> {
private:
    double a_;
public:
    explicit HillcrestFSM300(double a) : a_(a) {
        spdlog::get("delta_logger")->info("Created HillcrestFSM300 object (a: {})", a);
    }
    ~HillcrestFSM300() {
        spdlog::get("delta_logger")->info("Destroyed HillcrestFSM300");
    }
    int get_acceleration() override;
};


template <class THardware>
int HillcrestFSM300<THardware>::get_acceleration() {
    return 0;
}

#endif //DELTA__HILLCREST_FSM300_HPP
