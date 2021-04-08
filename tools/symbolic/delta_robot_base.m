%% Perform Symbolic Calculations for Delta Robot
%% Clear Previous workspace and Declare Symbolic Variables
clear all
syms q11 q12 q13 q11dot q12dot q13dot q11ddot q12ddot q13ddot real
syms q21 q22 q23 q21dot q22dot q23dot q21ddot q22ddot q23ddot real
syms q31 q32 q33 q31dot q32dot q33dot q31ddot q32ddot q33ddot real

%% Organize symbols into vectors
q1 = [q11; q12; q13]; % position of arm 1
q1dot = [q11dot; q12dot; q13dot]; % velocity of arm 1
q1ddot = [q11ddot; q12ddot; q13ddot]; % acceleration of arm 1
q2 = [q21; q22; q23]; % position of arm 2
q2dot = [q21dot; q22dot; q23dot]; % velocity of arm 2
q2ddot = [q21ddot; q22ddot; q23ddot]; % acceleration of arm 2
q3 = [q31; q32; q33]; % position of arm 3
q3dot = [q31dot; q32dot; q33dot]; % velocity of arm 3
q3ddot = [q31ddot; q32ddot; q33ddot]; % acceleration of arm 3
q = [q11; q12; q13; q21; q22; q23; q31; q32; q33]; % overall position
qdot = [q11dot; q12dot; q13dot; q21dot; q22dot; q23dot; q31dot; q32dot; q33dot]; % overall velocity
qddot = [q11ddot; q12ddot; q13ddot; q21ddot; q22ddot; q23ddot; q31ddot; q32ddot; q33ddot]; % overall acceleration

%% Declare Model Parameters
syms l1 l2 real % lengths of link 1 and link 2 (m)
syms m1 m2 m3 real % masses of link 1, link 2 and end-effector (kg)
syms g real % gravity (m/s^2)
syms r_base r_platform real % radius of base
lc1 = l1 / 2; % distance to center of mass along link 1 (m)
lc2 = l2 / 2; % distance to center of mass along link 2 (m)
J1 = m1 * l1^2 / 12.0; % inertia link 1 (assuming rod) -> 1/3
J2 = m2 * l2^2 / 12.0; % inertia link 2 (assuming rod) -> 1/3

%% Compute Lagrangian for First Arm
phi1 = q1(1);
phi2 = q1(2);
phi3 = q1(3);
phi1dot = q1dot(1);
phi2dot = q1dot(2);
phi3dot = q1dot(3);

% Kinematics
xc11 = [0; r_base + lc1 * cos(phi1); lc1 * sin(phi1)]; % Position of Center of Mass of Link 1 of Arm 1 in Cartesian Coordintes
xc11dot = diff(xc11, phi1) * phi1dot; % Velocity of Center of Mass of Link 1 of Arm 1 in Cartesian Coordinates
xc12 = [lc2 * sin(phi2) * sin(phi3); r_base + l1 * cos(phi1) + lc2 * cos(phi2); l1 * sin(phi1) + lc2 * sin(phi2) * cos(phi3)]; % Position of Center of Mass of Link 2 of Arm 1 in Cartesian Coordinates
xc12dot = diff(xc12, phi1) * phi1dot + diff(xc12, phi2) * phi2dot + diff(xc12, phi3) * phi3dot; % Velocity of Center of Mass of Link 2 of Arm 1 in Cartesian Coordaintes
xc13 = [l2 * sin(phi2) * sin(phi3); r_base + l1 * cos(phi1) + l2 * cos(phi2); l1 * sin(phi1) + l2 * sin(phi2) * cos(phi3)]; % Position of Center of Mass of End-Effector of Arm 1 in Cartesian Coordinates
xc13dot = diff(xc13, phi1) * phi1dot + diff(xc13, phi2) * phi2dot + diff(xc13, phi3) * phi3dot; % Velocity of Center of Mass of End-Effector of Arm 1 in Cartesian Coordinates

