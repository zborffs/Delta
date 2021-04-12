#ifndef DELTA_HARDWARE_HPP
#define DELTA_HARDWARE_HPP

/// STL includes
#include <typeinfo>

/// 3rd party includes
#include <spdlog/spdlog.h>

class HWJetson {
public:
    HWJetson() = default;
//    void gpio_on(void* address);
//    void gpio_off(void* address);
//    void gpio_toggle(void* address);
//    void adc_on(void* address);
//    void adc_off(void* address);
//    void adc_toggle(void* address);
    void speak();
};

class HWMock {
public:
    HWMock() = default;
//    void gpio_on(void* address);
//    void gpio_off(void* address);
//    void gpio_toggle(void* address);
//    void adc_on(void* address);
//    void adc_off(void* address);
//    void adc_toggle(void* address);
    void speak();
};

#endif //DELTA_HARDWARE_HPP
