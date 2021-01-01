#ifndef DELTA_CONTROLLER_HPP
#define DELTA_CONTROLLER_HPP

#include "_model/model.hpp"
#include <memory>
#include <vector>
#include "_motor/motor.hpp"
#include "_hw/_hw_jetson.hpp"
#include "_hw/_hw_mock.hpp"

class Controller {
protected:
    std::unique_ptr<Model> model_;
#ifdef JETSON_BUILD
    std::vector<std::unique_ptr<Motor<HWJetson> > > motors_;
#else
    std::vector<std::unique_ptr<Motor<HWMock> > > motors_;
#endif // JETSON_BUILD

public:
    virtual ~Controller() = default;
    virtual void actuate(int reference, int est_state_vec) = 0;
    void set_model(std::unique_ptr<Model> model) {
        model_ = std::move(model);
    }

#ifdef JETSON_BUILD
    void push_motor(std::unique_ptr<Motor<HWJetson> > motor) {
        motors_.emplace_back(std::move(motor));
    }
#else
    void push_motor(std::unique_ptr<Motor<HWMock> > motor) {
        motors_.emplace_back(std::move(motor));
    }
#endif // JETSON_BUILD
};

#endif //DELTA_CONTROLLER_HPP
