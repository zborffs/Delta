#ifndef DELTA_IMU_HPP
#define DELTA_IMU_HPP

/// STL includes
#include <typeinfo>

/// 3rd-party includes
#include <Eigen/Core>
#include <spdlog/spdlog.h>

template <class THardware>
class IMU {
protected:
    THardware hw_;
public:
    virtual ~IMU() = default;
    virtual Eigen::Matrix<double, 9, 1> get_data() = 0;
};

template <class THardware>
class HCrestFSM300 : public IMU<THardware> {
public:
    explicit HCrestFSM300() {
        spdlog::get("delta_logger")->info("Created HCrestFSM300 object");
    }
    ~HCrestFSM300() {
        spdlog::get("delta_logger")->info("Destroyed HillcrestFSM300 object");
    }

    Eigen::Matrix<double, 9, 1> get_data() override;
};

template <class THardware>
Eigen::Matrix<double, 9, 1> HCrestFSM300<THardware>::get_data() {
    return Eigen::Matrix<double, 9, 1>();
}

#endif //DELTA_IMU_HPP
