include <a8pico_dimensions.scad>
use <a8pico_back_v2.scad>
use <a8pico_front.scad>

/* [Rendering:] */
item = 3; // [0:Assembly, 1:Front, 2:Back, 3:3d print]

module assembly()
{
    back();

    translate ([0,cart_width,30])
        rotate([180,0,0])
            front();
}

if (item == 0) {
    assembly();
}

if (item == 1) {
    front();
}

if (item == 2) {
    back();
}

if (item == 3) {
    front();
    translate([0,cart_width+10,0]) {
        back();
    }
}
