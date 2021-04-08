"""
Script for simulating a delta robot when accounting for the Base and platform radii
    - Lagrangian Formulation
        - position and velocity initial conditions
        - robot parameters (lengths and masses)
        - time span to simulate over
    - Hamiltonian Formulation
        - position and velocity (velocities are internally converted to momentas)
        - robot parameters (lengths and masses)
        - time span to simulate over
"""
using DifferentialEquations
using LinearAlgebra
using Plots
using DeltaRobot
using BenchmarkTools
using Revise

# Set initial conditions
u0, du0, diff_vars = delta_robot_ics(
    [pi/4; 3*pi/4; 0.0; pi/4; 3*pi/4; 0.0; pi/4; 3*pi/4; 0.0], 
    [0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0], 
    [0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0]
)
@assert length(u0) == length(du0) # make sure position and velocity initial condition vectors have same dimensionality
@assert length(u0) == length(diff_vars) # make sure position and vector denoting which equations are differential equations have the same dimensionality
@assert u0[10:18] == du0[1:9] # make sure initial conditions are consistent
delta_robot_params = DeltaRobotParams(2.5, 3.0, 4.0, 0.312, 0.410, 9.8, 0.114, 0.051, 0.1, 0.1, 0.1); # Actual robot parameters with random d-params
tspan = (0.0, 30.0) # time span to simulate the system over
dt = 0.05 # time step

# Setup callbacks
# arm12_collision_cb = ContinuousCallback(DeltaRobot.arm12_collision_condition, DeltaRobot.arm12_collision_affect!)
# arm13_collision_cb = ContinuousCallback(DeltaRobot.arm13_collision_condition, DeltaRobot.arm13_collision_affect!)
# arm23_collision_cb = ContinuousCallback(DeltaRobot.arm23_collision_condition, DeltaRobot.arm23_collision_affect!)
# collisions_cbset = CallbackSet(arm12_collision_cb, arm13_collision_cb, arm23_collision_cb)

# Setup & solve the DAE under Lagrangian formalism
L_prob = DAEProblem(DeltaRobot.L_delta_robot_base!, du0, u0, tspan, delta_robot_params, differential_vars=diff_vars) # statement of the DAE problem
L_sol = solve(L_prob, saveat=dt)

# Animate the solution to the differential equation
anim_L = DeltaRobot.animate_robot(L_sol, delta_robot_params)
gif(anim_L, fps=1/dt)

# Setup and solve the DAE under the Hamiltonian formalism
H_prob = DAEProblem(DeltaRobot.H_delta_robot_base!, du0, u0, tspan, delta_robot_params, differential_vars=diff_vars)
H_sol = solve(H_prob, saveat=dt)

# Animfate the solution to the differential equation
anim_H = animate_robot(H_sol, delta_robot_params)
gif(anim_H, fps=1/dt)
