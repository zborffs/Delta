clear all;
clc;
syms Rb Rp Ru Rl real % radii of: base, platform, upper link, and lower links, respectively
syms phi11 phi21 phi31 real % angles we wanna find
syms xe ye ze % position of end effector
syms x y z;

% x = 0;
% y = Rb + cos(phi11) * Ru;
% z = 0 +  sin(phi11) * Ru;
F1 = [0; Rb; 0];
J1 = [x; y; z];
% E1 = [xe; ye - Rp; ze];
E1 = [xe; ye + Rp; ze];

y1_z_pos = F1(2) + 1/2 * sqrt(-4 * (z - F1(3))^2 - 4 * F1(1)^2 + 4 * Ru^2);
y1_z_neg = F1(2) - 1/2 * sqrt(-4 * (z - F1(3))^2 - 4 * F1(1)^2 + 4 * Ru^2);

z1_pos = simplify(E1(1)^2 - Rl^2 + (y1_z_pos - E1(2))^2 + (J1(3) - E1(3))^2);
z1_neg = simplify(E1(1)^2 - Rl^2 + (y1_z_neg - E1(2))^2 + (J1(3) - E1(3))^2);
z1_pos_jac = diff(z1_pos, z);
z1_neg_jac = diff(z1_neg, z);

[phi11_sol] = solve(z1_pos, z);
% solve for 'z' numerically
% substitute value of 'z' in for y_z_pos and y_z_neg
% solve for phi11 

F2 = rot_z(2 * pi / 3) * F1;
E2 = rot_z(2 * pi / 3) * E1;

y2_z_pos = F2(2) + 1/2 * sqrt(-4 * (z - F2(3))^2 - 4 * F2(1)^2 + 4 * Ru^2);
y2_z_neg = F2(2) - 1/2 * sqrt(-4 * (z - F2(3))^2 - 4 * F2(1)^2 + 4 * Ru^2);

z2_pos = simplify(E2(1)^2 - Rl^2 + (y2_z_pos - E2(2))^2 + (J1(3) - E2(3))^2);
z2_neg = simplify(E2(1)^2 - Rl^2 + (y2_z_neg - E2(2))^2 + (J1(3) - E2(3))^2);
z2_pos_jac = diff(z2_pos, z);
z2_neg_jac = diff(z2_neg, z);

[phi21_sol] = solve(z2_pos, z);

F3 = rot_z(-2 * pi / 3) * F1;
E3 = rot_z(-2 * pi / 3) * E1;

y3_z_pos = F3(2) + 1/2 * sqrt(-4 * (z - F3(3))^2 - 4 * F3(1)^2 + 4 * Ru^2);
y3_z_neg = F3(2) - 1/2 * sqrt(-4 * (z - F3(3))^2 - 4 * F3(1)^2 + 4 * Ru^2);

z3_pos = simplify(E3(1)^2 - Rl^2 + (y3_z_pos - E3(2))^2 + (J1(3) - E3(3))^2);
z3_neg = simplify(E3(1)^2 - Rl^2 + (y3_z_neg - E3(2))^2 + (J1(3) - E3(3))^2);
z3_pos_jac = diff(z3_pos, z);
z3_neg_jac = diff(z3_neg, z);

[phi31_sol] = solve(z3_pos, z);