#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>


/* -------------------------------------------------------------------------- */
/*                              Why This Section is Important                 */
/* -------------------------------------------------------------------------- */
// Foundation for 3D Math: All ray-object intersections, lighting calculations, and transformations rely on vector math.
// Reusability: The vec3 class provides a clean, reusable abstraction for 3D operations.

class vec3 {
    public:
        double e[3]; // Array to store x, y, z components.

       /* -------------------------------------------------------------------------- */
       /*                                 CONSTRUCTOR                                */
       /* -------------------------------------------------------------------------- */

        // Default constructor (vector initialized to (0, 0, 0)).
        vec3() {               // Default constructor
            e[0] = 0;
            e[1] = 0;
            e[2] = 0;          // Init e to (0, 0, 0)
        }

        // modern C++ way:
        // vec3() : e{0, 0, 0} {}
        //                     {} is empty constructor body

        // Constructor with specified x, y, z values.
        vec3(double e0, double e1, double e2) {  // Parameterized constructor
            e[0] = e0;
            e[1] = e1;
            e[2] = e2;
        }
        // modern C++ way:
        // vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}
        //                                                       {} is empty constructor body
        // ex:
        // const vec3 v_const(4.0, 5.0, 6.0);  // v_const is a constant vector

        /* -------------------------------------------------------------------------- */
        /*                         ACCESSOR METHODS (Getters)                         */
        /* -------------------------------------------------------------------------- */

        double x() const { return e[0]; }
        double y() const { return e[1]; }
        double z() const { return e[2]; }

        /* -------------------------------------------------------------------------- */
        /*                              BASIC OPERATIONS                              */
        /* -------------------------------------------------------------------------- */
       
        vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
        // Negate the vector (e.g., -(1, 2, 3) -> (-1, -2, -3)).
        // if type vec3, then when we use vec3 v = -v1 , it will return vec3(-e[0], -e[1], -e[2])
        
        double operator[](int i) const { return e[i]; }
        // Access by index (read-only).
        // if type double, then when we use v[i] , it will return e[i]
        
        double& operator[](int i) { return e[i]; }
        //    &                  x const
        // Access by index (read-AND-WRITE).
        // if type double& , then when we use v[i] = 1 , it will return e[i] = 1


        /* -------------------------------------------------------------------------- */
        /*                           OPERATIONS WITH VECTORS                          */
        /* -------------------------------------------------------------------------- */
        
        vec3& operator+=(const vec3& v) {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }
        // Add vector v2 to the current vector.
        // if type vec3& , then when we use v1 += v2 , it will return v1 = v1 + v2

        vec3& operator*=(const double t) {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }
        // Multiply the vector by a scalar.
        // if type vec3& , then when we use v1 *= 2 , it will return v1 = v1 * 2

        vec3& operator/=(double t) {
            return *this *= 1 / t;
            // (value of the object) = (value of the object) * (1 / t)
        }
        // Divide the vector by a scalar.
        // if type vec3& , then when we use v1 /= 2 , it will return v1 = v1 / 2

        /* -------------------------------------------------------------------------- */
        /*                              Utility Functions                             */
        /* -------------------------------------------------------------------------- */

        double length() const {
        return std::sqrt(length_squared());
        }
        // Return the length of the vector.
        
        double length_squared() const {
            return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
        }
        // Return the square of the length of the vector (cheaper than length).

};



using point3 = vec3; 
// "using point3" means that point3 is an ALIAS for vec3
// ex: point3 p(1, 2, 3); is the same as vec3 p(1, 2, 3);


/* -------------------------------------------------------------------------- */
/*                          Vector Utiltiy Functions                          */
/* -------------------------------------------------------------------------- */
inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}
// Add two vectors.
// if type vec3, then when we use v1 + v2 , it will return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2])
// if type double or int or any other type, then when we use v1 + v2 , it will return v1 + v2

inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}
// Subtract two vectors.
// if type vec3, then when we use v1 - v2 , it will return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2])
// if type double or int or any other type, then when we use v1 - v2 , it will return v1 - v2

inline vec3 operator*(double t, const vec3& v) {
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}
// Multiply a vector by a scalar.
// if type vec3, then when we use t * v , it will return vec3(t * v.e[0], t * v.e[1], t * v.e[2])
// if type double or int or any other type, then when we use t * v , it will return t * v
    
inline double dot(const vec3& u, const vec3& v) {
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}
// Dot product of two vectors.
// if type double or int or any other type, then when we use u.dot(v) , it will return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2]
// if type vec3, then when we use u.dot(v) , it will return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2]

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(
        u.e[1] * v.e[2] - u.e[2] * v.e[1],
        u.e[2] * v.e[0] - u.e[0] * v.e[2],
        u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}
// Cross product of two vectors.
// if type vec3, then when we use u.cross(v) , it will return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1], u.e[2] * v.e[0] - u.e[0] * v.e[2], u.e[0] * v.e[1] - u.e[1] * v.e[0])
// if type double or int or any other type, then when we use u.cross(v) , it will return u.e[1] * v.e[2] - u.e[2] * v.e[1]


#endif