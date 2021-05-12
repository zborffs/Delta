## Contains object for Delta Robot Params

"""
parameters for delta robot arm and delta robot models
"""
mutable struct DeltaRobotParams
    m1::Real # mass of first link (kg)
    m2::Real # mass of second link (kg)
    m3::Real # mass of end effector (kg)
    l1::Real # length of link 1 (m)
    l2::Real # length of link 2 (m)
    g::Real  # acceleration due to gravity (m/s^2)
    rb::Real # radius of base (m)
    rp::Real # radius of platform (m)
    d1::Real # damping between link 1 and base
    d2::Real # damping between link 1 and link 2 in phi2 direction
    d3::Real # damping between link 1 and link 2 in phi3 direction
    Kp::Real # proportional control constant
    Kd::Real # derivative control constant

    DeltaRobotParams(m1, m2, m3, l1, l2, g, rb=0.0, rp=0.0, d1=0.0, d2=0.0, d3=0.0, Kp=10.0, Kd=0.1) = new(m1, m2, m3, l1, l2, g, rb, rp, d1, d2, d3, Kp, Kd)
end
