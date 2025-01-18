


#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
// with our vec3 class, we can define a color class that inherits from it.


#include <iostream>

using color = vec3;
// Alias "vec3 as color" for readability

// Write a color to the output stream
void write_color(std::ostream& out, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Translate the [0,1] component values to the byte range [0,255].
    int rbyte = int(255.999 * r);
    int gbyte = int(255.999 * g);
    int bbyte = int(255.999 * b);

    // Write out the pixel color components.
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif
