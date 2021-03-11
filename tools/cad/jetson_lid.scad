module _bottom_border(jetson_x, jetson_y, border_x, border_y, border_z, screw_radius) {
    // base cube minus the jetson board
	difference() {
		translate([-border_x, -border_y, 0]) {
			cube([jetson_x + 2 * border_x, jetson_y + 2 * border_y, border_z]);
		}

        // screwholes
		border_contension_offset = 1;
		translate([0, 0, -border_contension_offset]) { // '1' is for border
			cube([jetson_x, jetson_y, border_z + 2 * border_contension_offset]);
		}
        
        
        translate([-border_x / 2, -border_y / 2, -border_contension_offset]) {
            cylinder(2 * border_contension_offset + border_z, screw_radius, screw_radius);
        }
        
        translate([border_x / 2 + jetson_x, -border_y / 2, -border_contension_offset]) {
            cylinder(2 * border_contension_offset + border_z, screw_radius, screw_radius);
        }
        
        translate([-border_x / 2, border_y / 2 + jetson_y, -border_contension_offset]) {
            cylinder(2 * border_contension_offset + border_z, screw_radius, screw_radius);
        }
        
        translate([border_x / 2 + jetson_x, border_y / 2 + jetson_y, -border_contension_offset]) {
            cylinder(2 * border_contension_offset + border_z, screw_radius, screw_radius);
        }
	}
}

module _port_wall(jetson_dimensions, thickness) {
    jetson_x = jetson_dimensions[0];
    jetson_y = jetson_dimensions[1];
    jetson_z = jetson_dimensions[2];
    jetson_display_port_z = jetson_dimensions[3];
    jetson_display_port_x = jetson_dimensions[4];
    jetson_display_port_x_offset = jetson_dimensions[5];
    jetson_display_port_z_offset = jetson_dimensions[6];
    jetson_power_port_z = jetson_dimensions[7];
    jetson_power_port_x = jetson_dimensions[8];
    jetson_power_port_x_offset = jetson_dimensions[9];
    jetson_usb_port1_z = jetson_dimensions[10];
    jetson_usb_port1_x = jetson_dimensions[11];
    jetson_usb_port1_x_offset = jetson_dimensions[12];
    jetson_usb_port2_x_offset = jetson_dimensions[13];
    jetson_eth_port_z = jetson_dimensions[14];
    jetson_eth_port_x = jetson_dimensions[15];
    jetson_eth_port_x_offset = jetson_dimensions[16];
    jetson_usbc_port_z = jetson_dimensions[17];
    jetson_usbc_port_x = jetson_dimensions[18];
    jetson_usbc_port_x_offset = jetson_dimensions[19];
    
    rotate([90, 0, 0]) {
        border_contension_offset = 1;
        difference() {
            cube([jetson_x, jetson_z, thickness]);
            translate([jetson_display_port_x_offset, jetson_display_port_z_offset, -border_contension_offset]) {
                cube([jetson_display_port_x, jetson_display_port_z, thickness + 2 * border_contension_offset]);
            }
            
            translate([jetson_power_port_x_offset, jetson_display_port_z_offset, -border_contension_offset]) {
                cube([jetson_power_port_x, jetson_power_port_z, thickness + 2 * border_contension_offset]);
            }
            
            translate([jetson_usb_port1_x_offset, jetson_display_port_z_offset, -border_contension_offset]) {
                cube([jetson_usb_port1_x, jetson_usb_port1_z, thickness + 2 * border_contension_offset]);
            }
            
            translate([jetson_usb_port2_x_offset, jetson_display_port_z_offset, -border_contension_offset]) {
                cube([jetson_usb_port1_x, jetson_usb_port1_z, thickness + 2 * border_contension_offset]);
            }
            
            translate([jetson_eth_port_x_offset, jetson_display_port_z_offset, -border_contension_offset]) {
                cube([jetson_eth_port_x, jetson_eth_port_z, thickness + 2 * border_contension_offset]);
            }
            
            translate([jetson_usbc_port_x_offset, jetson_display_port_z_offset, -border_contension_offset]) {
                cube([jetson_usbc_port_x, jetson_usbc_port_z, thickness + 2 * border_contension_offset]);
            }
        }
    }
}

