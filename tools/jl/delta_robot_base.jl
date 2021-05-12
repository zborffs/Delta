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
using Plots
using DifferentialEquations
using LinearAlgebra
using Sundials
using Revise
using DASSL
using DeltaRobot
using ModelingToolkit

# Set initial conditions
u0, du0, diff_vars = delta_robot_ics(
    [pi/4; 3*pi/4; 0.0; pi/4; 3*pi/4; 0.0; pi/4; 3*pi/4; 0.0],
    [0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0],
    [0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0]
)
@assert length(u0) == length(du0) # make sure position and velocity initial condition vectors have same dimensionality
@assert length(u0) == length(diff_vars) # make sure position and vector denoting which equations are differential equations have the same dimensionality
@assert u0[10:18] == du0[1:9] # make sure initial conditions are consistent
delta_robot_params = DeltaRobotParams(2.5, 3.0, 4.0, 0.312, 0.410, 9.8, 0.114, 0.051, 0.1, 0.1, 0.1, 100.0, 5.0); # Actual robot parameters with random d-params
tspan = (0.0, 20.0) # time span to simulate the system over
dt = 0.05 # time step

# Setup & solve the DAE under Lagrangian formalism
L_prob = DAEProblem(DeltaRobot.L_delta_robot_base!, du0, u0, tspan, delta_robot_params, differential_vars=diff_vars) # statement of the DAE problem
L_sol = solve(L_prob, IDA(), saveat=dt) # try :BCG

# Animate the solution to the differential equation
anim_L = DeltaRobot.animate_robot(L_sol, delta_robot_params)
gif(anim_L, fps=1 / dt)

# Second set of initial conditions
phi0 = deg2rad.([20., 20., 40.0])
x0 = DeltaRobot.for_kin(phi0, [3*pi/4, 0.0, 3*pi/4, 0.0, 3*pi/4, 0.0], delta_robot_params)
u0, du0, diff_vars = delta_robot_ics(x0[1], zeros(9), zeros(9))
@assert length(u0) == length(du0) # make sure position and velocity initial condition vectors have same dimensionality
@assert length(u0) == length(diff_vars) # make sure position and vector denoting which equations are differential equations have the same dimensionality
@assert u0[10:18] == du0[1:9] # make sure initial conditions are consistent
DeltaRobot.holonomic_constraint(x0[1], delta_robot_params)

# Setup & solve the DAE under Lagrangian formalism
L_prob = DAEProblem(DeltaRobot.L_delta_robot_base!, du0, u0, tspan, delta_robot_params, differential_vars=diff_vars) # statement of the DAE problem
L_sol = solve(L_prob, IDA(), saveat=dt)

# Animate the solution to the differential equation
anim_L = DeltaRobot.animate_robot(L_sol, delta_robot_params)
gif(anim_L, fps=1/dt)

# Gravity compensation
phi0 = deg2rad.([20., 20., 20.0])
x0 = DeltaRobot.for_kin(phi0, [3*pi/4, 0.0, 3*pi/4, 0.0, 3*pi/4, 0.0], delta_robot_params)
u0, du0, diff_vars = delta_robot_ics(x0[1], zeros(9), zeros(9))
@assert length(u0) == length(du0) # make sure position and velocity initial condition vectors have same dimensionality
@assert length(u0) == length(diff_vars) # make sure position and vector denoting which equations are differential equations have the same dimensionality
@assert u0[10:18] == du0[1:9] # make sure initial conditions are consistent
DeltaRobot.holonomic_constraint(x0[1], delta_robot_params)
grav_comp_prob = DAEProblem(DeltaRobot.gravity_compensated_model!, du0, u0, tspan, delta_robot_params, differential_vars=diff_vars) # statement of the DAE problem
grav_comp_sol = solve(grav_comp_prob,  IDA(linear_solver=:BCG), saveat=dt, reltol=1e-6,abstol=1e-6)

# Animate the solution to the differential equation
grav_comp_anim = DeltaRobot.animate_robot(grav_comp_sol, delta_robot_params)
gif(grav_comp_anim, fps=1/dt)

