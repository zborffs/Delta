#ifndef DELTA_REFERENCE_HPP
#define DELTA_REFERENCE_HPP

#include <Eigen/Core>
#include <opencv2/core.hpp>


using FEN = std::string;
using Photo = cv::Mat;

/**
 * complete state of the delta robot (with many redundant variables to trade-off having to compute forward dynamics
 * all the time)
 */
class State {
protected:
    Eigen::Matrix<double, 3, 3> joint_angles_;
    Eigen::Matrix<double, 3, 3> joint_ang_vel_; // joint angular velocities
    Eigen::Matrix<double, 3, 3> joint_ang_acc_; // joint angular accelerations
    Eigen::Matrix<double, 3, 1> ee_pos_; // end-effector position
    Eigen::Matrix<double, 3, 1> ee_vel_; // end-effector velocity
    Eigen::Matrix<double, 3, 1> ee_acc_; // end-effector acceleration

public:
    virtual ~State() = default;
};

/**
 * contains the same member data as the State class, but with covariance matrices for each variable
 */
class EstimatedState : public State {
protected:
    Eigen::Matrix<double, 3, 3> joint_angle_cov_;
    Eigen::Matrix<double, 3, 3> joint_ang_vel_cov_;
    Eigen::Matrix<double, 3, 3> joint_ang_acc_cov_;
    Eigen::Matrix<double, 3, 1> ee_pos_cov_;
    Eigen::Matrix<double, 3, 1> ee_vel_cov_;
    Eigen::Matrix<double, 3, 1> ee_acc_cov_;
};

/**
 * just the reference joint angle, angular velocity, and acceleration
 */
class Reference {
protected:
    Eigen::Matrix<double, 3, Eigen::Dynamic> joint_angles_;
    Eigen::Matrix<double, 3, Eigen::Dynamic> joint_ang_vel_; // joint angular velocities
    Eigen::Matrix<double, 3, Eigen::Dynamic> joint_ang_acc_; // joint angular accelerations
};

/**
 * discrete-time motor torques (output of control algorithm)
 */
class MotorTorques {
protected:
    Eigen::Matrix<double, 3, Eigen::Dynamic> tau_; // discrete-time torques (at every kth instant) -> I think I'm fundamentally limited by the PWM duty cycle (or whatever it's called) for my motor (I think it's around 20ms -> 50Hz)
};

/**
 * discrete-time motor commands (transformed motor torques that are actionable by the I/O thread)
 */
class MotorCommands {
protected:
    Eigen::Matrix<double, 3, Eigen::Dynamic> commands_; // discrete-time commands for the motors in terms of angle to go to next
};

/**
 * concatenation of sensor data for single IMU and 3 motor encoders at fixed time points for an indefinite number of
 * time points
 */
class SensorReadings {
protected:
    Eigen::Matrix<double, 9, Eigen::Dynamic> imu_data_; // not sure what the exact physical quantities returned by the HillCrest FSM 300 are, or how many there are, but I'm guessing maybe 9
    Eigen::Matrix<double, 3, Eigen::Dynamic> mot_encoder_data_; // this is not really data it's an inference of what the motor position should be given the history of inputs
};

#endif //DELTA_REFERENCE_HPP
