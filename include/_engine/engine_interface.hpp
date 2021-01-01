#ifndef DELTA_ENGINE_INTERFACE_HPP
#define DELTA_ENGINE_INTERFACE_HPP

#include <memory>
#include <optional>

class EngineInterface {
public:
    virtual ~EngineInterface() = default;
    virtual std::optional<int> best_move(const std::string& fen) = 0;
    virtual bool is_move_legal(const std::string& fen) = 0;
};

#endif // DELTA_ENGINE_INTERFACE_HPP
