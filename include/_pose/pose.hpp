#ifndef DELTA__POSE_HPP
#define DELTA__POSE_HPP

#include <memory>
#include <vector>
#include <memory>
#include "_camera/camera.hpp"
#include "_hw/_hw_jetson.hpp"
#include "_hw/_hw_mock.hpp"

class Pose {
protected:
#ifdef JETSON_BUILD
    std::vector<std::shared_ptr<Camera<HWJetson> > > cameras_;
#else
    std::vector<std::shared_ptr<Camera<HWMock> > > cameras_;
#endif // JETSON_BUILD

public:
    virtual ~Pose() = default;
    virtual int position() = 0;

#ifdef JETSON_BUILD
    inline void push_camera(std::unique_ptr<Camera<HWJetson> > camera) {
        cameras_.emplace_back(std::move(camera));
    }
#else
    inline void push_camera(std::shared_ptr<Camera<HWMock> > camera) {
        cameras_.emplace_back(std::move(camera));
    }
#endif // JETSON_BUILD
};

#endif //DELTA__POSE_HPP
