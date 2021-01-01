#ifndef DELTA__MOCK_CAM_HPP
#define DELTA__MOCK_CAM_HPP

/// Third-party includes
#include <spdlog/spdlog.h>
#include <opencv2/opencv.hpp>
#include <string>

/// Project Includes
#include "camera.hpp"

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
    cv::Mat take_picture() override;
};

template <class THardware>
cv::Mat MockCamera<THardware>::take_picture() {
    int n = ((num_) % 6) + 1; // 6 is number of images I have
    std::string f("../" + file_name_ + "img" + std::to_string(n) + ".jpg");
    num_ = ((++n) % 6);
    return cv::imread(f);
}


#endif //DELTA__MOCK_CAM_HPP
