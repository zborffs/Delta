/// stl includes
#include <iostream>
#include <memory>

/// 3rd party includes
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

/// internal includes
#include "recognition.hpp"

#define LOGGER_INITIALIZATION_FAILED -1
#define CONFIG_INITIALIZATION_FAILED -2
#define SUCCESS 0

/**
 * sets up the console and log-file sinks for the logger used throughout the codebase
 * @param  logfile_path the path to the logfile
 * @return true means the initialization was successful, false otherwise
 */
bool init_logger(const std::string& logfile_path) {
    try {
        /// Setup the console sink
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(spdlog::level::warn);
        console_sink->set_pattern("[%D %H:%M:%S] [%^%l%$] [Thread %t] [File:Line %@] [Function: %!] %v");
        console_sink->set_color_mode(spdlog::color_mode::always);

        /// setup the file sink
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logfile_path, true);
        file_sink->set_level(spdlog::level::trace);

        /// setup the logger using both sinks
        spdlog::logger logger("delta_logger", {console_sink, file_sink});
        logger.set_level(spdlog::level::debug);
        spdlog::set_default_logger(std::make_shared<spdlog::logger>("delta_logger", spdlog::sinks_init_list({console_sink, file_sink})));
    } catch (const spdlog::spdlog_ex& ex) {
        std::cout << "Logger initialization failed: " << ex.what() << std::endl;
        return false; // if we fail to initialize the logger, return false
    }

    return true;
}

int main([[maybe_unused]]const int argc, char** argv) {
    std::string path(argv[0]);
#ifdef WINDOWS
    std::string base(path.substr(0, path.find_last_of("\\")));
#else
    std::string base(path.substr(0, path.find_last_of('/')));
#endif // WINDOWS

    /// Setup logger output logfile from program arguments
    std::string logfile_path(base + "/../../logs/RecognitionAccuracy.log");
    if (!init_logger(logfile_path)) {
        return LOGGER_INITIALIZATION_FAILED; // if the initialization of the logger fails, just quit immediately
    }

    /// Position images
    std::string pos_img_base_path(base + "/../../res/positions/");
    std::vector<std::string> pos_img_paths({{"img1.jpg", "img2.jpg", "img3.jpg", "img4.jpg", "img5.jpg", "img6.jpg"}});

    /// Create recognition component
    std::unique_ptr<Recognition> recog = std::make_unique<SegmentClassify>();

    /// For each position, segment it
    for (auto& img_path : pos_img_paths) {
        Photo photo = cv::imread(pos_img_base_path + img_path);
        auto fen = recog->determine_position(photo);
    }

    /// mlpack

    return SUCCESS;
}
