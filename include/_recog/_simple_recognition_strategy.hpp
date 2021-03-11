#ifndef DELTA__SIMPLE_RECOGNITION_STRATEGY_HPP
#define DELTA__SIMPLE_RECOGNITION_STRATEGY_HPP

/// STL Includes
#include <memory>
#include <vector>
#include <string>
#include <cmath>

/// Third-Party Includes
#include <opencv2/opencv.hpp>
#include <matplot/matplot.h>

/// Project includes
#include "recognition.hpp"
#include "_hw/_hw_mock.hpp"
#include "_hw/_hw_jetson.hpp"
#include "_camera/camera.hpp"

class SimpleRecognitionStrategy : public Recognition {
private:
    static cv::Mat detect_edges(cv::Mat& input);
    static cv::Mat detect_edges2(cv::Mat& input);
    static cv::Mat detect_edges3(cv::Mat& input);
    static inline double PI{std::acos(-1)};
    static cv::Vec2f points_to_line(cv::Vec4f points);
public:
#ifdef JETSON_BUILD
    explicit SimpleRecognitionStrategy(std::vector<std::shared_ptr<Camera<HWJetson>>>& cameras) {
        spdlog::get("delta_logger")->info("Created SimpleRecognitionStrategy object");
        for (const auto & camera : cameras) {
            cameras_.push_back(camera);
        }
    }
#else
    explicit SimpleRecognitionStrategy(std::vector<std::shared_ptr<Camera<HWMock>>>& cameras) {
        spdlog::get("delta_logger")->info("Created SimpleRecognitionStrategy");
        for (const auto & camera : cameras) {
            cameras_.push_back(camera);
        }
    }
#endif // JETSON_BUILD
    ~SimpleRecognitionStrategy() {
        spdlog::get("delta_logger")->info("Destroyed SimpleRecognitionStrategy");
    }
    std::optional<std::string> position() override;
};

#endif // DELTA__SIMPLE_RECOGNITION_STRATEGY_HPP
