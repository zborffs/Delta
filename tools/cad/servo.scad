module servo_model(x, y, z) {
    translate([x, y, z]) {
        import("../stl/ServoMG996R.stl");
    }
}
