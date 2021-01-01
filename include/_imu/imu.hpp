#ifndef DELTA_IMU_HPP
#define DELTA_IMU_HPP

#include <typeinfo>

template <class THardware>
class IMU {
protected:
    THardware hw_;
    double x_{0.0};
public:
    virtual ~IMU() = default;
    virtual int get_acceleration() = 0;
};

#endif //DELTA_IMU_HPP
