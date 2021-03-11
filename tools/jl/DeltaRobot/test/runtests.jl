using DeltaRobot
using Test

@testset "Inverse Kinematics" begin
    p = DeltaRobotArmParams(0.3, 0.2, 0.1, 0.1, 0.3, 9.8, 0.075, 0.024)
    phi1, phi2, phi3 = inv_kin(0.013, -0.0226, -0.294011, p)
    phi1 = rad2deg(phi1)
    phi2 = rad2deg(phi2)
    phi3 = rad2deg(phi3)
end

@testset "Forward Kinematics" begin
    p = DeltaRobotArmParams(0.3, 0.2, 0.1, 0.1, 0.3, 9.8, 0.075, 0.024)
    phi1, phi2, phi3 = inv_kin(0.013, -0.0226, -0.294011, p)
    phi1 = rad2deg(phi1)
    phi2 = rad2deg(phi2)
    phi3 = rad2deg(phi3)
end
