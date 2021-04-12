#ifndef DELTA_BUTTON_HPP
#define DELTA_BUTTON_HPP

#include <spdlog/spdlog.h>

#include <typeinfo>

template <class THardware>
class Button {
protected:
    THardware hw_;
public:
    virtual ~Button() = default;
    virtual unsigned get_status() = 0;
    virtual void set_status(unsigned bit) = 0;
};

template <class THardware>
class CircuitButton : public Button<THardware> {
public:
    explicit CircuitButton() {
        spdlog::get("delta_logger")->info("Created CircuitButton.");
    }
    ~CircuitButton() {
        spdlog::get("delta_logger")->info("Destroyed CircuitButton.");
    }

    unsigned get_status() override;
    void set_status(unsigned bit) override;
};

template <class THardware>
unsigned CircuitButton<Thardware>::get_status() {
    return 0;
}

template <class THardware>
void CircuitButton<THardware>::set_status(unsigned bit) {
    bit ^= 1;
}

#endif //DELTA_BUTTON_HPP
