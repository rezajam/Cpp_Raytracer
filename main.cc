#include "color.h"
#include "vec3.h"
#include "ray.h"


#include <iostream>


// After mixing the ray and sphere equations and solving for t:
// RMBR: the sqrt(b2 -4ac) !! 

// Now hit_sphere shows if the ray INTERSECTS the sphere? (grazes(1) or 2 hits)? 
double hit_sphere(const point3& center, double radius, const ray& r) {
// parameters are:
// center: the center of the sphere. (object address)
// radius: the radius of the sphere. (double)
// r: the ray. (object address)

    vec3 oc =  center - r.origin();  // C - A                      We call .origin() from ray.h to give us priv param 'orig'  
    auto a = dot(r.direction(), r.direction()); // ∣∣B∣∣^2 OR B•B   We call .direction() from ray.h to give us priv param 'dir'
                                                // the dot() is from vec3.h

    auto b = -2.0 * dot(r.direction(), oc);     // -2B•(A-C) OR -2B•(oc)
    auto c = dot(oc, oc) - radius*radius;       // (A-C)•(A-C) - R^2 OR (oc)•(oc) - R^2
    auto discriminant = b*b - 4*a*c;            // B^2 - 4AC ... RMBR: the sqrt(b2 -4ac) when solving for t.

    // If 'discriminant' is greater than or equal to 0, then the ray INTERSECTS the sphere. (grazes(1) or 2 hits)
    // return (discriminant >= 0); // Now go to [*]
    if (discriminant < 0) { // [11]
        return -1.0; // No intersection
    } else {
        return (-b - std::sqrt(discriminant)) / (2.0*a); // [12]
        // RMBR:t = (-b  +- sqrt(discriminant)) / (2.0*a);
        // BUTTT we use   - sqrt(discriminant)...
        // So this returns the nearest t (intersection point). (bcz we can have 2 roots --> so get one closer to the camera)
    }
}

color ray_color(const ray& r) {
    // return color(0,0,0);

    // + SPHERE  -NO SHADE
    // if (hit_sphere(point3(0,0,-1), 0.1, r))  // [*] - check if the ray INTERSECTS the sphere.
    //              // set (x, y, z) to change the position of the SPHERE
    //     return color(1, 0, 0);  // show the sphere + set color

    // Farther sphere with a larger radius
    // if (hit_sphere(point3(0, 0, -3), 0.9, r))  // Farther sphere with a larger radius 
    //     return color(0, 1, 0);  // show the second sphere with a different color GREEN

    // if (hit_sphere(point3(0, 0, -1), 0.5, r))  // Closer sphere with a smaller radius
    //     return color(1, 0, 0);  // show the first sphere with color RED
    auto t = hit_sphere(point3(0,0,-1), 0.5, r);
    // Checks if the ray r HITS a sphere (centered at/Position of) (0,0,−1) w/ radius 0.5.
    if (t > 0.0) { // if r HITS
        vec3 N = unit_vector(r.at(t) - vec3(0,0,-1)); // Calculate the NORMAL VECTOR of sphere @ intersection point.
        // RMBR: N = (P - C) / |P - C| {p is r.at(t)}
        // RMBR: unit_vector() is from vec3.h
        return 0.5*color(N.x()+1, N.y()+1, N.z()+1); // Maps the NORMAL vector’s components [−1,1] --=> RGB color [0,1].
    }

        
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

    auto viewport_height = 2.0; // [6] - The fixed vertical size of the virtual viewport in the 3D world.
    auto viewport_width = viewport_height * (double(image_width)/image_height); // image_width =(VIEWPORT IS stretch/squeeze to FIT IMAGE Aspect Ratio)=> viewport_width
    //   viewport_width = viewport_height * aspect_ratio; XXXXX WHY NOT THIS XXXXX?
    // [7] - |P: Why didn’t we use aspect_ratio directly ? 


    // [8]
    // TODO: lower_left_corner = camera_center -  horizontal/2 ​- vertical/2 ​- focal_length * z (i.e. focal length along Z-axis)

    auto focal_length = 1.0; // dist camera to viewport [camera_center] -->[viewport_upper_left]
    auto camera_center = point3(0, 0, 0); // The center of the camera in the 3D world. 000
    
    // Viewport dimensions
    // [8] - Calculate the VECTORS (|--| and I vectors) across the horizontal and down the vertical viewport edges.
    auto horizontal  = vec3(viewport_width, 0               , 0);
    auto vertical =    vec3(0             , -viewport_height, 0); 
    //                                      -viewport_height bcz // [9]


    // + __
    // |
    //  Upper-left corner of the viewport
    //  RMBR: Rays are traced from left to right + --> + across the row.
    //  ...   after one row we go DOWN, next row.
    // [8] - Calculate the LOCATION(point3) of the upper left pixel.
    auto viewport_upper_left = camera_center - horizontal /2 - vertical/2 - vec3(0, 0, focal_length);
                                                                       // - vec3(0, 0, focal_length) is focal_length * z
                                                                       // its like: z is the vec3(0, 0, 1) and its being * by focal_length
                                                                       //           -->      vec3(0, 0, 1) * focal_length = vec3(0, 0, focal_length)
                                                                                    
    
    // [10] - Calculate the horizontal and vertical delta vectors from pixel to pixel.
    auto pixel_delta_H = horizontal  / image_width;
    auto pixel_delta_v = vertical / image_height;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_H + pixel_delta_v);
    


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
            // auto pixel_co    lor = color(double(i)/(image_width-1), double(j)/(image_height-1), 0);
            // //[4]
            auto pixel_center = pixel00_loc + (i * pixel_delta_H) + (j * pixel_delta_v);
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

