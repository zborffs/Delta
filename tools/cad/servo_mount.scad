module servo_mount(servo_mount_dimensions) {
    servo_x = servo_mount_dimensions[0];
    servo_x_with_fins = servo_mount_dimensions[1];
    servo_y = servo_mount_dimensions[2];
    servo_y_with_fins = servo_mount_dimensions[3];
    servo_z = servo_mount_dimensions[4];
    servo_hole_diameter = servo_mount_dimensions[5];
    base_bicep_slit_thickness = servo_mount_dimensions[6];
    
    fin_x = (servo_x_with_fins - servo_x) / 2;
    translate([servo_x, base_bicep_slit_thickness / 2, 0]) {
        cube([fin_x, servo_y_with_fins, servo_z]);
    }
    
    translate([-fin_x, base_bicep_slit_thickness / 2, 0]) {
        cube([fin_x, servo_y_with_fins, servo_z]);
    }
}