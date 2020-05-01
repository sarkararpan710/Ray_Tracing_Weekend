#ifndef RAYH
#define RAYH
#include "vec3.h"

struct ray
{
    vec3 A;
    vec3 B;

    ray set_ray(const vec3& a, const vec3& b) const
    {
        ray *r = (ray*)malloc(sizeof(ray));
        r->A=a;
        r->B=b;
        return *r;
    }

    vec3 origin() const
    {
        return A;
    }

    vec3 direction() const
    {
        return B;
    }

    vec3 point_at_parameter(float t) const
    {
        return A + t*B;
    }
};
/*
class ray   
{
    public:
        vec3 A,B;
        ray() {}
        ray(const vec3& a, const vec3& b) 
        {
            A=a;
            B=b;
        }
        vec3 origin() const {return A;}
        vec3 direction() const {return B;}
        vec3 point_at_parameter(float t) const {return A + t*B;}
        
};
*/
#endif