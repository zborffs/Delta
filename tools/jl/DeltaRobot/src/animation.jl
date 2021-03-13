## Contains functions for animating Delta Robot solution
using Plots

"""
transforms the solution's state variables for the delta robot to cartesian
coordinates
"""
function transform_to_cartesian(t, phi1, phi2, phi3, p)
    @assert(length(t) == length(phi1))
    @assert(length(t) == length(phi2))
    @assert(length(t) == length(phi3))

    # get parameters
    l1 = p.l1;
    l2 = p.l2;
    lc1 = l1 / 2.0;
    lc2 = l2 / 2.0;

    # end of link 1
    x1 = zeros(length(t))
    y1 = l1 * cos.(phi1) .+ p.rb
    z1 = l1 * sin.(phi1)

    # end of link 2 (end-effector)
    x2 = l2 .* sin.(phi2) .* sin.(phi3)
    y2 = y1 + l2 * cos.(phi2)
    z2 = z1 + l2 * sin.(phi2) .* cos.(phi3)

    return x1, y1, z1, x2, y2, z2
end

"""
generates an animation object from the solution to the delta robot arm IVP and
delta robot parameters
"""
function animate_arm(sol, p)
    gr()
    t = sol.t
    x1, y1, z1, x2, y2, z2 = transform_to_cartesian(t, sol[1,:], sol[2,:], sol[3,:], p)

    anim = Animation()
    for i in 1:length(t)
        t_str = string("Time = ", round(t[i]), " sec");
        p1 = plot3d([0,-y1[i]], [0,-x1[i]], [0, -z1[i]], size=(350,350), xlim=(-1,1), ylim=(-1,1), zlim=(-1, 1), markersize = 5, markershape=:circle, camera = (45, 60), grid=nothing)
        plot3d!(p1, [-y1[i],-y2[i]], [-x1[i],-x2[i]], [-z1[i], -z2[i]], markersize=5, markershape = :circle, title = t_str, title_location=:left, grid=nothing)

        p2 = plot3d([0,-y1[i]], [0,-x1[i]], [0, -z1[i]], size=(350,350), xlim=(-1,1), ylim=(-1,1), zlim=(-1, 1), markersize = 5, markershape=:circle, camera = (0, 0), grid=nothing)
        plot3d!(p2, [-y1[i],-y2[i]], [-x1[i],-x2[i]], [-z1[i], -z2[i]], markersize=5, markershape = :circle, grid=nothing)

        p3 = plot3d([0,-y1[i]], [0,-x1[i]], [0, -z1[i]], size=(350,350), xlim=(-1,1), ylim=(-1,1), zlim=(-1, 1), markersize = 5, markershape=:circle, camera = (0, 90), grid=nothing)
        plot3d!(p3, [-y1[i],-y2[i]], [-x1[i],-x2[i]], [-z1[i], -z2[i]], markersize=5, markershape = :circle, grid=nothing)

        p = plot(p1, p2, p3, layout=(1,3), size=(1500, 350), legend=:none)
        frame(anim, p)
    end
    return anim
end

