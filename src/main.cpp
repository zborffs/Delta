/// STL Includes
#include <iostream>
#include <thread>

/// Third-Party Includes
#include <yaml-cpp/yaml.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

/// Project Includes
#include "_app/app.hpp"
#include "_app/_app_state_init.hpp"
#include "_grpc/grpc_server.hpp"

/**
 * sets up the console and log-file sinks for the logger used throughout the codebase
 * @param logfile_path the path to the logfile
 * @return             true means the initialization was successful, false otherwise
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
    std::string logfile_path(base + "/../logs/Delta.log");
    if (!init_logger(logfile_path)) {
        return -1; // if the initialization of the logger fails, just quit immediately
    }

    /// Find configuration file from program arguments
    std::string config_path(base + "/../config/" + std::string(argv[1]));
    YAML::Node config_file;
    try {
        config_file = YAML::LoadFile(config_path);
    } catch (...) {
        spdlog::get("delta_logger")->error("Couldn't open configuration file!");
        return -2;
    }

    /// Setup the Application and the gRPC server
    App app(config_file, std::make_unique<AppStateInit>());
    gRPCServer server(&app);
    std::thread grpc_server_thread(server);
    app();
    grpc_server_thread.join();

    return 0;
}
