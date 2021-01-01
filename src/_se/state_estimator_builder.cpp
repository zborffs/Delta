#include "_se/state_estimator_builder.hpp"

/**
 * resets the UnscentedKalmanFilter product object in case the builder is reused to make another object
 */
void UnscentedKalmanFilterBuilder::reset() {
    unscented_kf_ = std::make_unique<UnscentedKalmanFilter>();
}

/**
 * builds the PoseStrategy object in the StateEstimator object from the configuration file passed in by the user as a
 * program argument
 */
#ifdef JETSON_BUILD
void UnscentedKalmanFilterBuilder::build_pose(std::vector<std::shared_ptr<Camera<HWJetson>>> &cameras) {
    /// If no "StateEstimator" tag is defined, then create the most basic UnscentedKalmanFilter object imaginable
    /// (i.e. no camera modules and the SimplePoseStrategy), then return;
    if (!config_.IsDefined() || !config_["Pose"].IsDefined() || !config_["Pose"]["type"].IsDefined()) {
        spdlog::get("delta_logger")->warn(
                "No State Estimator algorithm provided. Using SimplePoseStrategy for Pose Estimation by default");
        unscented_kf_->set_pose(std::make_unique<SimplePoseStrategy>(cameras));
        return;
    }

    /// If the "StateEstimator", "Pose", and "type" tags are all defined, then create the Pose algorithm associated with
    /// the given type
    if (config_["Pose"]["type"].as<std::string>() == "Simple") {
        std::unique_ptr<Pose> pose_strategy = std::make_unique<SimplePoseStrategy>(cameras); // declare a SimplePoseStategy unique pointer
        unscented_kf_->set_pose(std::move(pose_strategy));
    }
    /// if the type of the "Pose" tag couldn't be determined from the predefined set, then just use the simplest
    /// algorithm by default without any cameras
    else {
        spdlog::get("delta_logger")->warn(
                "Pose algorithm provided doesn't match a predefined option. Using SimplePoseStrategy by default");
        unscented_kf_->set_pose(std::make_unique<SimplePoseStrategy>(cameras));
    }
}
#else
void UnscentedKalmanFilterBuilder::build_pose(std::vector<std::shared_ptr<Camera<HWMock>>> &cameras) {
    /// If no "StateEstimator" tag is defined, then create the most basic UnscentedKalmanFilter object imaginable
    /// (i.e. no camera modules and the SimplePoseStrategy), then return;
    if (!config_.IsDefined() || !config_["Pose"].IsDefined() || !config_["Pose"]["type"].IsDefined()) {
        spdlog::get("delta_logger")->warn(
                "No State Estimator algorithm provided. Using SimplePoseStrategy for Pose Estimation by default");
        unscented_kf_->set_pose(std::make_unique<SimplePoseStrategy>(cameras));
        return;
    }

    /// If the "StateEstimator", "Pose", and "type" tags are all defined, then create the Pose algorithm associated with
    /// the given type
    if (config_["Pose"]["type"].as<std::string>() == "Simple") {
        std::unique_ptr<Pose> pose_strategy = std::make_unique<SimplePoseStrategy>(cameras); // declare a SimplePoseStategy unique pointer
        unscented_kf_->set_pose(std::move(pose_strategy));
    }
        /// if the type of the "Pose" tag couldn't be determined from the predefined set, then just use the simplest
        /// algorithm by default without any cameras
    else {
        spdlog::get("delta_logger")->warn(
                "Pose algorithm provided doesn't match a predefined option. Using SimplePoseStrategy by default");
        unscented_kf_->set_pose(std::make_unique<SimplePoseStrategy>(cameras));
    }
}
#endif // JETSON_BUILD