phi0 = deg2rad.([80., 80.0, 40.0])
x0 = DeltaRobot.for_kin(phi0, [3*pi/4, 0.0, 3*pi/4, 0.0, 3*pi/4, 0.0], delta_robot_params)
u0, du0, diff_vars = delta_robot_ics(x0[1], zeros(9), zeros(9))
@assert length(u0) == length(du0) # make sure position and velocity initial condition vectors have same dimensionality
@assert length(u0) == length(diff_vars) # make sure position and vector denoting which equations are differential equations have the same dimensionality
@assert u0[10:18] == du0[1:9] # make sure initial conditions are consistent
DeltaRobot.holonomic_constraint(x0[1], delta_robot_params)
grav_comp_prob = DAEProblem(DeltaRobot.gravity_compensated_model!, du0, u0, tspan, delta_robot_params, differential_vars=diff_vars) # statement of the DAE problem
grav_comp_sol = solve(grav_comp_prob,  IDA(linear_solver=:BCG), saveat=dt, reltol=1e-6,abstol=1e-6)

# Animate the solution to the differential equation
grav_comp_anim = DeltaRobot.animate_robot(grav_comp_sol, delta_robot_params)
gif(grav_comp_anim, fps=1/dt)

# PD 1
PD_prob = DAEProblem(DeltaRobot.gravity_compensated_model_PD!, du0, u0, tspan, delta_robot_params, differential_vars=diff_vars)
# IDA(), IDA(linear_solver=:BCG), DABDF2(), DImplicitEuler(), dassl()
PD_sol = solve(PD_prob, IDA(linear_solver=:GMRES), saveat=dt, reltol=1e-2*2, abstol=1e-2*2)
PD_anim = DeltaRobot.animate_robot(PD_sol, delta_robot_params)
gif(PD_anim, fps=1/dt)

# PD
phi0 = deg2rad.([0., 0., 0.])
x0 = DeltaRobot.for_kin(phi0, [3*pi/4, 0.0, 3*pi/4, 0.0, 3*pi/4, 0.0], delta_robot_params)
u0, du0, diff_vars = delta_robot_ics(x0[1], zeros(9), zeros(9))
@assert length(u0) == length(du0) # make sure position and velocity initial condition vectors have same dimensionality
@assert length(u0) == length(diff_vars) # make sure position and vector denoting which equations are differential equations have the same dimensionality
@assert u0[10:18] == du0[1:9] # make sure initial conditions are consistent
DeltaRobot.holonomic_constraint(x0[1], delta_robot_params)
PD_prob = DAEProblem(DeltaRobot.gravity_compensated_model_PD!, du0, u0, tspan, delta_robot_params, differential_vars=diff_vars)
# IDA(), IDA(linear_solver=:BCG), DABDF2(), DImplicitEuler(), dassl()
PD_sol = solve(PD_prob,  IDA(linear_solver=:BCG), saveat=dt, reltol=1e-4, abstol=1e-4)
PD_anim = DeltaRobot.animate_robot(PD_sol, delta_robot_params)
gif(PD_anim, fps=1/dt)

# Trying out automatic index reduction
using Plots;
using DifferentialEquations;
using LinearAlgebra;
using Sundials;
using Revise;
using DASSL;
using DeltaRobot;
using ModelingToolkit;
delta_robot_params = DeltaRobotParams(2.5, 3.0, 4.0, 0.312, 0.410, 9.8, 0.114, 0.051, 0.1, 0.1, 0.1, 10.0, 0.5); # Actual robot parameters with random d-params
phi0 = deg2rad.([20., 40., 40.]);
x0 = DeltaRobot.for_kin(phi0, [3*pi/4, 0.0, 3*pi/4, 0.0, 3*pi/4, 0.0], delta_robot_params);
u0, du0, diff_vars = delta_robot_ics(x0[1], zeros(9), zeros(9));
@assert length(u0) == length(du0); # make sure position and velocity initial condition vectors have same dimensionality
@assert length(u0) == length(diff_vars); # make sure position and vector denoting which equations are differential equations have the same dimensionality
@assert u0[10:18] == du0[1:9]; # make sure initial conditions are consistent
DeltaRobot.holonomic_constraint(x0[1], delta_robot_params);
p = [2.5, 3.0, 4.0, 0.312, 0.410, 9.8, 0.114, 0.051, 0.1, 0.1, 0.1, 10.0, 0.5];
myfunc = ODEFunction(DeltaRobot.gravity_compensated_model_PD_2!, mass_matrix=Diagonal(vcat(ones(18), zeros(6))));
tspan = (0.0, 5.0);
myprob = ODEProblem(myfunc, u0, tspan, p);
traced_sys = modelingtoolkitize(myprob);
mysys = structural_simplify(dae_index_lowering(traced_sys));
prob = ODEProblem(mysys, [], tspan); # haven't tried this one
sol = solve(prob, Rodas4()); # ODE: Rodas4(), DAE: Tsit5()

prob = ODAEProblem(mysys, [], tspan);
sol = solve(prob, Tsit5(), abstol=1e-8, reltol=1e-8);
# ODAEProblem
