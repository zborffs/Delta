#ifndef DELTA__ADAFRUIT_PTC08_HPP
#define DELTA__ADAFRUIT_PTC08_HPP

/// stl includes
#include <typeinfo>

/// third-party includes
#include <spdlog/spdlog.h>
#include <opencv2/opencv.hpp>

/// Project includes
#include "camera.hpp"

template <class THardware>
class AdafruitPTC08 : public Camera<THardware> {
private:
    inline static THardware hw_{};
    double x_;

public:
    explicit AdafruitPTC08(double x = 0) : x_(x) {
        spdlog::get("delta_logger")->info("Created AdafruitPTC08 object (x: {})", x);
    }
    cv::Mat take_picture() override;
};

template <class THardware>
cv::Mat AdafruitPTC08<THardware>::take_picture() {
    hw_.speak();
    cv::Mat m;
    return m;
}

#endif // DELTA__ADAFRUIT_PTC08_HPP
