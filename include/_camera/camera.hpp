#ifndef DELTA_CAMERA_HPP
#define DELTA_CAMERA_HPP

#include <typeinfo>

#include <opencv2/opencv.hpp>

template <class THardware>
class Camera {
public:
    virtual ~Camera() = default;
    virtual cv::Mat take_picture() = 0;
};

#endif // DELTA_CAMERA_HPP
