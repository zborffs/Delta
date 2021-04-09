#include "_app_state_init.hpp"

/**
 * initializes each one of the components using by using a configuration file to inform component's builder one proper
 * construction of modules.
 */
bool AppStateInit::handle() {
    /// Initialize Application from configuration file
    init_cameras();
    init_imus();
    init_motors();
    init_model();
    init_state_estimator();
    init_controller();
    init_plan();
    init_recognition();
    init_engine();

    /// launch controller / state-estimator thread and move into App's thread
    std::thread t(&App::rt_loop, context_);
    context_->rt_loop_thread_ = std::move(t);

    /// Transition to the next state and return true to stay within App's 'main' loop
    context_->transition_to(new AppStateRecognition);
    return true;
}

/**
 * initializes the camera objects
 */
void AppStateInit::init_cameras() {
    auto camera_config = context_->config_file()["Cameras"];

    // if the camera tag isn't in the config file, then just return
    if (!camera_config.IsDefined()) {
        spdlog::get("delta_logger")->warn(R"("Cameras" tag is not in config file.)");
        return;
    }
    // if the camera tag is not a "sequence" (yaml-cpp lingo for a list in YAML syntax), then quit
    else if (!camera_config.IsSequence()) {
        spdlog::get("delta_logger")->warn(R"("Cameras" tag is not a Sequence.)");
        return;
    } else {
        for (YAML::const_iterator it = camera_config.begin(); it != camera_config.end(); ++it) {
            auto& camera_yaml = *it;

            // if no "type" property was defined, then ignore this entry
            if (!camera_yaml["type"].IsDefined()) {
                spdlog::get("delta_logger")->warn(R"("Cameras" tag provided, no "type" provided. Skipping.)");
                continue;
            }
            // if the type was Adafruit, then instantiate an Adafruit unique_ptr in the cameras member variable of app
            else if (camera_yaml["type"].as<std::string>() == "AdafruitPTC08") {
#ifdef JETSON_BUILD
                context_->cameras_.emplace_back(std::make_unique<AdafruitPTC08<HWJetson> >());
#else
                context_->cameras_.emplace_back(std::make_unique<AdafruitPTC08<HWMock> >());
#endif // JETSON_BUILD
            }
            // if the type was Mock, then instantiate a Mock unique_ptr in the cameras member variable of app
            else if (camera_yaml["type"].as<std::string>() == "MockCamera") {
                std::string file_name{};
                if (camera_yaml["filename"].IsDefined()) {
                    file_name = camera_yaml["filename"].as<std::string>();
                }
#ifdef JETSON_BUILD
                context_->cameras_.emplace_back(std::make_unique<MockCamera<HWJetson> >(file_name));
#else
                context_->cameras_.emplace_back(std::make_unique<MockCamera<HWMock> >(file_name));
#endif // JETSON_BUILD
            }
            // if the type was neither mock nor adafruit, then just ignore the entire entry
            else {
                spdlog::get("delta_logger")->warn(R"("Cameras" tag provided, but "type" didn't match a predefined type. Skipping.)");
                continue;
            }
        }
    }
}

void AppStateInit::init_imus() {
    auto imu_config = context_->config_file()["IMUs"];

    // if the camera tag isn't in the config file, then just return
    if (!imu_config.IsDefined()) {
        spdlog::get("delta_logger")->warn(R"("IMUs" tag is not in config file.)");
        return;
    }
    // if the camera tag is not a "sequence" (yaml-cpp lingo for a list in YAML syntax), then quit
    else if (!imu_config.IsSequence()) {
        spdlog::get("delta_logger")->warn(R"("IMUs" tag is not a Sequence.)");
        return;
    } else {
        for (YAML::const_iterator it = imu_config.begin(); it != imu_config.end(); ++it) {
            auto& imu_yaml = *it;

            // if no "type" property was defined, then ignore this entry
            if (!imu_yaml["type"].IsDefined()) {
                spdlog::get("delta_logger")->warn(R"("IMUs" tag provided, no "type" provided. Skipping.)");
                continue;
            }
                // if the type was Adafruit, then instantiate an Adafruit unique_ptr in the cameras member variable of app
            else if (imu_yaml["type"].as<std::string>() == "HCrestFSM300") {
#ifdef JETSON_BUILD
                context_->imus_.emplace_back(std::make_unique<HCrestFSM300<HWJetson>>());
#else
                context_->imus_.emplace_back(std::make_unique<HCrestFSM300<HWMock>>());
#endif // JETSON_BUILD
            }
            // if the type was neither mock nor adafruit, then just ignore the entire entry
            else {
                spdlog::get("delta_logger")->warn(R"("IMUs" tag provided, but "type" didn't match a predefined type. Skipping.)");
                continue;
            }
        }
    }
}

void AppStateInit::init_motors() {
    auto motor_config = context_->config_file()["Motors"];

    // if the camera tag isn't in the config file, then just return
    if (!motor_config.IsDefined()) {
        spdlog::get("delta_logger")->warn(R"("Motors" tag is not in config file.)");
        return;
    }
        // if the camera tag is not a "sequence" (yaml-cpp lingo for a list in YAML syntax), then quit
    else if (!motor_config.IsSequence()) {
        spdlog::get("delta_logger")->warn(R"("Motors" tag is not a Sequence.)");
        return;
    } else {
        for (YAML::const_iterator it = motor_config.begin(); it != motor_config.end(); ++it) {
            auto& motor_yaml = *it;

            // if no "type" property was defined, then ignore this entry
            if (!motor_yaml["type"].IsDefined()) {
                spdlog::get("delta_logger")->warn(R"("Motors" tag provided, no "type" provided. Skipping.)");
                continue;
            }
                // if the type was Adafruit, then instantiate an Adafruit unique_ptr in the cameras member variable of app
            else if (motor_yaml["type"].as<std::string>() == "ServoMG996R") {
#ifdef JETSON_BUILD
                context_->motors_.emplace_back(std::make_unique<ServoMG996R<HWJetson>>());
#else
                context_->motors_.emplace_back(std::make_unique<ServoMG996R<HWMock>>());
#endif // JETSON_BUILD
            }
                // if the type was neither mock nor adafruit, then just ignore the entire entry
            else {
                spdlog::get("delta_logger")->warn(R"("Motors" tag provided, but "type" didn't match a predefined type. Skipping.)");
                continue;
            }
        }
    }
}

