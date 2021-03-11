using DifferentialEquations
using Plots
using DeltaRobot

# State and solve ODE
u0 = [0.01; 0.5; pi/2; 0.0; 0.0; 0.0] # ICs: (ϕ1, ϕ2, ϕ3, ϕ1dot, ...)
tspan = (0.0, 50.0) # time span to simulate system over
dt = 0.05 # sample solution every 0.05 seconds
no_damped_plant = DeltaRobotParams(0.3, 0.2, 0.1, 0.5, 0.5, 9.8) # model parameters
no_damped_prob = ODEProblem(delta_arm!, u0, tspan, no_damped_plant) # define ODE problem
no_damped_sol = solve(no_damped_prob, saveat=dt) # solve ode

# Animate solution to ODE
gr()
no_damped_anim = animate_arm(no_damped_sol, no_damped_plant) # animate the solution
gif(no_damped_anim, "../res/animations/no_damp_1.gif", fps=convert(Int64, 1 / dt)) # make a gif

# Plot solution to ODE
plotly()
plt = plot(no_damped_sol, vars=(1, 2, 3)) # make phase plot
savefig(plt, "plots/no_damp_phaseplot_1.html")
plt = plot(no_damped_sol.t, no_damped_sol[1,:]) # evolution of phi1 w.r.t time
savefig(plt, "plots/no_damp_phi1_1.html")
plt = plot(no_damped_sol.t, no_damped_sol[2,:]) # evolution of phi2 w.r.t time
savefig(plt, "plots/no_damp_phi2_1.html")
plt = plot(no_damped_sol.t, no_damped_sol[3,:]) # evolution of phi3 w.r.t time
savefig(plt, "plots/no_damp_phi3_1.html")

# State and solve ODE
damped_plant = DeltaRobotParams(0.3, 0.2, 0.1, 0.5, 0.5, 9.8, 0.05, 0.05, 0.05) # model parameters
damped_prob = ODEProblem(delta_arm!, u0, tspan, damped_plant) # define ODE problem
damped_sol = solve(damped_prob, saveat=dt) # solve ode

# Animate solution to ODE
gr()
damped_anim = animate_arm(damped_sol, damped_plant) # animate the solution
gif(damped_anim, "animations/damp_1.gif", fps=convert(Int64, 1 / dt)) # make a gif

# Plot solution to ODE
plotly()
plt = plot(damped_sol, vars=(1, 2, 3)) # make phase plot
savefig(plt, "plots/damp_phaseplot_1.html")
plt = plot(damped_sol.t, damped_sol[1,:]) # evolution of phi1 w.r.t time
savefig(plt, "plots/damp_phi1_1.html")
plt = plot(damped_sol.t, damped_sol[2,:]) # evolution of phi2 w.r.t time
savefig(plt, "plots/damp_phi2_1.html")
plt = plot(damped_sol.t, damped_sol[3,:]) # evolution of phi3 w.r.t time
savefig(plt, "plots/damp_phi3_1.html")