"""
generates an animation object from the solution to the delta robot IVP and
delta robot parameters
"""
function animate_robot(sol, p)
    gr()
    t = sol.t;
    x11, y11, z11, x12, y12, z12 = transform_to_cartesian(sol.t, sol[1,:], sol[2,:], sol[3,:], p)
    x21, y21, z21, x22, y22, z22 = transform_to_cartesian(sol.t, sol[4,:], sol[5,:], sol[6,:], p)
    x31, y31, z31, x32, y32, z32 = transform_to_cartesian(sol.t, sol[7,:], sol[8,:], sol[9,:], p)

    Rz(θ) = [cos(θ) -sin(θ) 0; sin(θ) cos(θ) 0; 0 0 1];
    x21, y21, z21 = Rz(-2 * pi / 3) * [x21, y21, z21];
    x22, y22, z22 = Rz(-2 * pi / 3) * [x22, y22, z22];
    x31, y31, z31 = Rz(2 * pi / 3) * [x31, y31, z31];
    x32, y32, z32 = Rz(2 * pi / 3) * [x32, y32, z32];

    anim = Animation()
    for i in 1:length(sol.t)
        t_str = string("Time = ", round(sol.t[i]), " sec");
        p1 = plot3d([0,-x11[i]], [-p.rb,-y11[i]], [0.0, -z11[i]], size=(400,300), xlim=(-1,1), ylim=(-1,1), zlim=(-1, 1), markersize = 5, markershape=:circle, camera = (45, 60), legend=:none, linecolor=:blue, markercolor=:blue);
        plot3d!([-x11[i],-x12[i]], [-y11[i],-y12[i]], [-z11[i], -z12[i]], markersize=5, markershape = :circle, title = t_str, title_location=:left, aspect_ratio=:auto, linecolor=:blue, markercolor=:blue);
        plot3d!([-p.rb * -sin(-2 * pi / 3),-x21[i]], [-p.rb * cos(-2 * pi / 3),-y21[i]], [0, -z21[i]], markersize = 5, markershape=:circle, linecolor=:red, markercolor=:red);
        plot3d!([-x21[i],-x22[i]], [-y21[i],-y22[i]], [-z21[i], -z22[i]], markersize=5, markershape = :circle, linecolor=:red, markercolor=:red);
        plot3d!([-p.rb * -sin(2 * pi / 3),-x31[i]], [-p.rb * cos(2 * pi / 3),-y31[i]], [0, -z31[i]], markersize = 5, markershape=:circle, linecolor=:green, markercolor=:green);
        plot3d!([-x31[i],-x32[i]], [-y31[i],-y32[i]], [-z31[i], -z32[i]], markersize=5, markershape = :circle, linecolor=:green, markercolor=:green);

        p2 = plot3d([0,-x11[i]], [-p.rb,-y11[i]], [0.0, -z11[i]], size=(400,300), xlim=(-1,1), ylim=(-1,1), zlim=(-1, 1), markersize = 5, markershape=:circle, camera = (0, 0), legend=:none, linecolor=:blue, markercolor=:blue);
        plot3d!([-x11[i],-x12[i]], [-y11[i],-y12[i]], [-z11[i], -z12[i]], markersize=5, markershape = :circle, aspect_ratio=:auto, linecolor=:blue, markercolor=:blue);
        plot3d!([-p.rb * -sin(-2 * pi / 3),-x21[i]], [-p.rb * cos(-2 * pi / 3),-y21[i]], [0, -z21[i]], markersize = 5, markershape=:circle, linecolor=:red, markercolor=:red);
        plot3d!([-x21[i],-x22[i]], [-y21[i],-y22[i]], [-z21[i], -z22[i]], markersize=5, markershape = :circle, linecolor=:red, markercolor=:red);
        plot3d!([-p.rb * -sin(2 * pi / 3),-x31[i]], [-p.rb * cos(2 * pi / 3),-y31[i]], [0, -z31[i]], markersize = 5, markershape=:circle, linecolor=:green, markercolor=:green);
        plot3d!([-x31[i],-x32[i]], [-y31[i],-y32[i]], [-z31[i], -z32[i]], markersize=5, markershape = :circle, linecolor=:green, markercolor=:green);

        p3 = plot3d([0,-x11[i]], [-p.rb,-y11[i]], [0.0, -z11[i]], size=(400,300), xlim=(-1,1), ylim=(-1,1), zlim=(-1, 1), markersize = 5, markershape=:circle, camera = (0, 90), legend=:none, linecolor=:blue, markercolor=:blue);
        plot3d!([-x11[i],-x12[i]], [-y11[i],-y12[i]], [-z11[i], -z12[i]], markersize=5, markershape = :circle, aspect_ratio=:auto, linecolor=:blue, markercolor=:blue);
        plot3d!([-p.rb * -sin(-2 * pi / 3),-x21[i]], [-p.rb * cos(-2 * pi / 3),-y21[i]], [0, -z21[i]], markersize = 5, markershape=:circle, linecolor=:red, markercolor=:red);
        plot3d!([-x21[i],-x22[i]], [-y21[i],-y22[i]], [-z21[i], -z22[i]], markersize=5, markershape = :circle, linecolor=:red, markercolor=:red);
        plot3d!([-p.rb * -sin(2 * pi / 3),-x31[i]], [-p.rb * cos(2 * pi / 3),-y31[i]], [0, -z31[i]], markersize = 5, markershape=:circle, linecolor=:green, markercolor=:green);
        plot3d!([-x31[i],-x32[i]], [-y31[i],-y32[i]], [-z31[i], -z32[i]], markersize=5, markershape = :circle, linecolor=:green, markercolor=:green);

        plt = plot(p1, p2, p3, layout=(1,3), size=(1500, 350), legend=:none)
        frame(anim, plt)
    end

    return anim
end
