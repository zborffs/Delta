#include "hardware.hpp"

void HWJetson::speak() {
    spdlog::get("delta_logger")->info("HWJetson says POOP");
}

void HWMock::speak() {
    spdlog::get("delta_logger")->info("HWMock says LOOP");
}