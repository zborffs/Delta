#ifndef DELTA_ENGINE_BUILDER_HPP
#define DELTA_ENGINE_BUILDER_HPP

#include "_engine/engine_interface.hpp"
#include "_engine/_uci_adapter.hpp"
#include <memory>
#include <yaml-cpp/yaml.h>

class EngineBuilder {
public:
    virtual ~EngineBuilder() = default;
    virtual void reset() = 0;
    virtual void startup_engine() = 0;
    virtual std::unique_ptr<EngineInterface> get_product() = 0;
};

class UCIEngineBuilder : public EngineBuilder {
private:
    YAML::Node config_;
    std::unique_ptr<UCIAdapter> uci_engine_;

public:
    explicit UCIEngineBuilder(const YAML::Node& config) : config_(config), uci_engine_(std::make_unique<UCIAdapter>()) {}
    void reset() override;
    void startup_engine() override;
    std::unique_ptr<EngineInterface> get_product() override;
};

#endif //DELTA_ENGINE_BUILDER_HPP
