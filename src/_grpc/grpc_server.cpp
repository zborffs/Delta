#include "_grpc/grpc_server.hpp"

void gRPCServer::operator()() {
    spdlog::get("delta_logger")->info("gRPCServer says: Engine Data = {}", app_->engine_data());
}