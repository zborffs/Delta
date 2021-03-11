## Contains functions for simulating the delta robot or a single delta robot arm
using LinearAlgebra, StaticArrays

"""
generates initial conditions for delta arm IVP
"""
function delta_arm_ics(u0::Array{Real, 1}, p::DeltaRobotParams)
    return u0
end

"""
generates initial conditions for delta robot IVP
"""
function delta_robot_ics(u0::Array{Real,1}, du0::Array{Real,1}, p::DeltaRobotParams)
    diff_args = zeros(0, 0)
    return u0, d0, diff_args
end

"""
delta_arm!(dx, d, p, t) defines the differential equation for single
delta robot arm with damping
"""
function delta_arm!(dx, x, p, t)
    # states of plant
    phi1 = x[1];
    phi2 = x[2];
    phi3 = x[3];

    phi1dot = x[4];
    phi2dot = x[5];
    phi3dot = x[6];

    # model params
    m1 = p.m1;
    m2 = p.m2;
    m3 = p.m3;
    l1 = p.l1;
    l2 = p.l2;
    g = p.g;

    # inertia matrix
    M1 = [(l1^2*(m1 + 3*m2 + m3))/3; (l1*l2*(sin(phi1)*sin(phi2) + cos(phi1)*cos(phi2)*cos(phi3))*(3*m2 + 2*m3))/6; -(l1*l2*cos(phi1)*sin(phi2)*sin(phi3)*(3*m2 + 2*m3))/6]';
    M2 = [(l1*l2*(sin(phi1)*sin(phi2) + cos(phi1)*cos(phi2)*cos(phi3))*(3*m2 + 2*m3))/6, (l2^2*(m2 + m3))/3, 0]';
    M3 = [-(l1*l2*cos(phi1)*sin(phi2)*sin(phi3)*(3*m2 + 2*m3))/6, 0, -(l2^2*(m2 + m3)*(cos(phi2)^2 - 1))/3]';
    M = [M1; M2; M3];

    # coriolis & gravitational matrices (basically external forces)
    b = [
    (g*l1*m1*cos(phi1))/2 + g*l1*m2*cos(phi1) + (g*l1*m3*cos(phi1))/3 - (l1*l2*m2*phi2dot^2*cos(phi2)*sin(phi1))/2 - (l1*l2*m3*phi2dot^2*cos(phi2)*sin(phi1))/3 + (l1*l2*m2*phi2dot^2*cos(phi1)*cos(phi3)*sin(phi2))/2 + (l1*l2*m2*phi3dot^2*cos(phi1)*cos(phi3)*sin(phi2))/2 + (l1*l2*m3*phi2dot^2*cos(phi1)*cos(phi3)*sin(phi2))/3 + (l1*l2*m3*phi3dot^2*cos(phi1)*cos(phi3)*sin(phi2))/3 + l1*l2*m2*phi2dot*phi3dot*cos(phi1)*cos(phi2)*sin(phi3) + (2*l1*l2*m3*phi2dot*phi3dot*cos(phi1)*cos(phi2)*sin(phi3))/3;
    (l2*(l2*m2*phi3dot^2*sin(2*phi2) + l2*m3*phi3dot^2*sin(2*phi2) + 3*g*m2*cos(phi2)*cos(phi3) + 2*g*m3*cos(phi2)*cos(phi3) - 3*l1*m2*phi1dot^2*cos(phi1)*sin(phi2) - 2*l1*m3*phi1dot^2*cos(phi1)*sin(phi2) + 3*l1*m2*phi1dot^2*cos(phi2)*cos(phi3)*sin(phi1) + 2*l1*m3*phi1dot^2*cos(phi2)*cos(phi3)*sin(phi1)))/6;
    -(l2*(3*g*m2*sin(phi2)*sin(phi3) + 2*g*m3*sin(phi2)*sin(phi3) + 2*l2*m2*phi2dot*phi3dot*sin(2*phi2) + 2*l2*m3*phi2dot*phi3dot*sin(2*phi2) + 3*l1*m2*phi1dot^2*sin(phi1)*sin(phi2)*sin(phi3) + 2*l1*m3*phi1dot^2*sin(phi1)*sin(phi2)*sin(phi3)))/6
    ];

    # Damping matrix
    D = zeros(3);
    D[1] = p.d1;
    D[2] = p.d2;
    D[3] = p.d3;

    # differential equation
    dx[1:3] = x[4:6];
    dx[4:6] = inv(M) * (b .- D .* x[4:6]);
