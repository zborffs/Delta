#ifndef DELTA_RECOGNITION_HPP
#define DELTA_RECOGNITION_HPP

/// STL Includes
#include <vector>
#include <optional>
#include <random>

/// 3rd party includes
#include <opencv2/opencv.hpp>
#include <spdlog/spdlog.h>

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

public:
    SegmentClassify() {
        spdlog::get("delta_logger")->info("Created SegmentClassify object.");
    }

    ~SegmentClassify() {
        spdlog::get("delta_logger")->info("Destroyed SegmentClassify object.");
    }

    std::optional<FEN> determine_position(Photo& photo) override;
};

#endif //DELTA_RECOGNITION_HPP
