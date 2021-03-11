module _connector(jetson_dimensions, pvc_dimensions, thickness) {
    jetson_x = jetson_dimensions[0];
    jetson_y = jetson_dimensions[1];
    jetson_z = jetson_dimensions[2];
    
    pvc_attachment_base_x_offset = pvc_dimensions[1];
    pvc_attachment_base_y_offset = pvc_dimensions[2];
    pvc_attachment_base_x = pvc_dimensions[3];
    pvc_attachment_base_y = pvc_dimensions[4];
    
    translate([pvc_attachment_base_x_offset, pvc_attachment_base_y_offset, jetson_z]) {
        cube([pvc_attachment_base_x, pvc_attachment_base_y, thickness]);
    }
}

module _cylinder(jetson_dimensions, pvc_dimensions, thickness) {
    jetson_x = jetson_dimensions[0];
    jetson_y = jetson_dimensions[1];
    jetson_z = jetson_dimensions[2];
    
    pvc_attachment_base_x_offset = pvc_dimensions[1];
    pvc_attachment_base_y_offset = pvc_dimensions[2];
    pvc_attachment_base_x = pvc_dimensions[3];
    pvc_attachment_base_y = pvc_dimensions[4];
    pvc_attachment_cylinder_height = pvc_dimensions[5];
    
    translate([pvc_attachment_base_x_offset + pvc_attachment_base_x / 2, pvc_attachment_base_y_offset + pvc_attachment_base_y / 2, jetson_z + thickness]) {
        r = pvc_attachment_base_y / 2 - thickness;
        cylinder(pvc_attachment_cylinder_height + thickness, r, r);
    }
}

module _pipe(jetson_dimensions, pvc_dimensions, thickness) {
    jetson_x = jetson_dimensions[0];
    jetson_y = jetson_dimensions[1];
    jetson_z = jetson_dimensions[2];
    
    pvc_pipe_radius = pvc_dimensions[0];
    pvc_attachment_base_x_offset = pvc_dimensions[1];
    pvc_attachment_base_y_offset = pvc_dimensions[2];
    pvc_attachment_base_x = pvc_dimensions[3];
    pvc_attachment_base_y = pvc_dimensions[4];
    pvc_attachment_cylinder_height = pvc_dimensions[5];
    
    r = pvc_pipe_radius + thickness;
    border_contension_offset = 1;
    translate([pvc_attachment_base_x_offset, pvc_attachment_base_y_offset + pvc_attachment_base_y / 2, jetson_z + thickness + pvc_attachment_cylinder_height + r]) {
        rotate([0, 90, 0]) {
            difference() {
                cylinder(pvc_attachment_base_x, r1 = r, r2 = r);
                translate([0, 0, -border_contension_offset]) {
                    cylinder(pvc_attachment_base_x + 2 * border_contension_offset, r1 = pvc_pipe_radius, r2 = pvc_pipe_radius);
                }
            }
        }
    }
}

module pvc_attachment(jetson_dimensions, pvc_dimensions, thickness) {
    _connector(jetson_dimensions, pvc_dimensions, thickness);
    _cylinder(jetson_dimensions, pvc_dimensions, thickness);
    _pipe(jetson_dimensions, pvc_dimensions, thickness);
}