#ifndef DELTA_HARDWARE_HPP
#define DELTA_HARDWARE_HPP

#include <typeinfo>
#include <spdlog/spdlog.h>

class HWJetson {
public:
    HWJetson() {
//        spdlog::get("delta_logger")->info("Created HWJetson object");
    }
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
    HWMock() {
//        spdlog::get("delta_logger")->info("Created HWMock object");
    }
//    void gpio_on(void* address);
//    void gpio_off(void* address);
//    void gpio_toggle(void* address);
//    void adc_on(void* address);
//    void adc_off(void* address);
//    void adc_toggle(void* address);
    void speak();
};

#endif //DELTA_HARDWARE_HPP
