#ifndef DELTA_RECOGNITION_HPP
#define DELTA_RECOGNITION_HPP

/// STL Includes
#include <vector>
#include <optional>
#include <random>

/// 3rd party includes
#include <opencv2/opencv.hpp>
#include <spdlog/spdlog.h>
#include <matplot/matplot.h>

/// Project Includes
#include "utils.hpp"

class Recognition {
public:
    virtual ~Recognition() = default;
    virtual std::optional<FEN> determine_position(Photo& photo) = 0;
};

class SegmentClassify : public Recognition {
private:
    static inline double PI{std::acos(-1)};
    static std::array<Photo, 64> segment(Photo& photo);
//    static std::vector<cv::Vec2f> hough_transform(Photo& photo);

public:
    SegmentClassify() {
        spdlog::get("delta_logger")->info("Created SegmentClassify object.");
    }

    ~SegmentClassify() {
        spdlog::get("delta_logger")->info("Destroyed SegmentClassify object.");
    }

    std::optional<FEN> determine_position(Photo& photo) override;
};

struct Neighbors {
    std::vector<std::pair<double, double>> accs;
    std::vector<unsigned> cardinality;
    std::unordered_map<cv::Point*, unsigned> graphs;
};

#endif //DELTA_RECOGNITION_HPP
