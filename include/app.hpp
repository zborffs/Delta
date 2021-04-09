#ifndef DELTA_APP_HPP
#define DELTA_APP_HPP

/// STL Includes
#include <thread>
#include <chrono>

/// Third-Party Includes
#include <yaml-cpp/yaml.h>
#include <spdlog/spdlog.h>

/// Project Includes
#include "state_estimator.hpp"
#include "engine.hpp"
#include "path_plan.hpp"
#include "recognition.hpp"
#include "controller.hpp"
#include "recognition.hpp"
#include "camera.hpp"
#include "imu.hpp"
#include "motor.hpp"
#include "model.hpp"
#include "hardware.hpp"

class App;

class AppState {
protected:
    App* context_{};
public:
    virtual ~AppState() = default;
    virtual bool handle() = 0;

    /**
     * member function in superclass of AppState setting the context (pointer to App) to the given App object
     * @param context the current App context pointer
     */
    void set_context(App* context) {
        this->context_ = context;
    }
};

class App {
protected:
    YAML::Node config_file_;
    friend class AppStateInit;
    friend class AppStateRecognition;
    friend class AppStateEng;
    friend class AppStatePlan;
    friend class AppStateWait;
    friend class AppStateShutdown;

private:
    std::unique_ptr<AppState> state_;

    // templated design-pattern algorithms
    std::unique_ptr<StateEstimator> se_;
    std::unique_ptr<EngineInterface> engine_;
    std::unique_ptr<PathPlanner> plan_;
    std::unique_ptr<Controller> control_;
    std::unique_ptr<Recognition> recognition_;
    std::unique_ptr<Model> model_;

    // io devices -> dependency injected hardware at compile time
#ifdef JETSON_BUILD
    std::vector<std::unique_ptr<Camera<HWJetson>>> cameras_;
    std::vector<std::unique_ptr<IMU<HWJetson>>> imus_;
    std::vector<std::unique_ptr<Motor<HWJetson>>> motors_;
#else
    std::vector<std::unique_ptr<Camera<HWMock>>> cameras_;
    std::vector<std::unique_ptr<IMU<HWMock>>> imus_;
    std::vector<std::unique_ptr<Motor<HWMock>>> motors_;
#endif

    // threads
    std::thread rt_loop_thread_;
    std::thread io_thread_;

    // mutexed variables and mutexes
    bool quit_{false};
    std::mutex mu_quit_;

    // private member functions
    void rt_loop();
    bool quit();
    void quit_rt_loop();

public:
    /**
     *
     * @param config_file
     * @param init_state
     */
    explicit App(const YAML::Node& config_file, std::unique_ptr<AppState> init_state) : config_file_(config_file), state_(std::move(init_state)) {
        state_->set_context(this);
        spdlog::get("delta_logger")->info("Created App");
    }

    /**
     *
     */
    ~App() {
        spdlog::get("delta_logger")->info("Destroyed App");
    }

    void transition_to(AppState* state);
    void operator()();

    /// Getters
    inline YAML::Node config_file() {
        return config_file_;
    }
};

#endif // DELTA_APP_HPP
