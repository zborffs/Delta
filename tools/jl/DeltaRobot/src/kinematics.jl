"""
implements Newton-Raphson method
"""
function newton(f::Function, x0::Number, fprime::Function, args::Tuple=(); tol::AbstractFloat=1e-8, maxiter::Integer=50, eps::AbstractFloat=1e-10)
    for _ in 1:maxiter
        yprime = fprime(x0, args...)
        if abs(yprime) < eps
            warn("First derivative is zero")
            return x0
        end
        y = f(x0, args...)
        x1 = x0 - y/yprime
        if abs(x1-x0) < tol
            return x1
        end
        x0 = x1
    end
    error("Max iteration exceeded")
end

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
implements forward kinematics for delta robot
"""
function for_kin(phi1, phi2, phi3, p::DeltaRobotParams)
end


function workspace(p::DeltaRobotParams)
end
