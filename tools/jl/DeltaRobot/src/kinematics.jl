using LinearAlgebra, StaticArrays

"""
implements inverse kinematics for delta robot
"""
function inv_kin(xe, ye, ze, p::DeltaRobotParams)
    Rl = p.l2
    Ru = p.l1
    Rb = p.rb
    Rp = p.rp

    # first arm
    f_z1_pos(z, ye, ze, Rl, Ru, Rb, Rp) = (z - ze)^2 + (Rb - Rp - ye + (Ru^2 - z^2)^(1/2))^2 - Rl^2 + xe^2
    f_z1_neg(z, ye, ze, Rl, Ru, Rb, Rp) = (z - ze)^2 - Rl^2 + xe^2 + (Rp - Rb + ye + (Ru^2 - z^2)^(1/2))^2
    f_z1_pos_prime(z, ye, ze, Rl, Ru, Rb, Rp) = 2*z - 2*ze - (2*z*(Rb - Rp - ye + (Ru^2 - z^2)^(1/2)))/(Ru^2 - z^2)^(1/2)
    f_z1_neg_prime(z, ye, ze, Rl, Ru, Rb, Rp) = 2*z - 2*ze - (2*z*(Rp - Rb + ye + (Ru^2 - z^2)^(1/2)))/(Ru^2 - z^2)^(1/2)
    f_z2_pos(z, ye, ze, Rl, Ru, Rb, Rp) = (z - ze)^2 + (xe/2 + (3^(1/2)*(Rp + ye))/2)^2 + (Rp/2 - Rb/2 + ye/2 + (- 3*Rb^2 + 4*Ru^2 - 4*z^2)^(1/2)/2 - (3^(1/2)*xe)/2)^2 - Rl^2
    f_z2_neg(z, ye, ze, Rl, Ru, Rb, Rp) = (z - ze)^2 + (xe/2 + (3^(1/2)*(Rp + ye))/2)^2 + (Rb/2 - Rp/2 - ye/2 + (- 3*Rb^2 + 4*Ru^2 - 4*z^2)^(1/2)/2 + (3^(1/2)*xe)/2)^2 - Rl^2
    f_z2_pos_prime(z, ye, ze, Rl, Ru, Rb, Rp) = 2*z - 2*ze - (4*z*(Rp/2 - Rb/2 + ye/2 + (- 3*Rb^2 + 4*Ru^2 - 4*z^2)^(1/2)/2 - (3^(1/2)*xe)/2))/(- 3*Rb^2 + 4*Ru^2 - 4*z^2)^(1/2)
    f_z2_neg_prime(z, ye, ze, Rl, Ru, Rb, Rp) = 2*z - 2*ze - (4*z*(Rb/2 - Rp/2 - ye/2 + (- 3*Rb^2 + 4*Ru^2 - 4*z^2)^(1/2)/2 + (3^(1/2)*xe)/2))/(- 3*Rb^2 + 4*Ru^2 - 4*z^2)^(1/2)
    f_z3_pos(z, ye, ze, Rl, Ru, Rb, Rp) = (z - ze)^2 + (xe/2 - (3^(1/2)*(Rp + ye))/2)^2 + (Rp/2 - Rb/2 + ye/2 + (- 3*Rb^2 + 4*Ru^2 - 4*z^2)^(1/2)/2 + (3^(1/2)*xe)/2)^2 - Rl^2
    f_z3_neg(z, ye, ze, Rl, Ru, Rb, Rp) = (z - ze)^2 + (xe/2 - (3^(1/2)*(Rp + ye))/2)^2 + (Rp/2 - Rb/2 + ye/2 - (- 3*Rb^2 + 4*Ru^2 - 4*z^2)^(1/2)/2 + (3^(1/2)*xe)/2)^2 - Rl^2
    f_z3_pos_prime(z, ye, ze, Rl, Ru, Rb, Rp) = 2*z - 2*ze - (4*z*(Rp/2 - Rb/2 + ye/2 + (- 3*Rb^2 + 4*Ru^2 - 4*z^2)^(1/2)/2 + (3^(1/2)*xe)/2))/(- 3*Rb^2 + 4*Ru^2 - 4*z^2)^(1/2)
    f_z3_neg_prime(z, ye, ze, Rl, Ru, Rb, Rp) = 2*z - 2*ze + (4*z*(Rp/2 - Rb/2 + ye/2 - (- 3*Rb^2 + 4*Ru^2 - 4*z^2)^(1/2)/2 + (3^(1/2)*xe)/2))/(- 3*Rb^2 + 4*Ru^2 - 4*z^2)^(1/2)
    left_half_plane(ϕ) = ϕ > π/2 || ϕ < -π/2
    z2ϕ(z) = asin(z / Ru)
    tup = (ye, ze, Rl, Ru, Rb, Rp)
    z1_pos = newton(f_z1_pos, Ru/sqrt(2), f_z1_pos_prime, tup);
    z1_neg = newton(f_z1_neg, -Ru/sqrt(2), f_z1_neg_prime, tup);
    # z1_neg = newton(f_z1_pos, 0.0, f_z1_pos_prime, tup);

    ϕ1pos = z2ϕ(z1_pos)
    ϕ1neg = z2ϕ(z1_neg)
    println("ϕ1pos = $(rad2deg(ϕ1pos)), ϕ1neg = $(rad2deg(ϕ1neg))")
    # @assert((left_half_plane(ϕ1neg) && !left_half_plane(ϕ1pos)) || (left_half_plane(ϕ1pos) && !left_half_plane(ϕ1neg)))

    ϕ1 = left_half_plane(ϕ1pos) ? ϕ1pos : ϕ1neg

    # second arm
    z2_pos = newton(f_z2_pos, Ru/sqrt(2), f_z2_pos_prime, tup);
    z2_neg = newton(f_z2_neg, -Ru/sqrt(2), f_z2_neg_prime, tup);

    ϕ2pos = z2ϕ(z2_pos)
    ϕ2neg = z2ϕ(z2_neg)
    println("ϕ2pos = $(rad2deg(ϕ2pos)), ϕ2neg = $(rad2deg(ϕ2neg))")
    # @assert((left_half_plane(ϕ2neg) && !left_half_plane(ϕ2pos)) || (left_half_plane(ϕ2pos) && !left_half_plane(ϕ2neg)))

    ϕ2 = left_half_plane(ϕ2pos) ? ϕ2pos : ϕ2neg

    # third arm
    z3_pos = newton(f_z3_pos, Ru/sqrt(2), f_z3_pos_prime, tup);
    z3_neg = newton(f_z3_neg, -Ru/sqrt(2), f_z3_neg_prime, tup);

    ϕ3pos = z2ϕ(z3_pos)
    ϕ3neg = z2ϕ(z3_neg)
    println("ϕ3pos = $(rad2deg(ϕ3pos)), ϕ3neg = $(rad2deg(ϕ3neg))")
    # @assert((left_half_plane(ϕ3neg) && !left_half_plane(ϕ3pos)) || (left_half_plane(ϕ3pos) && !left_half_plane(ϕ3neg)))

    ϕ3 = left_half_plane(ϕ3pos) ? ϕ3pog : ϕ3neg

    return ϕ1, ϕ2, ϕ3
