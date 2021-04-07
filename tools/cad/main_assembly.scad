/*
 * All measurements in millimeters
 */


include <jetson_xavier_nx.scad> // STL of the JetsonNX
include <jetson_lid.scad> // the lid over the JetsonNX
include <pvc_attachment.scad> // the PVC attachment
include <base.scad> // the base
include <servo.scad> // STL of the servo
include <servo_mount.scad> // servo mounts on base

$fn = 250; // number of fragments

// Jetson Lid dimensions
approx_jetson_x = 104;
approx_jetson_y = 92;
approx_jetson_z = 35;
approx_jetson_display_port_z = 22;
approx_jetson_display_port_x = 35 - 16;
approx_jetson_display_port_x_offset = 16;
approx_jetson_display_port_z_offset = 3;
approx_jetson_power_port_z = 12;
approx_jetson_power_port_x = 12 - 3;
approx_jetson_power_port_x_offset = 3;
approx_jetson_usb_port1_z = 20;
approx_jetson_usb_port1_x = 52 - 37;
approx_jetson_usb_port1_x_offset = 37;
approx_jetson_usb_port2_x_offset = 53.75;
approx_jetson_eth_port_z = 17;
approx_jetson_eth_port_x = 87 - 70;
approx_jetson_eth_port_x_offset = 70;
approx_jetson_usbc_port_z = 5;
approx_jetson_usbc_port_x = 98 - 89;
approx_jetson_usbc_port_x_offset = 89;
approx_jetson_fan_pins_x = 55 - 20;
approx_jetson_fan_pins_z = 5;
approx_jetson_fan_pins_x_offset = 20;
approx_jetson_cam1_z = 3;
approx_jetson_cam1_y = 73 - 50;
approx_jetson_cam1_y_offset = 49;
approx_jetson_cam2_y_offset = approx_jetson_cam1_y_offset - approx_jetson_cam1_y - 2;
approx_jetson_pin_y = 80 - 20;
approx_jetson_pin_y_offset = 20;
approx_jetson_pin_x = 102 - 96;
approx_jetson_pin_x_offset = 96;
approx_jetson_fan_x_offset = 20;
approx_jetson_fan_x = 80 - 20;
approx_jetson_fan_y_offset = 40;
approx_jetson_fan_y = 81 - 40;

// put all dimensions into single array
approx_jetson_dimensions = [approx_jetson_x, approx_jetson_y, approx_jetson_z, approx_jetson_display_port_z, approx_jetson_display_port_x, approx_jetson_display_port_x_offset, approx_jetson_display_port_z_offset, approx_jetson_power_port_z, approx_jetson_power_port_x, approx_jetson_power_port_x_offset, approx_jetson_usb_port1_z, approx_jetson_usb_port1_x, approx_jetson_usb_port1_x_offset, approx_jetson_usb_port2_x_offset, approx_jetson_eth_port_z, approx_jetson_eth_port_x,approx_jetson_eth_port_x_offset, approx_jetson_usbc_port_z, approx_jetson_usbc_port_x, approx_jetson_usbc_port_x_offset, approx_jetson_fan_pins_x, approx_jetson_fan_pins_z, approx_jetson_fan_pins_x_offset, approx_jetson_cam1_z, approx_jetson_cam1_y, approx_jetson_cam1_y_offset, approx_jetson_cam2_y_offset, approx_jetson_pin_y, approx_jetson_pin_y_offset, approx_jetson_pin_x, approx_jetson_pin_x_offset, approx_jetson_fan_x_offset, approx_jetson_fan_x, approx_jetson_fan_y_offset, approx_jetson_fan_y];


// arbitrarily chosen (not measured) jetson lid data
jetson_lid_bottom_border_radius = 10;// mm
jetson_lid_thickness = 3; // mm
jetson_lid_screw_radius = 2; // mm

// draw the JetsonNX offset by some parameters
jetson_model_offset_x = 16.75;
jetson_model_offset_y = 41.5;
jetson_model_offset_z = 7.7 + jetson_lid_thickness;
#jetson_model(jetson_model_offset_x, jetson_model_offset_y, jetson_model_offset_z);

// draw the jetson lid
jetson_lid(approx_jetson_dimensions, jetson_lid_bottom_border_radius, jetson_lid_thickness, jetson_lid_screw_radius);

// pvc attachment measurements
pvc_pipe_radius = 42.16 / 2; // mm -> 1 1/4'' PVC Pipe
pvc_attachment_base_x_offset = 25;
pvc_attachment_base_y_offset = 5;
pvc_attachment_base_x = 75 - 25;
pvc_attachment_base_y = 35 - 5;
pvc_attachment_cylinder_height = 10;
pvc_dimensions = [pvc_pipe_radius, pvc_attachment_base_x_offset, pvc_attachment_base_y_offset, pvc_attachment_base_x, pvc_attachment_base_y, pvc_attachment_cylinder_height];

// draw the pvc attachment
pvc_attachment(approx_jetson_dimensions, pvc_dimensions, jetson_lid_thickness);

// base measurements
base_radius = 145; // 114; // mm = 4.488 inch
base_thickness = 10;
base_jetson_divet = 4;
base_bicep_slit_depth = 60; // 40
base_bicep_slit_thickness = 16;
base_dimensions = [base_radius, base_jetson_divet, base_bicep_slit_depth, base_bicep_slit_thickness];

// draw the base
base(approx_jetson_dimensions, base_dimensions, base_thickness);

// servo measurements
approx_servo_x = 40;
approx_servo_x_with_fins = approx_servo_x + 2 * 7;
approx_servo_y = 48.5;
approx_servo_y_with_fins = 41.4 - 30.5;
approx_servo_z = 20;
servo_x_offset = -(approx_jetson_x / 2 + base_radius - base_bicep_slit_depth) - approx_servo_x;
servo_y_offset = -approx_jetson_y / 2 - approx_servo_y;
approx_hole_diameter = 5; // arbitrary
servo_mount_dimensions = [approx_servo_x, approx_servo_x_with_fins, approx_servo_y, approx_servo_y_with_fins, approx_servo_z, approx_hole_diameter, base_bicep_slit_thickness];

// draw the servos
rotate([0, 0, 180]) {
    #servo_model(servo_x_offset, servo_y_offset, approx_servo_z);
}
translate([approx_jetson_x / 2 + base_radius - base_bicep_slit_depth, approx_jetson_y / 2, 0]) {
    servo_mount(servo_mount_dimensions);
}


translate([approx_jetson_x / 2, approx_jetson_y / 2, 0]) {
    rotate([0, 180, 360/3]) {
        #servo_model(-base_radius + base_bicep_slit_depth - approx_servo_x, -approx_servo_y, 0);
    }
}

translate([approx_jetson_x / 2, approx_jetson_y / 2, 0]) {
    rotate([180, 180, -360/3]) {
        #servo_model(-base_radius + base_bicep_slit_depth - approx_servo_x, -approx_servo_y, approx_servo_z);
    }
}