void UnscentedKalmanFilterBuilder::build_imu() {
    /// If no "StateEstimator" or "IMUs" tags are defined, then create the most basic UnscentedKalmanFilter object
    /// imaginable (i.e. no IMUs), then return;
    if (!config_.IsDefined() || !config_["IMUs"].IsDefined()) {
        spdlog::get("delta_logger")->warn(
                "No State Estimator algorithm / IMUs provided. Doing nothing by Default.");
        return;
    }

    YAML::Node imus = config_["IMUs"];
    if (imus.IsSequence()) {
        /// for each mapping in the sequence, create an IMU unique pointer if it makes sense to and push it into
        /// the unscented_kf_ object's imu vector
        for (YAML::const_iterator it = imus.begin(); it != imus.end(); ++it) {
            /// dependency inject hardware interface into IMU object
#ifdef JETSON_BUILD
            std::unique_ptr<IMU<HWJetson> > imu; // if running on Jetson
            double a{0.0};
#else
            std::unique_ptr<IMU<HWMock> > imu; // if running on host
            double a{0.0};
#endif // JETSON_BUILD

            auto& imu_yaml = *it;

            /// if no "x" tag is defined, that's no problem
            if (!imu_yaml["a"].IsDefined()) {
                // Do nothing
            } else {
                a = imu_yaml["a"].as<double>();
            }

            /// if the type of the imu wasn't defined, then this is useless, skip it
            if (!imu_yaml["type"].IsDefined()) {
                spdlog::get("delta_logger")->warn(
                        R"("IMUs" tag provided, but no "type" was provided. Skipping...)");
                continue;
            }
                /// if the type of the imu is defined, then create the associated imu type object
            else if (imu_yaml["type"].as<std::string>() == "HillcrestFSM300") {
                /// dependency inject hardware interface into IMU object
#ifdef JETSON_BUILD
                imu = std::make_unique<HillcrestFSM300<HWJetson> >(a);
#else
                imu = std::make_unique<HillcrestFSM300<HWMock> >(a);
#endif // JETSON_BUILD
            }
                /// if the type of the imu couldn't be determined from predefined types, then skip
            else {
                spdlog::get("delta_logger")->warn(
                        R"("IMUs" tag provided, but "type" didn't match a predefined type. Skipping...)");
                continue;
            }

            /// push the imu onto the imu vector
            unscented_kf_->push_imu(std::move(imu));
        }
    } else {
        spdlog::get("delta_logger")->warn(
                R"("IMUs" tag provided, but resulting Node is not a Sequence. Breaking...)");
    }
}

/**
 * moves the UnscentedKalmanFilter object out of the class
 * @return std::unique_ptr<UnscentedKalmanFilter>
 */
std::unique_ptr<StateEstimator> UnscentedKalmanFilterBuilder::get_product() {
    return std::move(unscented_kf_);  // if no compile, try std::move(ptr);
}

/**
 * resets the ExtendedKalmanFilter product object in case the builder is reused to make another object
 */
void ExtendedKalmanFilterBuilder::reset() {
    extended_kf_ = std::make_unique<ExtendedKalmanFilter>();
}

/**
 * builds the PoseStrategy object in the StateEstimator object from the configuration file passed in by the user as a
 * program argument
 */
#ifdef JETSON_BUILD
void ExtendedKalmanFilterBuilder::build_pose(std::vector<std::shared_ptr<Camera<HWJetson>>> &cameras) {
    /// If no "StateEstimator" tag is defined, then create the most basic UnscentedKalmanFilter object imaginable
    /// (i.e. no camera modules and the SimplePoseStrategy), then return;
    if (!config_.IsDefined() || !config_["Pose"].IsDefined() || !config_["Pose"]["type"].IsDefined()) {
        spdlog::get("delta_logger")->warn(
                "No State Estimator algorithm provided. Using SimplePoseStrategy for Pose Estimation by default");
        extended_kf_->set_pose(std::make_unique<SimplePoseStrategy>(cameras));
        return;
    }

    /// If the "StateEstimator", "Pose", and "type" tags are all defined, then create the Pose algorithm associated with
    /// the given type
    if (config_["Pose"]["type"].as<std::string>() == "Simple") {
        std::unique_ptr<Pose> pose_strategy = std::make_unique<SimplePoseStrategy>(cameras); // declare a SimplePoseStategy unique pointer
        extended_kf_->set_pose(std::move(pose_strategy));
    }
    /// if the type of the "Pose" tag couldn't be determined from the predefined set, then just use the simplest
    /// algorithm by default without any cameras
    else {
        spdlog::get("delta_logger")->warn(
                "Pose algorithm provided doesn't match a predefined option. Using SimplePoseStrategy by default");
        extended_kf_->set_pose(std::make_unique<SimplePoseStrategy>(cameras));
    }
}
#else
void ExtendedKalmanFilterBuilder::build_pose(std::vector<std::shared_ptr<Camera<HWMock>>> &cameras) {
    /// If no "StateEstimator" tag is defined, then create the most basic UnscentedKalmanFilter object imaginable
    /// (i.e. no camera modules and the SimplePoseStrategy), then return;
    if (!config_.IsDefined() || !config_["Pose"].IsDefined() || !config_["Pose"]["type"].IsDefined()) {
        spdlog::get("delta_logger")->warn(
                "No State Estimator algorithm provided. Using SimplePoseStrategy for Pose Estimation by default");
        extended_kf_->set_pose(std::make_unique<SimplePoseStrategy>(cameras));
        return;
    }

    /// If the "StateEstimator", "Pose", and "type" tags are all defined, then create the Pose algorithm associated with
    /// the given type
    if (config_["Pose"]["type"].as<std::string>() == "Simple") {
        std::unique_ptr<Pose> pose_strategy = std::make_unique<SimplePoseStrategy>(cameras); // declare a SimplePoseStategy unique pointer
        extended_kf_->set_pose(std::move(pose_strategy));
    }
        /// if the type of the "Pose" tag couldn't be determined from the predefined set, then just use the simplest
        /// algorithm by default without any cameras
    else {
        spdlog::get("delta_logger")->warn(
                "Pose algorithm provided doesn't match a predefined option. Using SimplePoseStrategy by default");
        extended_kf_->set_pose(std::make_unique<SimplePoseStrategy>(cameras));
    }
}
#endif // JETSON_BUILD

