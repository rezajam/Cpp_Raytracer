#include "color.h"
#include "vec3.h"

#include <iostream>

int main() {

    // size of the grid (image dim)
    int image_height = 256; 
    int image_width = 256;

    // size of the PPM header
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    // draw image pixel x pixel
    for (int j = 0; j < image_height; ++j) {
        //[1]

        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush; //UPDATE PROGRESS PER ROW
        //[3]
 
        for (int i = 0; i < image_width; ++i) {
            // auto r = double(i) / (image_width - 1);  // Horizontal gradient for red
            // auto g = double(j) / (image_height - 1); // Vertical gradient for green
            // auto b = 0.0; // No blue

            // int ir = static_cast<int>(255.999 * r);
            // int ig = static_cast<int>(255.999 * g);
            // int ib = static_cast<int>(255.999 * b);
            // //[2]

            // std::cout << ir << " " << ig << " " << ib << "\n";
            auto pixel_color = color(double(i)/(image_width-1), double(j)/(image_height-1), 0);
            //[4]
            write_color(std::cout, pixel_color);
            //[5]
        }
    }

std::clog << "\rDone.                 \n";


    
}



/* ----------------------------------- [1] ---------------------------------- */
    // j <= image_height - 1 bcz ...
    // ... when we get to the (green) "auto g = double(j) / (image_height - 1);" to be only [0-1]
    // ... bcz if we DONT and say XXXXXXX "auto g = double(j) / (image_height    );" XXXXXXX
    // ...THEN :
    // ... so if we have image_height = 256, and j:[0 - 256-1] then:
    // ...      when j = 0 --> r = 0 / 256 ===> 0 [Great!]
    // ...      BUTTT when j = MAX = 255 --> r = 255 / 256 ===> =/= 1 [XXXXX BAD!]

/* ----------------------------------- [2] ---------------------------------- */
    // This part is scaling and converting the color values from a floating-point range (0.0 to 1.0) 
    // ... to an integer range (0 to 255), which is required for writing the PPM file.
    // We multiply by 255.999 to ensure the maximum floating-point value (1.0)
    // ...                                  maps to the maximum integer value (255) 
    // ...                                  WITHOUT rounding down due to truncation.
    // TLDR: first we add the 255.999 to avoid truncation(XX if to int first -> 0.99 = 1 XX) of integer and then we cast it to int.

/* ----------------------------------- [3] ---------------------------------- */
    // "\r" so it will be on the same line 
    // ... like this:
    // ... Scanlines remaining: 5,  4,  3,  2,  1,  0  all in one line.

/* ----------------------------------- [4] ---------------------------------- */
    // write_color is a function that writes the color to the output stream.
    // ... it takes the output stream and the color as arguments.
    // ... it writes the color to the output stream.

/* ----------------------------------- [5] ---------------------------------- */
    // write_color is a function that writes the color to the output stream.
    // ... it takes the output stream and the color as arguments.
    // ... it writes the color to the output stream.

