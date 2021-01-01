#ifndef DELTA_MOTOR_HPP
#define DELTA_MOTOR_HPP

#include <typeinfo>

template <class THardware>
class Motor {
public:
    virtual ~Motor() = default;
    virtual void move() = 0;
};

#endif // DELTA_MOTOR_HPP
