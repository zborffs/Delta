module DeltaRobot

include("animation.jl")
# include("block_diagram.jl")
# include("control.jl")
include("delta_robot_params.jl")
# include("kinematics.jl")
include("simulation.jl")
include("stats.jl")

export transform_to_cartesian, animate_arm, animate_robot
export DeltaRobotParams
export delta_arm_ics, delta_arm!
export TABLE_HEADER, summary_data

end
