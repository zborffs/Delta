module DeltaRobot

include("animation.jl")
# include("block_diagram.jl")
# include("control.jl")
include("delta_robot_params.jl")
include("kinematics.jl")
include("simulation.jl")
include("stats.jl")

export transform_to_cartesian, animate_arm, animate_robot
export DeltaRobotParams
export delta_arm_ics, delta_robot_ics, delta_arm!, L_delta_robot!, L_delta_robot_base!, H_delta_robot!
export TABLE_HEADER, summary_data

end
