#include "_control/controller_builder.hpp"

/**
 * creates a new LinearController product object in the event that this object needs to build another
 * LinearController object
 */
void LinearControllerBuilder::reset() {
    linear_controller_ = std::make_unique<LinearController>();
}

/**
 * instantiates the model member variable of the OptimalController object
 */
void LinearControllerBuilder::build_model() {
    // Do nothing!
}

/**
 * instantiates the motor vector member variable of the LinearController object
 */
void LinearControllerBuilder::build_motors() {
    if (!config_.IsDefined() || !config_["Motors"].IsDefined()) {
        spdlog::get("delta_logger")->warn(
                "No Controller algorithm / Motors provided. Doing nothing by default.");
        return;
    }

    YAML::Node motors = config_["Motors"];
    if (motors.IsSequence()) {
        for (YAML::const_iterator it = motors.begin(); it != motors.end(); ++it) {
#ifdef JETSON_BUILD
            std::unique_ptr<Motor<HWJetson> > motor;
            double x{0.0};
#else
            std::unique_ptr<Motor<HWMock> > motor;
            double x{0.0};
#endif // JETSON_BUILD
            auto& motor_yaml = *it;

            if (!motor_yaml["x"].IsDefined()) {
                // do nothing
            } else {
                x = motor_yaml["x"].as<double>();
            }

            /// if the type of the imu wasn't defined, then this is useless, skip it
            if (!motor_yaml["type"].IsDefined()) {
                spdlog::get("delta_logger")->warn(
                        R"("Motors" tag provided, but no "type" was provided. Skipping...)");
                continue;
            }
                /// if the type of the camera is defined, then create the associated camera type object
            else if (motor_yaml["type"].as<std::string>() == "uServoSG90") {
                /// dependency inject hardware interface into Camera object
#ifdef JETSON_BUILD
                motor = std::make_unique<uServoSG90<HWJetson> >(x);
#else
                motor = std::make_unique<uServoSG90<HWMock> >(x);
#endif // JETSON_BUILD
            }
                /// if the type of the camera couldn't be determined from predefined types, then skip
            else {
                spdlog::get("delta_logger")->warn(
                        R"("Motors" tag provided, but "type" didn't match a predefined type. Skipping...)");
                continue;
            }

            /// push the camera onto the camera vector
            linear_controller_->push_motor(std::move(motor));
        }
    } else {
        spdlog::get("delta_logger")->warn(
                R"("Motors" tag provided, but resulting YAML::Node is not a Sequence. Breaking...)");
    }
}

/**
 * moves the LinearController product object into caller's scope and out of class
 * @return std::unique_ptr<LinearController> object
 */
std::unique_ptr<Controller> LinearControllerBuilder::get_product() {
    return std::move(linear_controller_);  // if no compile, try std::move(ptr);
}


/**
 * creates a new OptimalController product object in the event that this object needs to create another
 * OptimalController object
 */
void OptimalControllerBuilder::reset() {
    optimal_controller_ = std::make_unique<OptimalController>();
}

/**
 * instantiates the model member variable of the OptimalController object
 */
void OptimalControllerBuilder::build_model() {
    if (!config_.IsDefined()) {
        spdlog::get("delta_logger")->warn("No Robot Model specified. Creating SimpleDeltaModel by Default");
        optimal_controller_->set_model(std::make_unique<SimpleDeltaModel>());
    } else if (config_["Model"]["type"].as<std::string>() == std::string("Simple")) {
        optimal_controller_->set_model(std::make_unique<SimpleDeltaModel>());
    } else {
        spdlog::get("delta_logger")->warn("Robot Model does not match a predefined type. Creating SimpleDeltaModel by Default");
        optimal_controller_->set_model(std::make_unique<SimpleDeltaModel>());
    }
}

/**
 * instantiates the motor vector member variable of the OptimalController object
 */
void OptimalControllerBuilder::build_motors() {
    if (!config_.IsDefined() || !config_["Motors"].IsDefined()) {
        spdlog::get("delta_logger")->warn(
                "No Controller algorithm / Motors provided. Doing nothing by default.");
        return;
    }

    YAML::Node motors = config_["Motors"];
    if (motors.IsSequence()) {
        for (YAML::const_iterator it = motors.begin(); it != motors.end(); ++it) {
#ifdef JETSON_BUILD
            std::unique_ptr<Motor<HWJetson> > motor;
            double x{0.0};
#else
            std::unique_ptr<Motor<HWMock> > motor;
            double x{0.0};
#endif // JETSON_BUILD
            auto& motor_yaml = *it;

            if (!motor_yaml["x"].IsDefined()) {
                // do nothing
            } else {
                x = motor_yaml["x"].as<double>();
            }

            /// if the type of the imu wasn't defined, then this is useless, skip it
            if (!motor_yaml["type"].IsDefined()) {
                spdlog::get("delta_logger")->warn(
                        R"("Motors" tag provided, but no "type" was provided. Skipping...)");
                continue;
            }
                /// if the type of the camera is defined, then create the associated camera type object
            else if (motor_yaml["type"].as<std::string>() == "uServoSG90") {
                /// dependency inject hardware interface into Camera object
#ifdef JETSON_BUILD
                motor = std::make_unique<uServoSG90<HWJetson> >(x);
#else
                motor = std::make_unique<uServoSG90<HWMock> >(x);
#endif // JETSON_BUILD
            }
                /// if the type of the camera couldn't be determined from predefined types, then skip
            else {
                spdlog::get("delta_logger")->warn(
                        R"("Motors" tag provided, but "type" didn't match a predefined type. Skipping...)");
                continue;
            }

            /// push the camera onto the camera vector
            optimal_controller_->push_motor(std::move(motor));
        }
    } else {
        spdlog::get("delta_logger")->warn(
                R"("Motors" tag provided, but resulting YAML::Node is not a Sequence. Breaking...)");
    }
}

/**
 * moves the OptimalController product object into caller's scope and out of class
 * @return std::unique_ptr<OptimalController> object
 */
std::unique_ptr<Controller> OptimalControllerBuilder::get_product() {
    return std::move(optimal_controller_);  // if no compile, try std::move(ptr);
}