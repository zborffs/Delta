#ifndef DELTA_GRPC_SERVER_HPP
#define DELTA_GRPC_SERVER_HPP

#include "_app/app_interface.hpp"
#include <spdlog/spdlog.h>

class gRPCServer {
    AppInterface* app_;

public:
    explicit gRPCServer(AppInterface* app) : app_(app) {}
    void operator()();
};

#endif // DELTA_GRPC_SERVER_HPP