% Kinetic & Potential Energies of Arm 1
T1 = 1/2 * m1 * xc11dot' * xc11dot + 1/2 * m2 * xc12dot' * xc12dot + 1/6 * m3 * xc13dot' * xc13dot + 1/2 * J1 * phi1dot^2 + 1/2 * J2 * (sin(phi2)^2 * phi3dot^2 + phi2dot^2); % Kinetic
V1 = -g * ((lc1 * m1 + l1 * (m2 + m3 / 3)) * sin(phi1) + (lc2 * m2 + l2 * m3 / 3) * sin(phi2) * cos(phi3)); % Potential

% Lagrangian of Arm 1
L1 = T1 - V1;

%% Compute Lagrangian for Second Arm
phi1 = q2(1);
phi2 = q2(2);
phi3 = q2(3);
phi1dot = q2dot(1);
phi2dot = q2dot(2);
phi3dot = q2dot(3);

% Kinematics
xc21 = [0; r_base + lc1 * cos(phi1); lc1 * sin(phi1)];
xc21dot = diff(xc21, phi1) * phi1dot;
xc22 = [lc2 * sin(phi2) * sin(phi3); r_base + l1 * cos(phi1) + lc2 * cos(phi2); l1 * sin(phi1) + lc2 * sin(phi2) * cos(phi3)];
xc22dot = diff(xc22, phi1) * phi1dot + diff(xc22, phi2) * phi2dot + diff(xc22, phi3) * phi3dot;
xc23 = [l2 * sin(phi2) * sin(phi3); r_base + l1 * cos(phi1) + l2 * cos(phi2); l1 * sin(phi1) + l2 * sin(phi2) * cos(phi3)];
xc23dot = diff(xc23, phi1) * phi1dot + diff(xc23, phi2) * phi2dot + diff(xc23, phi3) * phi3dot;

% Kinetic & Potential Energies
T2 = 1/2 * m1 * xc21dot' * xc21dot + 1/2 * m2 * xc22dot' * xc22dot + 1/6 * m3 * xc23dot' * xc23dot + 1/2 * J1 * phi1dot^2 + 1/2 * J2 * (sin(phi2)^2 * phi3dot^2 + phi2dot^2); % Kinetic
V2 = -g * ((lc1 * m1 + l1 * (m2 + m3 / 3)) * sin(phi1) + (lc2 * m2 + l2 * m3 / 3) * sin(phi2) * cos(phi3)); % Potential

% Lagrangian
L2 = T2 - V2;

%% Compute Lagrangian of Third Arm
phi1 = q3(1);
phi2 = q3(2);
phi3 = q3(3);
phi1dot = q3dot(1);
phi2dot = q3dot(2);
phi3dot = q3dot(3);

% Kinematics
xc31 = [0; r_base + lc1 * cos(phi1); lc1 * sin(phi1)];
xc31dot = diff(xc31, phi1) * phi1dot;
xc32 = [lc2 * sin(phi2) * sin(phi3); r_base + l1 * cos(phi1) + lc2 * cos(phi2); l1 * sin(phi1) + lc2 * sin(phi2) * cos(phi3)];
xc32dot = diff(xc32, phi1) * phi1dot + diff(xc32, phi2) * phi2dot + diff(xc32, phi3) * phi3dot;
xc33 = [l2 * sin(phi2) * sin(phi3); r_base + l1 * cos(phi1) + l2 * cos(phi2); l1 * sin(phi1) + l2 * sin(phi2) * cos(phi3)];
xc33dot = diff(xc33, phi1) * phi1dot + diff(xc33, phi2) * phi2dot + diff(xc33, phi3) * phi3dot;

% Kinetic & Potential Energy
T3 = 1/2 * m1 * xc31dot' * xc31dot + 1/2 * m2 * xc32dot' * xc32dot + 1/6 * m3 * xc33dot' * xc33dot + 1/2 * J1 * phi1dot^2 + 1/2 * J2 * (sin(phi2)^2 * phi3dot^2 + phi2dot^2); % Kinetic
V3 = -g * ((lc1 * m1 + l1 * (m2 + m3 / 3)) * sin(phi1) + (lc2 * m2 + l2 * m3 / 3) * sin(phi2) * cos(phi3)); % Potential

% Lagrangian
L3 = T3 - V3;

