#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
public:
    /* -------------------------------------------------------------------------- */
    /*                                 CONSTRUCTOR                                */
    /* -------------------------------------------------------------------------- */
    ray() {}
    // Default constructor
    ray(const point3& origin, const vec3& direction)
        : orig(origin), dir(direction) {}
    // Parameterized constructor

    /* -------------------------------------------------------------------------- */
    /*                                 ACCESSOR(Getter)                           */
    /* -------------------------------------------------------------------------- */
    point3 origin() const { return orig; }
    vec3 direction() const { return dir; }


    /* -------------------------------------------------------------------------- */
    /*                                 RAY FUNCTION                               */
    /* -------------------------------------------------------------------------- */
    point3 at(double t) const {
        return orig + t * dir;
    }
    // the function P(t) = A + tB, where A: orig and  B: dir
    // RMBR: A is the origin of the ray, and B is the direction of the ray,
    // ... and t is a scalar parameter that determines how far along the ray we are.
    // ... so P(t) is a point along the ray at time t.
    // Think of a laser pointer:
    // [orig] is where the laser starts (your hand holding the laser pointer).
    // [dir] is the direction the laser is pointing.
    // [t] determines how far along the laser beam you’re measuring:
    // ... t=0: At your hand.
    // ... t=1: A point on the beam 1 unit away from your hand.
    // ... t=10: A point on the beam 10 units away.


    // Beautiful usage/reason:
    // orig = camera (where ray starts/ eyes !)
    // dir = direction (where ray is pointing)
    // t = time/distance (how far along the ray we want to go (is it hitting something?))
    // ... t is a CONTROL MECHANISM. Ray is gonna HIT SOMETHING ==> 
    // ... ==> so we use t to keep an eye on WHERE or WHEN ray hits something ==> to then use this data to  CALCULATE stuff like shadow, reflection, refraction,....

    


/* -------------------------------------------------------------------------- */
/*                                 PRIVATE PARAMETERS                         */
/* -------------------------------------------------------------------------- */
// bcz Protect Integrity + Encapsulation
private:
    point3 orig; // ACTUAL origin of the ray
    vec3 dir;    // ACTUAL direction of the ray
};


#endif
