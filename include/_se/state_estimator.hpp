#ifndef DELTA_STATE_ESTIMATOR_HPP
#define DELTA_STATE_ESTIMATOR_HPP

#include "_pose/pose.hpp"
#include "_imu/imu.hpp"

class StateEstimator {
    std::unique_ptr<Pose> pose_;
#ifdef JETSON_BUILD
    std::vector<std::unique_ptr<IMU<HWJetson> > > imus_;
#else
    std::vector<std::unique_ptr<IMU<HWMock> > > imus_;
#endif // JETSON_BUILD
public:
    virtual ~StateEstimator() = default;
    virtual double estimated_state() = 0;
    inline void set_pose(std::unique_ptr<Pose> pose) {
        pose_ = std::move(pose);
    }

#ifdef JETSON_BUILD
    inline void push_imu(std::unique_ptr<IMU<HWJetson> > imu) {
        imus_.emplace_back(std::move(imu));
    }


#else
    inline void push_imu(std::unique_ptr<IMU<HWMock> > imu) {
        imus_.emplace_back(std::move(imu));
    }
#endif // JETSON_BUILD
};

#endif // DELTA_STATE_ESTIMATOR_HPP