end

"""
computes the forward kinematics of the delta robot from the measured joint parameters phi, an initial guess
of the joint parameters x0, and the delta robot parameters p; the user may optionally pass in 2 additional 
keyword arguments, namely abstol, which determines the absolute tolerance of the algorithm, and the maximum
number of iterations one would wish to take place

ex:
ee1, ee2, ee3, h1, h2, i, eps = for_kin(phi, [0.01,0.01,0.01,0.01,0.01,0.01], p)

- note on usage: if the initial guess vector is the zero vector, then one of the rows of the jacobian
  matrix becomes 0, which makes the inversion ill-posed
    - always initialize the matrix with a "good guess" other otherwise with at least 1 non-zero element
"""
function for_kin(phi::Array{T, 1}, x0::Array{T, 1}, p::DeltaRobotParams; abstol=1e-12, max_iter=50) where T <: Real
    @assert length(phi) == 3
    @assert length(x0) == 6

    q11 = phi[1]
    q21 = phi[2]
    q31 = phi[3]
    ϵ = nothing
    l1 = p.l1
    l2 = p.l2
    r_base = p.rb
    r_platform = p.rp
    i = 0
    
    while (ϵ === nothing || abs(ϵ) > abstol) && i < max_iter
        q12 = x0[1];
        q13 = x0[2];
        q22 = x0[3];
        q23 = x0[4];
        q32 = x0[5];
        q33 = x0[6];

        J = @SMatrix [
            l2*cos(q12)*sin(q13) l2*cos(q13)*sin(q12) (l2*(cos(q22)*sin(q23) + 3^(1/2)*sin(q22)))/2 (l2*cos(q23)*sin(q22))/2 0 0; 
            -l2*sin(q12) 0 -(l2*(sin(q22) - 3^(1/2)*cos(q22)*sin(q23)))/2 (3^(1/2)*l2*cos(q23)*sin(q22))/2 0 0; 
            l2*cos(q12)*cos(q13) -l2*sin(q12)*sin(q13) -l2*cos(q22)*cos(q23) l2*sin(q22)*sin(q23) 0 0; 
            l2*cos(q12)*sin(q13) l2*cos(q13)*sin(q12) 0 0 (l2*(cos(q32)*sin(q33) - 3^(1/2)*sin(q32)))/2 (l2*cos(q33)*sin(q32))/2; 
            -l2*sin(q12) 0 0 0 -(l2*(sin(q32) + 3^(1/2)*cos(q32)*sin(q33)))/2 -(3^(1/2)*l2*cos(q33)*sin(q32))/2; 
            l2*cos(q12)*cos(q13) -l2*sin(q12)*sin(q13) 0 0 -l2*cos(q32)*cos(q33) l2*sin(q32)*sin(q33)
        ];

        h = SVector{6}([
            l2*sin(q12)*sin(q13) - (3^(1/2)*(r_base - r_platform + l1*cos(q21) + l2*cos(q22)))/2 + (l2*sin(q22)*sin(q23))/2; 
            (3*r_base)/2 - (3*r_platform)/2 + l1*cos(q11) + l2*cos(q12) + (l1*cos(q21))/2 + (l2*cos(q22))/2 + (3^(1/2)*l2*sin(q22)*sin(q23))/2; 
            l1*sin(q11) - l1*sin(q21) + l2*cos(q13)*sin(q12) - l2*cos(q23)*sin(q22); 
            (3^(1/2)*(r_base - r_platform + l1*cos(q31) + l2*cos(q32)))/2 + l2*sin(q12)*sin(q13) + (l2*sin(q32)*sin(q33))/2; 
            (3*r_base)/2 - (3*r_platform)/2 + l1*cos(q11) + l2*cos(q12) + (l1*cos(q31))/2 + (l2*cos(q32))/2 - (3^(1/2)*l2*sin(q32)*sin(q33))/2; 
            l1*sin(q11) - l1*sin(q31) + l2*cos(q13)*sin(q12) - l2*cos(q33)*sin(q32)
        ]);

        xk = x0 - J \ h;

        ϵ = sum(xk - x0);
        x0 = xk;
        i += 1;
    end

    ee1 = SVector{3}([l2 * sin(x0[1]) * sin(x0[2]); r_base + l1 * cos(q11) + l2 * cos(x0[1]); l1 * sin(q11) + l2 * sin(x0[1]) * cos(x0[2])]);
    ee2 = SVector{3}([l2 * sin(x0[3]) * sin(x0[4]); r_base + l1 * cos(q21) + l2 * cos(x0[3]); l1 * sin(q21) + l2 * sin(x0[3]) * cos(x0[4])]);
    ee3 = SVectir{3}([l2 * sin(x0[5]) * sin(x0[6]); r_base + l1 * cos(q31) + l2 * cos(x0[5]); l1 * sin(q31) + l2 * sin(x0[5]) * cos(x0[6])]);

    rot_z(theta) = [
        cos(theta) -sin(theta) 0; 
        sin(theta) cos(theta) 0; 
        0 0 1
    ];
    Rz_2pi3 = rot_z(2 * pi / 3);
    Rz_neg_2pi3 = rot_z(-2 * pi / 3);
    h1 = ee1 - [0;r_platform;0] - Rz_neg_2pi3 * (ee2 - [0;r_platform;0]);
    h2 = ee1 - [0;r_platform;0] -     Rz_2pi3 * (ee3 - [0;r_platform;0]);

    return ee1, ee2, ee3, h1, h2, i, ϵ
end


function workspace(p::DeltaRobotParams)
end
