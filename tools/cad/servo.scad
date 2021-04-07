module servo_model(x, y, z) {
    translate([x, y, z]) {
        import("../../res/stl/ServoMG996R.stl");
    }
}
