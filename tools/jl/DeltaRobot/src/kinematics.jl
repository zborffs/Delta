using LinearAlgebra, StaticArrays

"""
implements inverse kinematics for delta robot
"""
function inv_kin(x::Vector{Real}, p::DeltaRobotParams)
    @assert length(x) == 3
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
    - always initialize the matrix with a "good guess" otherwise with at least 1 non-zero element
"""
function for_kin(phi::Array{T, 1}, x0::Array{T, 1}, p::DeltaRobotParams; abstol=1e-15, max_iter=75) where T <: Real
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

    x0 = convert(Vector{Real}, x0)
    x0 = [q11, x0[1], x0[2], q21, x0[3], x0[4], q31, x0[5], x0[6]];

    return x0, i, ϵ;
end

function joint_angles_to_ee(x0::Vector{Real})
    @assert length(x) == 9

    ee1 = [l2 * sin(x0[2]) * sin(x0[3]); r_base + l1 * cos(x0[1]) + l2 * cos(x0[2]); l1 * sin(x0[1]) + l2 * sin(x0[2]) * cos(x0[3])];
    ee2 = [l2 * sin(x0[5]) * sin(x0[6]); r_base + l1 * cos(x0[4]) + l2 * cos(x0[5]); l1 * sin(x0[5]) + l2 * sin(x0[5]) * cos(x0[6])];
    ee3 = [l2 * sin(x0[8]) * sin(x0[9]); r_base + l1 * cos(x0[7]) + l2 * cos(x0[8]); l1 * sin(x0[7]) + l2 * sin(x0[8]) * cos(x0[9])];

    rot_z(theta) = [
        cos(theta) -sin(theta) 0;
        sin(theta) cos(theta) 0;
        0 0 1
    ];
    Rz_2pi3 = rot_z(2 * pi / 3);
    Rz_neg_2pi3 = rot_z(-2 * pi / 3);
    h1 = ee1 - [0;r_platform;0] - Rz_neg_2pi3 * (ee2 - [0;r_platform;0]);
    h2 = ee1 - [0;r_platform;0] -     Rz_2pi3 * (ee3 - [0;r_platform;0]);

    @assert isapprox(sum(h1), 0.0; atol=1e-12)
    @assert isapprox(sum(h2), 0.0; atol=1e-12)

    return ee1 - [0;r_platform;0]
end
