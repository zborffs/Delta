#ifndef DELTA_APP_HPP
#define DELTA_APP_HPP

/// STL Includes
#include <thread>
#include <chrono>

/// Third-Party Includes
#include <yaml-cpp/yaml.h>
#include <spdlog/spdlog.h>

/// Project Includes
#include "app_interface.hpp"
#include "_se/state_estimator.hpp"
#include "_engine/engine_interface.hpp"
#include "_plan/plan_interface.hpp"
#include "_plan/_simple_plan_alg.hpp"
#include "_control/controller.hpp"
#include "_recog/recognition.hpp"

class App;

class AppState {
protected:
    App* context_{};
public:
    virtual ~AppState() = default;
    virtual bool handle() = 0;
    void set_context(App* context) {
        this->context_ = context;
    }
};

class App : public AppInterface {
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
    std::unique_ptr<StateEstimator> se_;
    std::unique_ptr<EngineInterface> engine_;
    std::unique_ptr<PlanInterface> plan_;
    std::unique_ptr<Controller> control_;
    std::unique_ptr<Recognition> recognition_;
    std::thread rt_loop_thread_;

    bool quit_{false};
    std::mutex mu_quit_;

    void rt_loop();
    bool quit();
    void quit_rt_loop();

public:
    explicit App(const YAML::Node& config_file, std::unique_ptr<AppState> init_state) : config_file_(config_file), state_(std::move(init_state)) {
        state_->set_context(this);
        spdlog::get("delta_logger")->info("Created App");
    }
    ~App() {
        spdlog::get("delta_logger")->info("Destroyed App");
    }
    void transition_to(AppState* state);
    void operator()();

    /// Getters
    inline YAML::Node config_file() {
        return config_file_;
    }

    /// Interface functions
    int engine_data() override;
};

#endif // DELTA_APP_HPP
