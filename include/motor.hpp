#ifndef DELTA_MOTOR_HPP
#define DELTA_MOTOR_HPP

#include <typeinfo>

template <class THardware>
class Motor {
public:
    virtual ~Motor() = default;
    virtual void move(double abs_degree) = 0;
};

template <class THardware>
class ServoMG996R : public Motor<THardware> {
    inline static THardware hw_{};

public:
    explicit ServoMG996R() {
        spdlog::get("delta_logger")->info("Created ServoMG996R object.");
    }

    ~ServoMG996R() {
        spdlog::get("delta_logger")->info("Destroyed ServoMG996R object.");
    }

    void move(double abs_degree) override;
};

template <class THardware>
void ServoMG996R<THardware>::move(double abs_degree) {
    return;
}

#endif // DELTA_MOTOR_HPP
