function [matrix] = rot_z(theta)
%ROT_Z Returns rotation matrix about z-axis
%   given angle representing amount to rotate about z-axis, returns
%   rotation matrix
matrix = [cos(theta), -sin(theta), 0; sin(theta), cos(theta), 0; 0 0 1];
end

