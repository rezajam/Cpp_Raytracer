#include "color.h"
#include "vec3.h"
#include "ray.h"


#include <iostream>

color ray_color(const ray& r) {
    // return color(0,0,0);
    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}


int main() {

    /* ---------------------------------- Image --------------------------------- */
    // // size of the grid (image dim)
    // int image_height = 256; 
    // int image_width = 256;

    // USING THE RAY.H
    // (viewport: A virtual rectangle in front of the camera through which rays are shot. 
    //                                / monitor screen resolution in games/ the amount of pixels and screen that we can render(out of THE ENTIRE SCENE) )
    //  its like we have big scene, and we capture a part of it (the viewport)/what we SEE on the screen.
    // aspect ratio is viewport's width to height 
    auto aspect_ratio = 16.0 / 9.0;  // Widescreen 16:9 (ideal aspect ratio)
    int image_width = 400;     // Width of image in px

    // Calculate the image height, and rounded to nearest int(pixels must be integers) that height is at minimum 1 (not less).
    int image_height = int(image_width / aspect_ratio); // Height of image = width / aspect_ratio
    image_height = (image_height < 1) ? 1 : image_height; //...                        that height is at minimum 1 (not less)


    /* --------------------------------- Camera --------------------------------- */

    auto focal_length = 1.0; // distance from camera to viewport [camera_center] -->[viewport_upper_left]. We say its 1 unit.
    auto viewport_height = 2.0; // The fixed vertical size of the virtual viewport in the 3D world.
    // It does not directly correspond to image_height, but rather acts as the scaled reference for how tall the viewport should be.
    auto viewport_width = viewport_height * (double(image_width)/image_height); // image_width =(VIEWPORT IS stretch/squeeze to FIT IMAGE Aspect Ratio)=> viewport_width
    //XX viewport_width = viewport_height * aspect_ratio; XXXXX NOT THIS
    // RMBR: we have an image dim (image_width and image_height), and we want to FIT(stretch/squeeze) it viewport to IMAGE (viewport_width -> image_width , viewport_height is fixed) ==> into screen IMAGE aspect ratio (ex: 16:9).
    // ... [me:] rmbr: game resolution 647x364 to 1080x1920 (WE STRETCH OR SQUEEZE the image to fit the screen/viewport)
    // ... [me:] ...  this is the same thing. We have an screen(viewport here)  and we want to FIT IT to image 

    // |P: Why didn’t we use aspect_ratio directly (i.e., "auto viewport_width = viewport_height * aspect_ratio;" XXXXXX)?
    // ... because while aspect_ratio(ideal aspect ratio)                    is a DOUBLE (e.g., 16.0 / 9.0), 
    // ... BUTTTT the ACTUAL image DIM (image_width and image_height) are INTs.
    // ... ==>>       ACTUAL image DIM (INT) ==// might not perfectly match //==  the ideal aspect ratio (DOUBLE) 
    // ...                                     bcz ROUNDING(double to int) or USER input.
    //
    // eX: If we use aspect_ratio directly, the calculated viewport width may not align with the ACTUAL image dimensions (pixel grid).
    // ... For example, if image_width is 400, the calculated image_height = int(400 / (16.0 / 9.0)) = 225 (rounded).
    // ... The actual aspect ratio becomes image_width / image_height = 400 / 225 ≈ 1.777... (close to 16:9, but not exact).
    // ... [me:] see how we dont use height, and let the aspect ratio to find the height --> but since height is int --> our calculations gives double with EXTRA DECIMALS --> back to height (back to int) --> WE ROUND IT --> WE GET a round number cool BUTT WE REMOVED SOME DETAILS (NOT EXACTLY MATCHING THE SCREEN)
    //
    // /A: =======>> SO: WE DONT WAIT FOR THE ASPECT RATIO TO FIND THE HEIGHT. => recalculate the aspect ratio using the ACTUAL image dims (h and w)
    // To ensure PERFECT ALIGNMENT between the VIEWPORT ASPECT RATIO and the IMAGE DIMENSIONS:
    // ... We recalculate the aspect ratio using the ACTUAL pixel dimensions (image_width and image_height),
    // ... guaranteeing that the viewport perfectly matches the image grid.

    auto camera_center = point3(0, 0, 0);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    auto viewport_upper_left = camera_center
                             - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);


    /* --------------------------------- Render --------------------------------- */

    // size of the PPM header
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    // draw image pixel x pixel
    for (int j = 0; j < image_height; ++j) {
        
        //[1]

        //UPDATE PROGRESS PER ROW
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush; 
        //[3]
 
        for (int i = 0; i < image_width; ++i) {
            // auto r = double(i) / (image_width - 1);  // Horizontal gradient for red
            // auto g = double(j) / (image_height - 1); // Vertical gradient for green
            // auto b = 0.0; // No blue

            // int ir = static_cast<int>(255.999 * r);
            // int ig = static_cast<int>(255.999 * g);
            // int ib = static_cast<int>(255.999 * b);
            // //[2]

            // // std::cout << ir << " " << ig << " " << ib << "\n";
            // auto pixel_color = color(double(i)/(image_width-1), double(j)/(image_height-1), 0);
            // //[4]
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r);
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

/* ------------------------ USING COLOR.H and VEC3.H ------------------------ */
/* ----------------------------------- [4] ---------------------------------- */
    // write_color is a function that writes the color to the output stream.
    // ... it takes the output stream and the color as arguments.
    // ... it writes the color to the output stream.

/* ----------------------------------- [5] ---------------------------------- */
    // write_color is a function that writes the color to the output stream.
    // ... it takes the output stream and the color as arguments.
    // ... it writes the color to the output stream.

