
// translation 1: 16.75, 41.5, 13.25
// translation 2: 16.75, 41.5, 6.7
module jetson_model(x, y, z) {
    translate([x, y, z]) {
        import("../../res/stl/JetsonXavierNX.stl");
    }
}