void AppStateInit::init_model() {
    auto model_config = context_->config_file()["StateEstimator"]; // get "StateEstimator" YAML map

    if (!model_config.IsDefined() || !model_config["type"].IsDefined()) {
        spdlog::get("delta_logger")->warn(
                R"(Either "Model" or "Model:type" not specified.
                        Creating Lagragian by default)");
        context_->model_ = std::make_unique<LagrangianDeltaRobot>();
    } else if (model_config["type"].as<std::string>() == "Lagragian") {
        context_->model_ = std::make_unique<LagrangianDeltaRobot>();
    } else {
        spdlog::get("delta_logger")->warn(
                R"(Model "type" tag does not match a predefined "type".
                    Creating Lagragian by default)");
        context_->model_ = std::make_unique<LagrangianDeltaRobot>();
    }
}

/**
 * Initializes the StateEstimator component
 */
void AppStateInit::init_state_estimator() {
    auto se_config = context_->config_file()["StateEstimator"]; // get "StateEstimator" YAML map

    if (!se_config.IsDefined() || !se_config["type"].IsDefined()) {
        spdlog::get("delta_logger")->warn(
                R"(Either "StateEstimator" or "StateEstimator:type" not specified.
                        Creating Unscented Kalman Filter by default)");
    } else if (se_config["type"].as<std::string>() == "UnscentedKalmanFilter") {
        context_->se_ = std::make_unique<UnscentedKalmanFilter>();
    } else if (se_config["type"].as<std::string>() == "ExtendedKalmanFilter") {
        context_->se_ = std::make_unique<ExtendedKalmanFilter>();
    } else {
        spdlog::get("delta_logger")->warn(
                R"(StateEstimator "type" tag does not match a predefined "type".
                    Creating Unscented Kalman Filter by default)");
    }
}

/**
 * Initializes the Engine component
 */
void AppStateInit::init_engine() {
    auto eng_config = context_->config_file()["Engine"];
    std::unique_ptr<EngineBuilder> builder;

    if (!eng_config.IsDefined() || !eng_config["type"].IsDefined()) {
        spdlog::get("delta_logger")->warn(
                R"(Either "Engine" or "Engine:type" not specified. Creating UCI Adapter by default)");
        builder = std::make_unique<UCIEngineBuilder>(eng_config);
    } else if (eng_config["type"].as<std::string>() == std::string("UCIEngine")) {
        builder = std::make_unique<UCIEngineBuilder>(eng_config);
    } else {
        spdlog::get("delta_logger")->warn(
                "\"Engine:type\" does not match a predefined type. Creating UCI Adapter by Default");
        builder = std::make_unique<UCIEngineBuilder>(eng_config);
    }

    builder->startup_engine();
    context_->engine_ = builder->get_product();
}

/**
 * Initializes the Path Planning Algorithm
 */
void AppStateInit::init_plan() {
    auto plan_config = context_->config_file()["PathPlanning"];
    if (!plan_config.IsDefined()) {
        spdlog::get("delta_logger")->warn("Path Planning Algorithm not specified. Creating B-Splines Trajectory by Default");
        context_->plan_ = std::make_unique<PlanBSplinesTrajectory>();
    } else if (plan_config["type"].as<std::string>() == std::string("B-Splines")) {
        context_->plan_ = std::make_unique<PlanBSplinesTrajectory>();
    } else {
        spdlog::get("delta_logger")->warn("Path Planning Algorithm does not match a predefined type. Creating B-Splines Trajectory by Default");
        context_->plan_ = std::make_unique<PlanBSplinesTrajectory>();
    }
}

/**
 * Initializes the controller
 */
void AppStateInit::init_controller() {
    auto control_config = context_->config_file()["Controller"];
    if (!control_config.IsDefined()) {
        spdlog::get("delta_logger")->warn("Controller type not specified. Creating GravityCompPID by Default");
        context_->control_ = std::make_unique<GravityCompPID>();
    } else if (control_config["type"].as<std::string>() == std::string("GravityCompPID")) {
        context_->control_ = std::make_unique<GravityCompPID>();
    } else {
        spdlog::get("delta_logger")->warn("Controller type does not match a predefined type. Creating GravityCompPIDby Default");
        context_->control_ = std::make_unique<GravityCompPID>();
    }
}

/**
 * initializes the recognition component
 */
void AppStateInit::init_recognition() {
    auto recog_config = context_->config_file()["Recognition"];
    if (!recog_config.IsDefined() || !recog_config["type"].IsDefined()) {
        spdlog::get("delta_logger")->warn(R"("Recognition" tag is not in config file. Creating SegmentClassify by default.)");
        context_->recognition_ = std::make_unique<SegmentClassify>();
    } else if (recog_config["type"].as<std::string>() == "SegmentClassify") {
        context_->recognition_ = std::make_unique<SegmentClassify>();
    } else {
        spdlog::get("delta_logger")->warn(R"("Recognition:type" tag doesn't match predefined types. Creating SegmentClassify by default.)");
        context_->recognition_ = std::make_unique<SegmentClassify>();
    }
}


