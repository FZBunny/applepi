/*****************************************************************************

    File: "led_dim_green.pov"

    Copyright (C) 2003,2019 Bruce Ward

    This file is part of apple2-qt, an Apple2 Emulator.
    (Apple2 is a trademark of Apple, Inc.)

    "apple2-qt" is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    "apple2-qt" is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You may obtain a copy of the GNU General Public License by writing to:

    The Free Software Foundation, Inc.
    51 Franklin Street, Fifth Floor
    Boston, MA 02110-1335 USA

    Or simply visit: https://www.gnu.org/licenses/gpl-3.0.html

*****************************************************************************/




// Persistence Of Vision raytracer version 3.5 file.
// -------------------------------------------------
//
//  A simple green LED, seen head-on.
//  Do this:
//  povray  led_dim_green.pov  +UA +W24 +H18
//

global_settings { assumed_gamma 2.2 }

#include "colors.inc"
#include "shapes.inc"
#include "textures.inc"
#include "glass.inc"
#include "consts.inc"                  // Index of refraction constants

camera {
   location <0, 0, -100>
   angle 40
   look_at <0, 0, 0>
}



light_source { <-200, 200, -100>  rgb<2, 2, 2> }


#declare plastic = material {
    texture {
        pigment {
            rgbf<0.2, 1, 0.2, 1>
        }

        finish { // from glass.inc, F_Glass8
            specular 0.3
            roughness 0.1
            ambient 0
            diffuse 0.15
            brilliance 4
            reflection {
              0.2, 1.0
                fresnel on
            }
            conserve_energy
        }
    }
    interior {
        ior 1.5
        media {
            scattering {
                ISOTROPIC_SCATTERING,
                rgb <0.1, 0.5, 0.1>
                extinction 1
            }
        }
    }
}



union {
    sphere { <0, 0, 0>, 18
        material {plastic}
    }
    cylinder { <0,0,0>, <0,0,36>, 18
        material {plastic}
    }
}



// this light source & box approximate the diode.
// (light source is omitted... it's dim.)

box {
    <-0.15, -0.2 18>,<0.15, 0.2, 18+0.1>
    pigment {rgb<0.1, 1, 0.1>}
    finish {
        ambient 1
        diffuse 1
    }
}