%% Declare Holonomic Constraint
Rz_2pi3 = rot_z(2 * pi / 3); % Rotation by 1/3 circle about z-axis
Rz_neg_2pi3 = rot_z(-2 * pi / 3);  % Rotation by 1/3 circle about z-axis
h1 = xc13 - [0;r_platform;0] - Rz_neg_2pi3 * (xc23 - [0;r_platform;0]); % holonomic constraint 1
h2 = xc13 - [0;r_platform;0] -     Rz_2pi3 * (xc33 - [0;r_platform;0]); % holonomic constraint 2
h = [h1; h2];  % organize constraints into single 6x1 vector
H = jacobian(h, q); % Jacobian of holonomic constraints
syms lambda1 lambda2 lambda3 lambda4 lambda5 lambda6 real % declare lagrangian multipliers
lambda = [lambda1 lambda2 lambda3 lambda4 lambda5 lambda6]'; % organize lagrangian multipliers into vector

Lu = L1 + L2 + L3; % add the lagrangians into single unconstrained lagrangian
L = Lu + lambda' * h; % constrain lagrangian with addition of holonomic constraint * lagrangian multipliers

%% Transform Lagrangians into the Euler-Lagrange Equation
dLdqdot   = jacobian(L, qdot)';
dtdLdqdot = jacobian(dLdqdot, [q;qdot]) * [qdot; qddot];
dLdq      = jacobian(L, q)';
EL = simplify(dtdLdqdot - dLdq); % Euler-Lagrange Equation

%% Determine Standard Representation from Euler-Lagrange Equation (M*qddot + C(q, qdot) + G(q) = lambda' * H) -> Mqqdot + C + G = lambda' H + Bu -> G = lambda' * H + Bu -> choose u s.t. u = G(1) - lambda' * H (eq 24.) Mbar_y
[M, b] = equationsToMatrix(EL, qddot);
M = simplify(M);
b = simplify(b); % External Forces: -b == C + G - lambda' * H

%% Take derivative w.r.t time of H
H1 = H(:, 1);
H1dot = jacobian(H1, q) * qdot;
H2 = H(:, 2);
H2dot = jacobian(H2, q) * qdot;
H3 = H(:, 3);
H3dot = jacobian(H3, q) * qdot;
H4 = H(:, 4);
H4dot = jacobian(H4, q) * qdot;
H5 = H(:, 5);
H5dot = jacobian(H5, q) * qdot;
H6 = H(:, 6);
H6dot = jacobian(H6, q) * qdot;
H7 = H(:, 7);
H7dot = jacobian(H7, q) * qdot;
H8 = H(:, 8);
H8dot = jacobian(H8, q) * qdot;
H9 = H(:, 9);
H9dot = jacobian(H9, q) * qdot;
Hdot = [H1dot, H2dot, H3dot, H4dot, H5dot, H6dot, H7dot, H8dot, H9dot];

% Perform Baumgarte Index Reduction
z0 = h; % z0 vector (6x1)
z1 = H * qdot; % z0dot == z1 vector (6x9 x 9x1 = 6x1)
z2 = Hdot * qdot + H * (M \ b); % 6x9 * 9x1 + 6x9 * 9x1 = 6x1 + 6x1 = 6x1

% dae1_holonomic = simplify(z0 + z1 + z2);

%% Determine the Sub-Inertia Matrices
Mtop = M(1:3, 1:3);
Mmid = M(4:6, 4:6);
Mbot = M(7:9, 7:9);

%% Write Matrices to File
% writematrix(char(Dtop), "../../res/mat_data/Dtop_base.txt");
% writematrix(char(Dmid), "../../res/mat_data/Dmid_base.txt");
% writematrix(char(Dbot), "../../res/mat_data/Dbot_base.txt");
% writematrix(char(Hdot), "../../res/mat_data/Hdot_base.txt");
% writematrix(   char(h), "../../res/mat_data/h_lower_base.txt");
% writematrix(   char(b), "../../res/mat_data/b_base.txt");
% writematrix(   char(H), "../../res/mat_data/H_upper_base.txt");

%% Forward Kinematics Jacobian
J = jacobian(h, [q12, q13, q22, q23, q32, q33]);
% for_kin_matrix = J \ -h;