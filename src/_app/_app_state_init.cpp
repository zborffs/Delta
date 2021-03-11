#include "_app/_app_state_init.hpp"

/**
 * initializes each one of the components using by using a configuration file to inform component's builder one proper
 * construction of modules.
 */
bool AppStateInit::handle() {
    /// Initialize Application from configuration file
    init_cameras();
    init_state_estimator();
    init_engine();
    init_plan();
    init_controller();
    init_recognition();

#ifndef NDEBUG
    for (auto & camera : cameras_) {
        assert(camera.use_count() == 3);
    }
#endif // NDEBUG

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

    if (!camera_config.IsDefined()) {
        spdlog::get("delta_logger")->warn(R"("Cameras" tag is not in config file.)");
        return;
    } else if (!camera_config.IsSequence()) {
        spdlog::get("delta_logger")->warn(R"("Cameras" tag is not a Sequence.)");
        return;
    } else {
        for (YAML::const_iterator it = camera_config.begin(); it != camera_config.end(); ++it) {
#ifdef JETSON_BUILD
            std::shared_ptr<Camera<HWJetson> > camera; // if running on Jetson
#else
            std::shared_ptr<Camera<HWMock> > camera; // if running on host
#endif // JETSON_BUILD
            auto& camera_yaml = *it;

            if (!camera_yaml["type"].IsDefined()) {
                spdlog::get("delta_logger")->warn(R"("Cameras" tag provided, no "type" provided. Skipping.)");
                continue;
            } else if (camera_yaml["type"].as<std::string>() == "AdafruitPTC08") {
                double x{0.0};
                if (camera_yaml["x"].IsDefined()) {
                    x = camera_yaml["x"].as<double>();
                }
#ifdef JETSON_BUILD
                cameras_.emplace_back(std::make_shared<AdafruitPTC08<HWJetson> >(x));
#else
                cameras_.emplace_back(std::make_shared<AdafruitPTC08<HWMock> >(x));
#endif // JETSON_BUILD
            } else if (camera_yaml["type"].as<std::string>() == "MockCamera") {
                std::string file_name{};
                if (camera_yaml["filename"].IsDefined()) {
                    file_name = camera_yaml["filename"].as<std::string>();
                }
#ifdef JETSON_BUILD
                cameras_.emplace_back(std::make_shared<MockCamera<HWJetson> >(file_name));
#else
                cameras_.emplace_back(std::make_shared<MockCamera<HWMock> >(file_name));
#endif // JETSON_BUILD
            } else {
                spdlog::get("delta_logger")->warn(R"("Cameras" tag provided, but "type" didn't match a predefined type. Skipping.)");
                continue;
            }
        }
    }
}

/**
 * Initializes the StateEstimator component
 */
void AppStateInit::init_state_estimator() {
    auto se_config = context_->config_file()["StateEstimator"]; // get "StateEstimator" YAML map
    std::unique_ptr<StateEstimatorBuilder> builder; // declare abstract builder unique pointer, dynamically define later

    if (!se_config.IsDefined() || !se_config["type"].IsDefined()) {
        spdlog::get("delta_logger")->warn(
                R"(Either "StateEstimator" or "StateEstimator:type" not specified.
                        Creating Unscented Kalman Filter by default)");
        builder = std::make_unique<UnscentedKalmanFilterBuilder>(se_config);
    } else if (se_config["type"].as<std::string>() == "UnscentedKalmanFilter") {
        builder = std::make_unique<UnscentedKalmanFilterBuilder>(se_config);
    } else if (se_config["type"].as<std::string>() == "ExtendedKalmanFilter") {
        builder = std::make_unique<ExtendedKalmanFilterBuilder>(se_config);
    } else {
        spdlog::get("delta_logger")->warn(
                R"(StateEstimator "type" tag does not match a predefined "type".
                    Creating Unscented Kalman Filter by default)");
        builder = std::make_unique<UnscentedKalmanFilterBuilder>(se_config);
    }

    builder->build_pose(cameras_);
    builder->build_imu();
    context_->se_ = builder->get_product();
    assert(cameras_[0].use_count() == 2);
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
 * Initializes the Route Planning Algorithm
 */
void AppStateInit::init_plan() {
    auto plan_config = context_->config_file()["PathPlanningAlg"];
    std::unique_ptr<PlanBuilder> builder;

    if (!plan_config.IsDefined()) {
        spdlog::get("delta_logger")->warn("Path Planning Algorithm not specified. Creating PathAlgorithmSimple by Default");
        builder = std::make_unique<SimplePlanAlgorithmBuilder>(plan_config);
    } else if (plan_config["type"].as<std::string>() == std::string("Simple")) {
        builder = std::make_unique<SimplePlanAlgorithmBuilder>(plan_config);
    } else {
        spdlog::get("delta_logger")->warn("Path Planning Algorithm does not match a predefined type. Creating PathAlgorithmSimple by Default");
        builder = std::make_unique<SimplePlanAlgorithmBuilder>(plan_config);
    }

    context_->plan_ = builder->get_product();
}

/**
 * Initializes the controller
 */
void AppStateInit::init_controller() {
    auto control_config = context_->config_file()["Controller"];
    std::unique_ptr<ControllerBuilder> builder;

    if (!control_config.IsDefined()) {
        spdlog::get("delta_logger")->warn("Controller type not specified. Creating LinearController with no gain (i.e. open-loop system) by Default");
        builder = std::make_unique<LinearControllerBuilder>(control_config);
    } else if (control_config["type"].as<std::string>() == std::string("Linear")) {
        builder = std::make_unique<LinearControllerBuilder>(control_config);
    } else if (control_config["type"].as<std::string>() == std::string("Optimal")) {
        builder = std::make_unique<OptimalControllerBuilder>(control_config);
    } else {
        spdlog::get("delta_logger")->warn("Controller type does not match a predefined type. Creating LinearController with no gain (i.e. open-loop system) by Default");
        builder = std::make_unique<LinearControllerBuilder>(control_config);
    }

    builder->build_model();
    builder->build_motors();
    context_->control_ = builder->get_product();
}

/**
 * initializes the recognition component
 */
void AppStateInit::init_recognition() {
    auto recog_config = context_->config_file()["Recognition"];
    std::unique_ptr<Recognition> recog_strat;

    if (!recog_config.IsDefined() || !recog_config["type"].IsDefined()) {
        spdlog::get("delta_logger")->warn(R"("Recognition" tag is not in config file. Creating SimpleRecognitionStrategy by default.)");
        recog_strat = std::make_unique<SimpleRecognitionStrategy>(cameras_);
    } else if (recog_config["type"].as<std::string>() == "Simple") {
        recog_strat = std::make_unique<SimpleRecognitionStrategy>(cameras_);
    } else {
        spdlog::get("delta_logger")->warn(R"("Recognition:type" tag doesn't match predefined types. Creating SimpleRecognitionStrategy by default.)");
        recog_strat = std::make_unique<SimpleRecognitionStrategy>(cameras_);
    }

    context_->recognition_ = std::move(recog_strat);
}