void ExtendedKalmanFilterBuilder::build_imu() {
    /// If no "StateEstimator" or "IMUs" tags are defined, then create the most basic ExtendedKalmanFilter object
    /// imaginable (i.e. no IMUs), then return;
    if (!config_.IsDefined() || !config_["IMUs"].IsDefined()) {
        spdlog::get("delta_logger")->warn(
                "No State Estimator algorithm / IMUs provided. Doing nothing by Default.");
        return;
    }

    YAML::Node imus = config_["IMUs"];
    if (imus.IsSequence()) {
        /// for each mapping in the sequence, create an IMU unique pointer if it makes sense to and push it into
        /// the extended_kf_ object's imu vector
        for (YAML::const_iterator it = imus.begin(); it != imus.end(); ++it) {
            /// dependency inject hardware interface into IMU object
#ifdef JETSON_BUILD
            std::unique_ptr<IMU<HWJetson> > imu; // if running on Jetson
            double a{0.0};
#else
            std::unique_ptr<IMU<HWMock> > imu; // if running on host
            double a{0.0};
#endif // JETSON_BUILD

            auto& imu_yaml = *it;

            /// if no "x" tag is defined, that's no problem
            if (!imu_yaml["a"].IsDefined()) {
                // Do nothing
            } else {
                a = imu_yaml["a"].as<double>();
            }

            /// if the type of the imu wasn't defined, then this is useless, skip it
            if (!imu_yaml["type"].IsDefined()) {
                spdlog::get("delta_logger")->warn(
                        R"("IMUs" tag provided, but no "type" was provided. Skipping...)");
                continue;
            }
                /// if the type of the camera is defined, then create the associated camera type object
            else if (imu_yaml["type"].as<std::string>() == "HillcrestFSM300") {
                /// dependency inject hardware interface into Camera object
#ifdef JETSON_BUILD
                imu = std::make_unique<HillcrestFSM300<HWJetson> >(a);
#else
                imu = std::make_unique<HillcrestFSM300<HWMock> >(a);
#endif // JETSON_BUILD
            }
                /// if the type of the camera couldn't be determined from predefined types, then skip
            else {
                spdlog::get("delta_logger")->warn(
                        R"("IMUs" tag provided, but "type" didn't match a predefined type. Skipping...)");
                continue;
            }

            /// push the camera onto the camera vector
            extended_kf_->push_imu(std::move(imu));
        }
    } else {
        spdlog::get("delta_logger")->warn(
                R"("IMUs" tag provided, but resulting YAML::Node is not a Sequence. Breaking...)");
    }
}

/**
 * moves the ExtendedKalmanFilter object out of the class
 * @return std::unique_ptr<ExtendedKalmanFilter>
 */
std::unique_ptr<StateEstimator> ExtendedKalmanFilterBuilder::get_product() {
    return std::move(extended_kf_); // if no compile, try std::move(ptr);
}
