## Contains object for Delta Robot Params

"""
parameters for delta robot arm and delta robot models
"""
mutable struct DeltaRobotParams
    m1::Float64 # mass of first link (kg)
    m2::Float64 # mass of second link (kg)
    m3::Float64 # mass of end effector (kg)
    l1::Float64 # length of link 1 (m)
    l2::Float64 # length of link 2 (m)
    g::Float64  # acceleration due to gravity (m/s^2)
    rb::Float64 # radius of base (m)
    rp::Float64 # radius of platform (m)
    d1::Float64 # damping between link 1 and base
    d2::Float64 # damping between link 1 and link 2 in phi2 direction
    d3::Float64 # damping between link 1 and link 2 in phi3 direction

    DeltaRobotParams(m1, m2, m3, l1, l2, g, rb=0.0, rp=0.0, d1=0.0, d2=0.0, d3=0.0) = new(m1, m2, m3, l1, l2, g, rb, rp, d1, d2, d3)
end
