#ifndef DELTA_PLAN_INTERFACE_HPP
#define DELTA_PLAN_INTERFACE_HPP

#include <optional>

/**
 * The plan interface
 * - Input: Move
 * - Output: Vector of Coordinates in World Coordinate Frame
 */
class PlanInterface {
public:
    virtual ~PlanInterface() = default;
    virtual std::optional<int> plan_route(int best_move) = 0;
};

#endif //DELTA_PLAN_INTERFACE_HPP
