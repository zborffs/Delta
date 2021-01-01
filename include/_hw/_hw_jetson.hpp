#ifndef DELTA__HW_JETSON_HPP
#define DELTA__HW_JETSON_HPP

#include <typeinfo>
#include <spdlog/spdlog.h>

class HWJetson {
public:
    HWJetson() = default;
    void gpio_on(void* address);
    void gpio_off(void* address);
    void gpio_toggle(void* address);
    void adc_on(void* address);
    void adc_off(void* address);
    void adc_toggle(void* address);
    void speak();
};

#endif // DELTA__HW_JETSON_HPP
