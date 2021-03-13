## Script for simulating a Delta Robot
using DifferentialEquations
using LinearAlgebra
using Plots
using DeltaRobot
using BenchmarkTools

# Set initial conditions
u0, du0, diff_vars = delta_robot_ics([pi/4; 3*pi/4; 0.0; pi/4; 3*pi/4; 0.0; pi/4; 3*pi/4; 0.0], [0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0], [0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0])
@assert length(u0) == length(du0)
@assert length(u0) == length(diff_vars)
@assert u0[10:18] == du0[1:9] # make sure initial conditions are consistent
delta_robot_params = DeltaRobotParams(2.5, 3.0, 4.0, 0.312, 0.410, 9.8, 0.114, 0.051, 0.1, 0.1, 0.1); # Actual robot parameters with random d-params
tspan = (0.0, 30.0)

# setup callbacks
arm12_collision_cb = ContinuousCallback(DeltaRobot.arm12_collision_condition, DeltaRobot.arm12_collision_affect!)
arm13_collision_cb = ContinuousCallback(DeltaRobot.arm13_collision_condition, DeltaRobot.arm13_collision_affect!)
arm23_collision_cb = ContinuousCallback(DeltaRobot.arm23_collision_condition, DeltaRobot.arm23_collision_affect!)
collisions_cbset = CallbackSet(arm12_collision_cb, arm13_collision_cb, arm23_collision_cb)

# solve
prob = DAEProblem(DeltaRobot.L_delta_robot_base!, du0, u0, tspan, delta_robot_params, differential_vars=diff_vars, callback=collisions_cbset)
sol = solve(prob)

# animate
gr()
anim = DeltaRobot.animate_robot(sol, delta_robot_params)
gif(anim, fps=1/dt)





## hamiltonian

u0 = [pi/4; 3*pi/4; 0.0; pi/4; 3*pi/4; 0.0; pi/4; 3*pi/4; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0];
du0 = zeros(24);
probH = DAEProblem(DeltaRobot.H_delta_robot_base!, du0, u0, tspan, delta_robot_params, differential_vars=diff_vars)
solH = solve(probH, saveat=dt)

gr()
anim = animate_robot(solH, delta_robot_params)
gif(anim, fps=1/dt)

plotly()
plot(sol, vars=(1, 2, 3))
plot(sol.t, sol[1,:])
plot(sol.t, sol[2,:])
plot(sol.t, sol[3,:])
