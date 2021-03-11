using DifferentialEquations
using LinearAlgebra
using Plots
using DeltaRobot


# u0 =  [pi/4; pi/2; 0.0; pi/4; pi/2; 0.0; pi/4; pi/2; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 1.0; 1.0; 1.0; 1.0; 1.0; 1.0]; # what should lambda be?
u0 =  [pi/4; 3*pi/4; 0.0; pi/4; 3*pi/4; 0.0; pi/4; 3*pi/4; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 1.0; 1.0; 1.0; 1.0; 1.0; 1.0]; # what should lambda be?
du0 = [0.0; 0.0 ; 0.0 ; 0.0; 0.0 ; 0.0 ; 0.0; 0.0 ; 0.0 ; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0; 1.0; 1.0; 1.0; 1.0; 1.0; 1.0]; # why must I give this?
differential_vars = [true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, false, false, false, false, false, false]
@assert length(u0) == length(du0)
@assert length(u0) == length(differential_vars)
@assert u0[10:18] == du0[1:9] # make sure initial conditions are consistent
p = DeltaRobotArmParams(0.3, 0.2, 0.1, 0.5, 0.5, 9.8, 0.09, 0.09, 0.09);
tspan = (0.0, 20.0)
prob = DAEProblem(delta_robot!, du0, u0, tspan, p, differential_vars=differential_vars)
dt = 0.05
sol = solve(prob, saveat=dt)

gr()
anim = animate_robot(sol, p)
gif(anim, "animations/robot_1.gif", fps=convert(Int64, 1 / dt))

plotly()
plot(sol, vars=(1, 2, 3))
plot(sol.t, sol[1,:])
plot(sol.t, sol[2,:])
plot(sol.t, sol[3,:])

gr()
prob_damped = DAEProblem(delta_robot_damped!, du0, u0, tspan, p, differential_vars=differential_vars)
sol_damped = solve(prob_damped, saveat=dt)
anim = animate_robot(sol_damped, p)
gif(anim, "animations/damp_robot_1.gif", fps=convert(Int64, 1 / dt))
