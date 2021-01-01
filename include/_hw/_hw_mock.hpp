#ifndef DELTA__HW_MOCK_HPP
#define DELTA__HW_MOCK_HPP

#include <typeinfo>
#include <spdlog/spdlog.h>

class HWMock {
public:
    HWMock() = default;
//    HWMock() {
//        spdlog::get("delta_logger")->info("Constructed HWMock object");
//    }

    void gpio_on(void* address);
    void gpio_off(void* address);
    void gpio_toggle(void* address);
    void adc_on(void* address);
    void adc_off(void* address);
    void adc_toggle(void* address);
    void speak();
};

#endif // DELTA__HW_MOCK_HPP