/* -------------------------------------------------------------------------- */
/*                          USING COLOR.H and VEC3.H                          */
/* -------------------------------------------------------------------------- */

/* ----------------------------------- [4] ---------------------------------- */
    // write_color is a function that writes the color to the output stream.
    // ... it takes the output stream and the color as arguments.
    // ... it writes the color to the output stream.

/* ----------------------------------- [5] ---------------------------------- */
    // write_color is a function that writes the color to the output stream.
    // ... it takes the output stream and the color as arguments.
    // ... it writes the color to the output stream.


// ----------------------------------- [6] ---------------------------------- //
    // It does not directly correspond to image_height, but rather acts as the scaled reference for how tall the viewport should be.

/* ----------------------------------- [7] ---------------------------------- */
    // |P: Why didn’t we use aspect_ratio directly ? 
    // (i.e., "auto viewport_width = viewport_height * aspect_ratio;" XXXXXX)?
    // RMBR: we have an image dim (image_width and image_height), and we want to FIT(stretch/squeeze) it viewport to IMAGE (viewport_width -> image_width , viewport_height is fixed) ==> into screen IMAGE aspect ratio (ex: 16:9).
    // ... [me:] rmbr: game resolution 647x364 to 1080x1920 (WE STRETCH OR SQUEEZE the image to fit the screen/viewport)
    // ... [me:] ...  this is the same thing. We have an screen(viewport here)  and we want to FIT IT to image 

    // |P: Why didn’t we use aspect_ratio directly ? (i.e., "auto viewport_width = viewport_height * aspect_ratio;" XXXXXX)?
    // ... because while aspect_ratio(ideal aspect ratio)                    is a DOUBLE (e.g., 16.0 / 9.0), 
    // ... BUTTTT the ACTUAL image DIM (image_width and image_height) are INTs.
    // ... ==>>       ACTUAL image DIM (INT) ==// might not perfectly match //==  the ideal aspect ratio (DOUBLE) 
    // ...                                     bcz ROUNDING(double to int) or USER input.

    // eX: If we use aspect_ratio directly, the calculated viewport width may not align with the ACTUAL image dimensions (pixel grid).
    // ... For example, if image_width is 400, the calculated image_height = int(400 / (16.0 / 9.0)) = 225 (rounded).
    // ... The actual aspect ratio becomes image_width / image_height = 400 / 225 ≈ 1.777... (close to 16:9, but not exact).
    // ... [me:] see how we dont use height, and let the aspect ratio to find the height --> but since height is int --> our calculations gives double with EXTRA DECIMALS --> back to height (back to int) --> WE ROUND IT --> WE GET a round number cool BUTT WE REMOVED SOME DETAILS (NOT EXACTLY MATCHING THE SCREEN)

    // /A: =======>> SO: WE DONT WAIT FOR THE ASPECT RATIO TO FIND THE HEIGHT. => recalculate the aspect ratio using the ACTUAL image dims (h and w)
    // To ensure PERFECT ALIGNMENT between the VIEWPORT ASPECT RATIO and the IMAGE DIMENSIONS:
    // ... We recalculate the aspect ratio using the ACTUAL pixel dimensions (image_width and image_height),
    // ... guaranteeing that the viewport perfectly matches the image grid.


// ----------------------------------- [8] ---------------------------------- //
    // The camera is positioned at the origin ( 0 , 0 , 0 ) (0,0,0). Rays are cast from the camera through the viewport.
    // RMBR: Rays are cast from the camera through the viewport.

    // Viewport Geometry:

    // The upper-left corner of the viewport is calculated as:

    // upper_left_corner= camera_origin -  horizontal/2 ​- vertical/2 ​- focal_length * z
    // Where:
    // Horizontal Vector (horizontal): Spans the viewport Width.
    // Vertical Vector (vertical): Spans the viewport Height.
    // Focal Length: Distance between the camera and the viewport.


// ----------------------------------- [9] ---------------------------------- //
    // we used -vertical (INVERTED) bcz:
    // [3D] Coordinate System:                             Positive Y goes upward.  (from bottom to top)
    // [2D/Image] Coordinate System:                       Positive Y goes downward (from top to bottom).
    // more details:
    // In [2D] image formats (e.g., PPM, BMP, PNG):
    // ... the origin: (0,0) is at top-left corner
    // ... [ Y-axis    : increases from top to bottom. ] !!
    // ... X-axis    : increases from left to right.
    // ... its like this bcz TRADITIONALLY 2D screens, drew pixels from top-left corner ROW by ROW.

    // BUTTTTT:
    // In [3D] we have:
    // ... the origin: (0,0,0) is at where camera position is at.
    // ... [ Y-axis    : increases from bottom to top. ] !!
    // ....[x].. [z]... dw

// ----------------------------------- [10] ---------------------------------- //
    // The entire point of pixel00_loc and delta adjustments is:
    // [to NUDGE rays to even spaces between pixels.]
    // Essentially w    e will get NOISE and artifacts from the rays we/camera/eye sets 
    // ... So we set some even spaces pixels between rows and columns and rows
    // ... then we NUDGE(move) the rays to the center of the close ones.

    // SO now we can:
    // Control noise and artifacts by aligning rays to pixel centers.
    // Ensure consistency and uniform sampling across the viewport.
    // Lay a solid foundation for accurate rendering, especially when scenes get complex with objects, lighting, and shading.
