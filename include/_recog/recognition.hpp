#ifndef DELTA_RECOGNITION_HPP
#define DELTA_RECOGNITION_HPP

/// STL Includes
#include <memory>
#include <string>
#include <optional>

/// Project Includes
#include "_camera/camera.hpp"
#include "_hw/_hw_jetson.hpp"
#include "_hw/_hw_mock.hpp"

class Recognition {
protected:
#ifdef JETSON_BUILD
    std::vector<std::shared_ptr<Camera<HWJetson> > > cameras_;
#else
    std::vector<std::shared_ptr<Camera<HWMock> > > cameras_;
#endif // JETSON_BUILD
public:
    virtual ~Recognition() = default;
    virtual std::optional<std::string> position() = 0;
};

#endif //DELTA_RECOGNITION_HPP
