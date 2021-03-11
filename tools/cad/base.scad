
module _circle(jetson_dimensions, base_dimensions, thickness) {
    jetson_x = jetson_dimensions[0];
    jetson_y = jetson_dimensions[1];
    jetson_z = jetson_dimensions[2];
    
    base_radius = base_dimensions[0];
    base_jetson_divet = base_dimensions[1];
    base_bicep_slit_depth = base_dimensions[2];
    base_bicep_slit_thickness = base_dimensions[3];
    border_contension_offset = 1;
    difference() {
        translate([jetson_x / 2, jetson_y / 2, -thickness]) {
            difference() {
                cylinder(thickness, base_radius, base_radius);
            
                translate([base_radius - base_bicep_slit_depth, -base_bicep_slit_thickness / 2, -border_contension_offset]) {
                    cube([base_bicep_slit_depth, base_bicep_slit_thickness, thickness + 2 * border_contension_offset]);
                }

                rotate([0, 0, 360 / 3]) {                
                    translate([base_radius - base_bicep_slit_depth, -base_bicep_slit_thickness / 2, -border_contension_offset]) {

                        cube([base_bicep_slit_depth, base_bicep_slit_thickness, thickness + 2 * border_contension_offset]);
                    }
                }

                rotate([0, 0, -360 / 3]) {                
                    translate([base_radius - base_bicep_slit_depth, -base_bicep_slit_thickness / 2, -border_contension_offset]) {
                        cube([base_bicep_slit_depth, base_bicep_slit_thickness, thickness + 2 * border_contension_offset]);
                    }
                }
            }
        }
        
        // jetson divet
        translate([0, 0, -base_jetson_divet]) {
            cube([jetson_x, jetson_y, base_jetson_divet + 2 * border_contension_offset]);
        }
        
        // bicep slits
        
        
        // screwholes
//        translate([0, 0, -border_contension_offset]) { // '1' is for border
//			cube([jetson_x, jetson_y, border_z + 2 * border_contension_offset]);
//		}
//        
//        
//        translate([-border_x / 2, -border_y / 2, -border_contension_offset]) {
//            cylinder(2 * border_contension_offset + border_z, screw_radius, screw_radius);
//        }
//        
//        translate([border_x / 2 + jetson_x, -border_y / 2, -border_contension_offset]) {
//            cylinder(2 * border_contension_offset + border_z, screw_radius, screw_radius);
//        }
//        
//        translate([-border_x / 2, border_y / 2 + jetson_y, -border_contension_offset]) {
//            cylinder(2 * border_contension_offset + border_z, screw_radius, screw_radius);
//        }
//        
//        translate([border_x / 2 + jetson_x, border_y / 2 + jetson_y, -border_contension_offset]) {
//            cylinder(2 * border_contension_offset + border_z, screw_radius, screw_radius);
//        }
    }
}

module base(jetson_dimensions, base_dimensions, thickness) {
    _circle(jetson_dimensions, base_dimensions, thickness);
}