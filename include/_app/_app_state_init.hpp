#ifndef DELTA__APP_STATE_INIT_HPP
#define DELTA__APP_STATE_INIT_HPP

/// Third-Party Includes
#include <spdlog/spdlog.h>

/// Project Includes
#include "app.hpp"
#include "_app_state_recognition.hpp"
#include "_se/state_estimator_builder.hpp"
#include "_control/controller_builder.hpp"
#include "_engine/engine_builder.hpp"
#include "_plan/plan_builder.hpp"
#include "_recog/recognition.hpp"
#include "_recog/_simple_recognition_strategy.hpp"
#include "_camera/camera.hpp"
#include "_camera/_adafruit_ptc08.hpp"
#include "_camera/_mock_cam.hpp"
#include "_hw/_hw_mock.hpp"
#include "_hw/_hw_jetson.hpp"

/**
 * this class contains high-level logic for the initialization of the application
 * - always the first state
 * - is never returned to
 * - continues onto Recognition state
 * - makes use of component's builders to build complex component objects from configuration file data
 * - updates internal state of App object with component objects (Controllers, StateEstimators, etc.)
 */
class AppStateInit : public AppState {
private:
#ifdef JETSON_BUILD
    std::vector<std::shared_ptr<Camera<HWJetson> > > cameras_;
#else
    std::vector<std::shared_ptr<Camera<HWMock> > > cameras_;
#endif // JETSON_BUILD

    void init_cameras();
    void init_state_estimator();
    void init_engine();
    void init_plan();
    void init_controller();
    void init_recognition();
public:
    explicit AppStateInit() {
        spdlog::get("delta_logger")->info("Created AppStateInit");
    }
    ~AppStateInit() {
        spdlog::get("delta_logger")->info("Destroyed AppStateInit");
    }
    bool handle() override;
};

#endif // DELTA__APP_STATE_INIT_HPP