#include <gtest/gtest.h>
#include <vector>
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include "_camera/camera.hpp"
#include "_camera/_mock_cam.hpp"
#include "_hw/_hw_mock.hpp"
#include "_recog/_simple_recognition_strategy.hpp"

/**
 * sets up the console and log-file sinks for the logger used throughout the codebase
 * @param logfile_path the path to the logfile
 * @return             true means the initialization was successful, false otherwise
 */
bool init_logger() {
    try {
        /// Setup the console sink
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(spdlog::level::debug);
        console_sink->set_pattern("[%D %H:%M:%S] [%^%l%$] [Thread %t] %v");
        console_sink->set_color_mode(spdlog::color_mode::always);

        /// setup the logger using both sinks
        spdlog::logger logger("delta_logger", {console_sink});
        logger.set_level(spdlog::level::debug);
        spdlog::set_default_logger(std::make_shared<spdlog::logger>("delta_logger", spdlog::sinks_init_list({console_sink})));
    } catch (const spdlog::spdlog_ex& ex) {
        std::cout << "Logger initialization failed: " << ex.what() << std::endl;
        return false; // if we fail to initialize the logger, return false
    }

    return true;
}

class SimpleRecognitionStrategyTester : public ::testing::Test {
protected:
    std::string file_name_;
    std::vector<std::shared_ptr<Camera<HWMock>>> cameras_;
    std::unique_ptr<SimpleRecognitionStrategy> strat_;

public:
    SimpleRecognitionStrategyTester() : file_name_("../data/recog/") {
        cameras_.emplace_back(std::make_shared<MockCamera<HWMock>>(file_name_));
        strat_ = std::make_unique<SimpleRecognitionStrategy>(cameras_);
    }
};

TEST_F(SimpleRecognitionStrategyTester, Position) {
    for (unsigned i = 0; i < 6; ++i) {
        EXPECT_EQ(strat_->position(), std::nullopt);
    }
}

int main(int argc, char** argv) {
    if (!init_logger()) {
        return -1; // if the initialization of the logger fails, just quit immediately
    }

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