module _fan_wall(jetson_dimensions, thickness) {
    jetson_x = jetson_dimensions[0];
    jetson_y = jetson_dimensions[1];
    jetson_z = jetson_dimensions[2];
    jetson_display_port_z_offset = jetson_dimensions[6];
    jetson_fan_pins_x = jetson_dimensions[20];
    jetson_fan_pins_z = jetson_dimensions[21];
    jetson_fan_pins_x_offset = jetson_dimensions[22];
    
    translate([0, jetson_y + thickness, 0]) {
        rotate([90, 0, 0]) {
            border_contension_offset = 1;
            difference() {
                cube([jetson_x, jetson_z, thickness]);
                translate([jetson_fan_pins_x_offset, jetson_display_port_z_offset, -border_contension_offset]) {
                    cube([jetson_fan_pins_x, jetson_fan_pins_z, thickness + 2 * border_contension_offset]);
                }
            }
        }
    }
}

module _cam_wall(jetson_dimensions, thickness) {
    jetson_x = jetson_dimensions[0];
    jetson_y = jetson_dimensions[1];
    jetson_z = jetson_dimensions[2];
    jetson_display_port_z_offset = jetson_dimensions[6];
    jetson_cam1_z = jetson_dimensions[23];
    jetson_cam1_y = jetson_dimensions[24];
    jetson_cam1_y_offset = jetson_dimensions[25];
    jetson_cam2_y_offset = jetson_dimensions[26];
    
    rotate([0, -90, 0]) {
        border_contension_offset = 1;
        difference() {
            cube([jetson_z, jetson_y, thickness]);
            
            translate([jetson_display_port_z_offset + jetson_z / 2, jetson_cam1_y_offset, -border_contension_offset]) {
                cube([jetson_cam1_z, jetson_cam1_y, thickness + 2 * border_contension_offset]);
            }
            
            translate([jetson_display_port_z_offset + jetson_z / 2, jetson_cam2_y_offset, -border_contension_offset]) {
                cube([jetson_cam1_z, jetson_cam1_y, thickness + 2 * border_contension_offset]);
            }
        }
    }
}

module _pin_wall(jetson_dimensions, thickness) {
    jetson_x = jetson_dimensions[0];
    jetson_y = jetson_dimensions[1];
    jetson_z = jetson_dimensions[2];
    jetson_display_port_z_offset = jetson_dimensions[6];
    jetson_pin_y = jetson_dimensions[27];
    jetson_pin_y_offset = jetson_dimensions[28];
    jetson_pin_x = jetson_dimensions[29];
    jetson_pin_x_offset = jetson_dimensions[30];

    translate([jetson_x + thickness, 0, 0]) {
        rotate([0, -90, 0]) {
            cube([jetson_z, jetson_pin_y_offset, thickness]);
        }
    }
    
    translate([jetson_x + thickness, jetson_pin_y + jetson_pin_y_offset, 0]) {
        rotate([0, -90, 0]) {
            cube([jetson_z, jetson_y - jetson_pin_y - jetson_pin_y_offset, thickness]);
        }
    }
}

module _roof(jetson_dimensions, thickness) {
    jetson_x = jetson_dimensions[0];
    jetson_y = jetson_dimensions[1];
    jetson_z = jetson_dimensions[2];
    jetson_fan_x_offset = jetson_dimensions[31];
    jetson_fan_x = jetson_dimensions[32];
    jetson_fan_y_offset = jetson_dimensions[33];
    jetson_fan_y = jetson_dimensions[34];

    difference() {
        border_contension_offset = 1;
        translate([0, 0, jetson_z - thickness]) {
            cube([jetson_x, jetson_y, thickness]);
        }
        
        translate([jetson_fan_x_offset, jetson_fan_y_offset, jetson_z - thickness - border_contension_offset]) {
            cube([jetson_fan_x, jetson_fan_y, thickness + 2 * border_contension_offset]);
        }
    }
}

module jetson_lid(jetson_dimensions, bottom_border_radius, thickness, screw_radius) {
    jetson_x = jetson_dimensions[0];
    jetson_y = jetson_dimensions[1];
    jetson_z = jetson_dimensions[2];
    union() {
        _bottom_border(jetson_x, jetson_y, bottom_border_radius, bottom_border_radius, thickness, screw_radius);
        _port_wall(jetson_dimensions, thickness);
        _fan_wall(jetson_dimensions, thickness);
        _cam_wall(jetson_dimensions, thickness);
        _pin_wall(jetson_dimensions, thickness);
        _roof(jetson_dimensions, thickness);
    }
}
