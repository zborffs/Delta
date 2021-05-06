using DeltaRobot
using Test

delta_robot_params = DeltaRobotParams(2.5, 3.0, 4.0, 0.312, 0.410, 9.8, 0.114, 0.051, 0.0106, 0.0106, 0.0106); # Actual robot parameters with random d-params

@testset "Inverse Kinematics" begin

end

@testset "Forward Kinematics" begin
    p = DeltaRobotArmParams(0.3, 0.2, 0.1, 0.1, 0.3, 9.8, 0.075, 0.024)

end