end

"""
L_delta_robot!(out, dx, x, p, t) is the DAE update equation for the entire
delta robot under the lagrangian formulation
"""
function L_delta_robot!(out, dx, x, p, t)
    # states of plant
    q11 = x[1];
    q12 = x[2];
    q13 = x[3];
    q21 = x[4];
    q22 = x[5];
    q23 = x[6];
    q31 = x[7];
    q32 = x[8];
    q33 = x[9];
    q11dot = x[10];
    q12dot = x[11];
    q13dot = x[12];
    q21dot = x[13];
    q22dot = x[14];
    q23dot = x[15];
    q31dot = x[16];
    q32dot = x[17];
    q33dot = x[18];
    lambda1 = x[19];
    lambda2 = x[20];
    lambda3 = x[21];
    lambda4 = x[22];
    lambda5 = x[23];
    lambda6 = x[24];

    # model params
    m1 = p.m1;
    m2 = p.m2;
    m3 = p.m3;
    l1 = p.l1;
    l2 = p.l2;
    g = p.g;

    # 'D' elements
    M1 = [(l1^2*(m1 + 3*m2 + m3))/3, (l1*l2*(sin(q11)*sin(q12) + cos(q11)*cos(q12)*cos(q13))*(3*m2 + 2*m3))/6, -(l1*l2*cos(q11)*sin(q12)*sin(q13)*(3*m2 + 2*m3))/6]';
    M2 = [(l1*l2*(sin(q11)*sin(q12) + cos(q11)*cos(q12)*cos(q13))*(3*m2 + 2*m3))/6, (l2^2*(m2 + m3))/3, 0]';
    M3 = [-(l1*l2*cos(q11)*sin(q12)*sin(q13)*(3*m2 + 2*m3))/6, 0, -(l2^2*(m2 + m3)*(cos(q12)^2 - 1))/3]';
    Mtop = [M1; M2; M3];
    M4 = [(l1^2*(m1 + 3*m2 + m3))/3, (l1*l2*(sin(q21)*sin(q22) + cos(q21)*cos(q22)*cos(q23))*(3*m2 + 2*m3))/6, -(l1*l2*cos(q21)*sin(q22)*sin(q23)*(3*m2 + 2*m3))/6]';
    M5 = [(l1*l2*(sin(q21)*sin(q22) + cos(q21)*cos(q22)*cos(q23))*(3*m2 + 2*m3))/6, (l2^2*(m2 + m3))/3, 0]';
    M6 = [-(l1*l2*cos(q21)*sin(q22)*sin(q23)*(3*m2 + 2*m3))/6, 0, -(l2^2*(m2 + m3)*(cos(q22)^2 - 1))/3]';
    Mmid = [M4; M5; M6];
    M7 = [(l1^2*(m1 + 3*m2 + m3))/3, (l1*l2*(sin(q31)*sin(q32) + cos(q31)*cos(q32)*cos(q33))*(3*m2 + 2*m3))/6, -(l1*l2*cos(q31)*sin(q32)*sin(q33)*(3*m2 + 2*m3))/6]';
    M8 = [(l1*l2*(sin(q31)*sin(q32) + cos(q31)*cos(q32)*cos(q33))*(3*m2 + 2*m3))/6, (l2^2*(m2 + m3))/3, 0]';
    M9 = [-(l1*l2*cos(q31)*sin(q32)*sin(q33)*(3*m2 + 2*m3))/6, 0, -(l2^2*(m2 + m3)*(cos(q32)^2 - 1))/3]';
    Mbot = [M7; M8; M9];
    invMtop = inv(Mtop);
    invMmid = inv(Mmid);
    invMbot = inv(Mbot);
    invM1 = [invMtop[1,:]; 0; 0; 0; 0; 0; 0]';
    invM2 = [invMtop[2,:]; 0; 0; 0; 0; 0; 0]';
    invM3 = [invMtop[3,:]; 0; 0; 0; 0; 0; 0]';
    invM4 = [0; 0; 0; invMmid[1,:]; 0; 0; 0]';
    invM5 = [0; 0; 0; invMmid[2,:]; 0; 0; 0]';
    invM6 = [0; 0; 0; invMmid[3,:]; 0; 0; 0]';
    invM7 = [0; 0; 0; 0; 0; 0; invMbot[1,:]]';
    invM8 = [0; 0; 0; 0; 0; 0; invMbot[2,:]]';
    invM9 = [0; 0; 0; 0; 0; 0; invMbot[3,:]]';
    invM = [invM1; invM2; invM3; invM4; invM5; invM6; invM7; invM8; invM9]

    # External Forces
    b = [
        (l1*(6*lambda3*cos(q11) + 6*lambda6*cos(q11) - 6*lambda2*sin(q11) - 6*lambda5*sin(q11) + 3*g*m1*cos(q11) + 6*g*m2*cos(q11) + 2*g*m3*cos(q11) - 3*l2*m2*q12dot^2*cos(q12)*sin(q11) - 2*l2*m3*q12dot^2*cos(q12)*sin(q11) + 3*l2*m2*q12dot^2*cos(q11)*cos(q13)*sin(q12) + 3*l2*m2*q13dot^2*cos(q11)*cos(q13)*sin(q12) + 2*l2*m3*q12dot^2*cos(q11)*cos(q13)*sin(q12) + 2*l2*m3*q13dot^2*cos(q11)*cos(q13)*sin(q12) + 6*l2*m2*q12dot*q13dot*cos(q11)*cos(q12)*sin(q13) + 4*l2*m3*q12dot*q13dot*cos(q11)*cos(q12)*sin(q13)))/6;
        (l2*(6*lambda3*cos(q12)*cos(q13) - 6*lambda5*sin(q12) - 6*lambda2*sin(q12) + 6*lambda6*cos(q12)*cos(q13) + 6*lambda1*cos(q12)*sin(q13) + 6*lambda4*cos(q12)*sin(q13) + l2*m2*q13dot^2*sin(2*q12) + l2*m3*q13dot^2*sin(2*q12) + 3*g*m2*cos(q12)*cos(q13) + 2*g*m3*cos(q12)*cos(q13) - 3*l1*m2*q11dot^2*cos(q11)*sin(q12) - 2*l1*m3*q11dot^2*cos(q11)*sin(q12) + 3*l1*m2*q11dot^2*cos(q12)*cos(q13)*sin(q11) + 2*l1*m3*q11dot^2*cos(q12)*cos(q13)*sin(q11)))/6;
        -(l2*(6*lambda3*sin(q12)*sin(q13) - 6*lambda4*cos(q13)*sin(q12) - 6*lambda1*cos(q13)*sin(q12) + 6*lambda6*sin(q12)*sin(q13) + 3*g*m2*sin(q12)*sin(q13) + 2*g*m3*sin(q12)*sin(q13) + 2*l2*m2*q12dot*q13dot*sin(2*q12) + 2*l2*m3*q12dot*q13dot*sin(2*q12) + 3*l1*m2*q11dot^2*sin(q11)*sin(q12)*sin(q13) + 2*l1*m3*q11dot^2*sin(q11)*sin(q12)*sin(q13)))/6;
        (l1*(3*g*m1*cos(q21) - 3*lambda2*sin(q21) - 6*lambda3*cos(q21) + 6*g*m2*cos(q21) + 2*g*m3*cos(q21) + 3*3^(1/2)*lambda1*sin(q21) - 3*l2*m2*q22dot^2*cos(q22)*sin(q21) - 2*l2*m3*q22dot^2*cos(q22)*sin(q21) + 3*l2*m2*q22dot^2*cos(q21)*cos(q23)*sin(q22) + 3*l2*m2*q23dot^2*cos(q21)*cos(q23)*sin(q22) + 2*l2*m3*q22dot^2*cos(q21)*cos(q23)*sin(q22) + 2*l2*m3*q23dot^2*cos(q21)*cos(q23)*sin(q22) + 6*l2*m2*q22dot*q23dot*cos(q21)*cos(q22)*sin(q23) + 4*l2*m3*q22dot*q23dot*cos(q21)*cos(q22)*sin(q23)))/6;
        (l2*(3*3^(1/2)*lambda1*sin(q22) - 3*lambda2*sin(q22) - 6*lambda3*cos(q22)*cos(q23) + 3*lambda1*cos(q22)*sin(q23) + l2*m2*q23dot^2*sin(2*q22) + l2*m3*q23dot^2*sin(2*q22) + 3*g*m2*cos(q22)*cos(q23) + 2*g*m3*cos(q22)*cos(q23) + 3*3^(1/2)*lambda2*cos(q22)*sin(q23) - 3*l1*m2*q21dot^2*cos(q21)*sin(q22) - 2*l1*m3*q21dot^2*cos(q21)*sin(q22) + 3*l1*m2*q21dot^2*cos(q22)*cos(q23)*sin(q21) + 2*l1*m3*q21dot^2*cos(q22)*cos(q23)*sin(q21)))/6;
        -(l2*(3*g*m2*sin(q22)*sin(q23) - 6*lambda3*sin(q22)*sin(q23) - 3*3^(1/2)*lambda2*cos(q23)*sin(q22) - 3*lambda1*cos(q23)*sin(q22) + 2*g*m3*sin(q22)*sin(q23) + 2*l2*m2*q22dot*q23dot*sin(2*q22) + 2*l2*m3*q22dot*q23dot*sin(2*q22) + 3*l1*m2*q21dot^2*sin(q21)*sin(q22)*sin(q23) + 2*l1*m3*q21dot^2*sin(q21)*sin(q22)*sin(q23)))/6;
        (l1*(3*g*m1*cos(q31) - 3*lambda5*sin(q31) - 6*lambda6*cos(q31) + 6*g*m2*cos(q31) + 2*g*m3*cos(q31) - 3*3^(1/2)*lambda4*sin(q31) - 3*l2*m2*q32dot^2*cos(q32)*sin(q31) - 2*l2*m3*q32dot^2*cos(q32)*sin(q31) + 3*l2*m2*q32dot^2*cos(q31)*cos(q33)*sin(q32) + 3*l2*m2*q33dot^2*cos(q31)*cos(q33)*sin(q32) + 2*l2*m3*q32dot^2*cos(q31)*cos(q33)*sin(q32) + 2*l2*m3*q33dot^2*cos(q31)*cos(q33)*sin(q32) + 6*l2*m2*q32dot*q33dot*cos(q31)*cos(q32)*sin(q33) + 4*l2*m3*q32dot*q33dot*cos(q31)*cos(q32)*sin(q33)))/6;
        (l2*(3*lambda4*cos(q32)*sin(q33) - 3*3^(1/2)*lambda4*sin(q32) - 6*lambda6*cos(q32)*cos(q33) - 3*lambda5*sin(q32) + l2*m2*q33dot^2*sin(2*q32) + l2*m3*q33dot^2*sin(2*q32) + 3*g*m2*cos(q32)*cos(q33) + 2*g*m3*cos(q32)*cos(q33) - 3*3^(1/2)*lambda5*cos(q32)*sin(q33) - 3*l1*m2*q31dot^2*cos(q31)*sin(q32) - 2*l1*m3*q31dot^2*cos(q31)*sin(q32) + 3*l1*m2*q31dot^2*cos(q32)*cos(q33)*sin(q31) + 2*l1*m3*q31dot^2*cos(q32)*cos(q33)*sin(q31)))/6;
        -(l2*(3*3^(1/2)*lambda5*cos(q33)*sin(q32) - 6*lambda6*sin(q32)*sin(q33) - 3*lambda4*cos(q33)*sin(q32) + 3*g*m2*sin(q32)*sin(q33) + 2*g*m3*sin(q32)*sin(q33) + 2*l2*m2*q32dot*q33dot*sin(2*q32) + 2*l2*m3*q32dot*q33dot*sin(2*q32) + 3*l1*m2*q31dot^2*sin(q31)*sin(q32)*sin(q33) + 2*l1*m3*q31dot^2*sin(q31)*sin(q32)*sin(q33)))/6
    ];

    # Holonomic Constraint
    h = [
        l2*sin(q12)*sin(q13) - (3^(1/2)*(l1*cos(q21) + l2*cos(q22)))/2 + (l2*sin(q22)*sin(q23))/2;
        l1*cos(q11) + l2*cos(q12) + (l1*cos(q21))/2 + (l2*cos(q22))/2 + (3^(1/2)*l2*sin(q22)*sin(q23))/2;
        l1*sin(q11) - l1*sin(q21) + l2*cos(q13)*sin(q12) - l2*cos(q23)*sin(q22);
        (3^(1/2)*(l1*cos(q31) + l2*cos(q32)))/2 + l2*sin(q12)*sin(q13) + (l2*sin(q32)*sin(q33))/2;
        l1*cos(q11) + l2*cos(q12) + (l1*cos(q31))/2 + (l2*cos(q32))/2 - (3^(1/2)*l2*sin(q32)*sin(q33))/2;
        l1*sin(q11) - l1*sin(q31) + l2*cos(q13)*sin(q12) - l2*cos(q33)*sin(q32)
    ];

    # Jacobian of Holonomic Constraint w.r.t q
    H1 = [0, l2*cos(q12)*sin(q13), l2*cos(q13)*sin(q12), (3^(1/2)*l1*sin(q21))/2, (3^(1/2)*l2*sin(q22))/2 + (l2*cos(q22)*sin(q23))/2, (l2*cos(q23)*sin(q22))/2, 0, 0, 0]';
    H2 = [-l1*sin(q11), -l2*sin(q12), 0, -(l1*sin(q21))/2, (3^(1/2)*l2*cos(q22)*sin(q23))/2 - (l2*sin(q22))/2, (3^(1/2)*l2*cos(q23)*sin(q22))/2, 0, 0, 0]';
    H3 = [l1*cos(q11), l2*cos(q12)*cos(q13), -l2*sin(q12)*sin(q13), -l1*cos(q21), -l2*cos(q22)*cos(q23), l2*sin(q22)*sin(q23), 0, 0, 0]';
    H4 = [0, l2*cos(q12)*sin(q13), l2*cos(q13)*sin(q12), 0, 0, 0, -(3^(1/2)*l1*sin(q31))/2, (l2*cos(q32)*sin(q33))/2 - (3^(1/2)*l2*sin(q32))/2, (l2*cos(q33)*sin(q32))/2]';
    H5 = [-l1*sin(q11), -l2*sin(q12), 0, 0, 0, 0, -(l1*sin(q31))/2, - (l2*sin(q32))/2 - (3^(1/2)*l2*cos(q32)*sin(q33))/2, -(3^(1/2)*l2*cos(q33)*sin(q32))/2]';
    H6 = [l1*cos(q11), l2*cos(q12)*cos(q13), -l2*sin(q12)*sin(q13), 0, 0, 0, -l1*cos(q31), -l2*cos(q32)*cos(q33), l2*sin(q32)*sin(q33)]';
    H = [
        H1; H2; H3; H4; H5; H6
    ];

    # Time Derivative of Jacobian w.r.t of Holonomic Constraint
    Hdot1 = [0, l2*q13dot*cos(q12)*cos(q13) - l2*q12dot*sin(q12)*sin(q13), l2*q12dot*cos(q12)*cos(q13) - l2*q13dot*sin(q12)*sin(q13), (3^(1/2)*l1*q21dot*cos(q21))/2, (l2*q23dot*cos(q22)*cos(q23))/2 - q22dot*((l2*sin(q22)*sin(q23))/2 - (3^(1/2)*l2*cos(q22))/2), (l2*q22dot*cos(q22)*cos(q23))/2 - (l2*q23dot*sin(q22)*sin(q23))/2, 0, 0, 0]';
    Hdot2 = [-l1*q11dot*cos(q11), -l2*q12dot*cos(q12), 0, -(l1*q21dot*cos(q21))/2, (3^(1/2)*l2*q23dot*cos(q22)*cos(q23))/2 - q22dot*((l2*cos(q22))/2 + (3^(1/2)*l2*sin(q22)*sin(q23))/2), (3^(1/2)*l2*q22dot*cos(q22)*cos(q23))/2 - (3^(1/2)*l2*q23dot*sin(q22)*sin(q23))/2, 0, 0, 0]';
    Hdot3 = [-l1*q11dot*sin(q11), - l2*q12dot*cos(q13)*sin(q12) - l2*q13dot*cos(q12)*sin(q13), - l2*q12dot*cos(q12)*sin(q13) - l2*q13dot*cos(q13)*sin(q12), l1*q21dot*sin(q21), l2*q22dot*cos(q23)*sin(q22) + l2*q23dot*cos(q22)*sin(q23), l2*q22dot*cos(q22)*sin(q23) + l2*q23dot*cos(q23)*sin(q22), 0, 0, 0]';
    Hdot4 = [0, l2*q13dot*cos(q12)*cos(q13) - l2*q12dot*sin(q12)*sin(q13), l2*q12dot*cos(q12)*cos(q13) - l2*q13dot*sin(q12)*sin(q13), 0, 0, 0, -(3^(1/2)*l1*q31dot*cos(q31))/2, (l2*q33dot*cos(q32)*cos(q33))/2 - q32dot*((l2*sin(q32)*sin(q33))/2 + (3^(1/2)*l2*cos(q32))/2), (l2*q32dot*cos(q32)*cos(q33))/2 - (l2*q33dot*sin(q32)*sin(q33))/2]';
    Hdot5 = [-l1*q11dot*cos(q11), -l2*q12dot*cos(q12), 0, 0, 0, 0, -(l1*q31dot*cos(q31))/2, - q32dot*((l2*cos(q32))/2 - (3^(1/2)*l2*sin(q32)*sin(q33))/2) - (3^(1/2)*l2*q33dot*cos(q32)*cos(q33))/2, (3^(1/2)*l2*q33dot*sin(q32)*sin(q33))/2 - (3^(1/2)*l2*q32dot*cos(q32)*cos(q33))/2]';
    Hdot6 = [-l1*q11dot*sin(q11), - l2*q12dot*cos(q13)*sin(q12) - l2*q13dot*cos(q12)*sin(q13), - l2*q12dot*cos(q12)*sin(q13) - l2*q13dot*cos(q13)*sin(q12), 0, 0, 0, l1*q31dot*sin(q31), l2*q32dot*cos(q33)*sin(q32) + l2*q33dot*cos(q32)*sin(q33), l2*q32dot*cos(q32)*sin(q33) + l2*q33dot*cos(q33)*sin(q32)]';
    Hdot = [
        Hdot1; Hdot2; Hdot3; Hdot4; Hdot5; Hdot6
    ];

    D = zeros(9);
    D[1] = p.d1;
    D[2] = p.d2;
    D[3] = p.d3;
    D[4] = p.d1;
    D[5] = p.d2;
    D[6] = p.d3;
    D[7] = p.d1;
    D[8] = p.d2;
    D[9] = p.d3;

    invD_times_b = invD * (b .- D * x[10:18]); # invD_times_b = invD * b;
    z0 = h;
    z1 = H * x[10:18];
    z2 = Hdot * x[10:18] .+ H * invD_times_b;
    z = z0 + z1 + z2;

    out[1:9] = x[10:18] .- dx[1:9]; # first derivative residual
    out[10:18] = invD_times_b .- dx[10:18]; # second derivative residual
    out[19:24] = z; # index reduced holonomic constraint
end

"""
H_delta_robot!(out, dx, x, p, t) is the DAE update equation for the entire
delta robot under the hamiltonian formulation
"""
function H_delta_robot!(out, dx, x, p, t)
end

"""
different callbacks for step response etc.
"""
