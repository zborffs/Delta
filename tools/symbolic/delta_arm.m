%% Symbolic calculations for Single Delta Arm
clear all

% Declare Symbolic Variables
syms phi1 phi2 phi3 phi1dot phi2dot phi3dot phi1ddot phi2ddot phi3ddot real
%syms tau real % torque

% Underactuate
%B = [1;0;0];

% Declare Numeric Variables
syms l1 l2 m1 m2 m3 g real
% l1 = 0.5; % length of link 1 (m)
% l2 = 0.5; % length of link 2 (m)
lc1 = l1 / 2; % distance to center of mass along link 1 (m)
lc2 = l2 / 2; % distance to center of mass along link 2 (m)
% m1 = 0.3; % mass of link 1 (m)
% m2 = 0.2; % mass of link 2 (m)
% m3 = 0.1; % mass of end effecter (m)
J1 = m1 * l1^2 / 12.0; % inertia link 1 (assuming rod)
J2 = m2 * l2^2 / 12.0; % inertia link 2 (assuming rod)
% g = 9.81; % m/s^2
d1 = 0.0; % damping in phi1 direction
d2 = 0.0; % damping in phi2 direction
d3 = 0.0; % damping in phi3 direction

% Organize symbols into vectors
q     = [phi1; phi2; phi3];
qdot  = [phi1dot; phi2dot; phi3dot];
qddot = [phi1ddot; phi2ddot; phi3ddot];

% Kinematics
xc1 = [0; lc1 * cos(phi1); lc1 * sin(phi1)];
xc1dot = diff(xc1, phi1) * phi1dot;
xc2 = [lc2 * sin(phi2) * sin(phi3); l1 * cos(phi1) + lc2 * cos(phi2); l1 * sin(phi1) + lc2 * sin(phi2) * cos(phi3)];
xc2dot = diff(xc2, phi1) * phi1dot + diff(xc2, phi2) * phi2dot + diff(xc2, phi3) * phi3dot;
xc3 = [l2 * sin(phi2) * sin(phi3); l1 * cos(phi1) + l2 * cos(phi2); l1 * sin(phi1) + l2 * sin(phi2) * cos(phi3)];
xc3dot = diff(xc3, phi1) * phi1dot + diff(xc3, phi2) * phi2dot + diff(xc3, phi3) * phi3dot;

% Kinetic & Potential Energy
T = 1/2 * m1 * xc1dot' * xc1dot + 1/2 * m2 * xc2dot' * xc2dot + 1/6 * m3 * xc3dot' * xc3dot + 1/2 * J1 * phi1dot^2 + 1/2 * J2 * (sin(phi2)^2 * phi3dot^2 + phi2dot^2); % Kinetic
V = -g * ((lc1 * m1 + l1 * (m2 + m3 / 3)) * sin(phi1) + (lc2 * m2 + l2 * m3 / 3) * sin(phi2) * cos(phi3)); % Potential

% Lagrangian
L = T - V;

%% Determine the Euler-Lagrange Equation
dLdqdot   = jacobian(L, qdot)';
dtdLdqdot = jacobian(dLdqdot, [q;qdot]) * [qdot; qddot];
dLdq      = jacobian(L, q)';
EL = simplify(dtdLdqdot - dLdq); % Euler-Lagrange Equation

%% Determine Standard Representation from Euler-Lagrange Equation (D*qddot + C + DelP = B*tau), where C is C * qdot
[D, b] = equationsToMatrix(EL,[phi1ddot;phi2ddot;phi3ddot]);
D = simplify(D);
b = simplify(b);
% DelP = simplify(jacobian(V, q)');
% C = simplify(-b - DelP + B*tau);
% 
% %% Get state-space model of dot(x) = f(x) + g(x)u, x=(q, qdot), dot(x)=(qdot, qddot)
% f2 = simplify(-D\(C + DelP));
% g2 = simplify(D\B); % Verify that g2(1) never crosses 0 by plotting surface
% f = [qdot; f2];
% g = [zeros(size(q)); g2];