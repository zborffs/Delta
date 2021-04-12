#ifndef DELTA_CAMERA_HPP
#define DELTA_CAMERA_HPP

/// project includes
#include <string>

/// Third-party includes
#include <spdlog/spdlog.h>
#include <opencv2/opencv.hpp>

/// Project Includes
#include "utils.hpp"

template <class THardware>
class Camera {
public:
    virtual ~Camera() = default;
    virtual Photo take_picture() = 0;
};

template <class THardware>
class MockCamera : public Camera<THardware> {
    // no THardware object
    std::string file_name_;
    int num_{0};
public:
    explicit MockCamera(std::string& file_name) : file_name_(file_name) {
        spdlog::get("delta_logger")->info("Created MockCamera object (file_name: {}", file_name_);
    }
    ~MockCamera() {
        spdlog::get("delta_logger")->info("Destroyed MockCamera");
    }
    Photo take_picture() override;
};

template <class THardware>
Photo MockCamera<THardware>::take_picture() {
    int n = ((num_) % 6) + 1; // 6 is number of images I have
    std::string f("../" + file_name_ + "img" + std::to_string(n) + ".jpg");
    num_ = ((++n) % 6);
    return cv::imread(f);
}

template <class THardware>
class AdafruitPTC08 : public Camera<THardware> {
private:
    inline static THardware hw_{};

public:
    explicit AdafruitPTC08() {
        spdlog::get("delta_logger")->info("Created AdafruitPTC08 object.");
    }
    cv::Mat take_picture() override;
};

template <class THardware>
Photo AdafruitPTC08<THardware>::take_picture() {
    hw_.speak();
    Photo m;
    return m;
}

#endif //DELTA_CAMERA_HPP
