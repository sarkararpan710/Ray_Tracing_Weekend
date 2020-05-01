#ifndef CAMERAH
#define CAMERAH

#include "ray.h"

struct camera
{
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 origin;
};

//Probably initiate the camera in main function with probably changing coordinates for lower lef corner, horizontal and origin.
//In main function we declare the co-ordinates of the camera and instantiate them here in this function.
camera initiate(vec3 lower_left_corner, vec3 horizontal, vec3 vertical, vec3 origin)
{
    camera c ;
    c.horizontal = vec3(4.0,0.0,0.0);
    c.vertical = vec3(0.0, 2.0, 0.0);
    c.lower_left_corner = vec3(-2.0, -1.0, -1.0);
    c.origin = vec3(0.0,0.0,0.0);

    return c;
}

//get ray function defined here where the ray is instantiated and then returned where the set function is called
ray get_ray(float u, float v, vec3& llc, vec3& horizontal, vec3& vertical, vec3& origin)
{
    camera cam = initiate(llc, horizontal, vertical, origin);
    ray r;
    return (r.set_ray(cam.origin, (cam.lower_left_corner + (u*cam.horizontal) + (v*cam.vertical - cam.origin))));
    
};
/*
class camera{
    public:
        camera() {
            lower_left_corner = vec3(-2.0, -1.0, -1.0);
            horizontal = vec3(4.0, 0.0, 0.0);
            vertical = vec3(0.0, 2.0, 0.0);
            origin = vec3(0.0, 0.0, 0.0);
        }
        ray get_ray(float u, float v){
            return ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
        }

        vec3 origin;
        vec3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
};
*/
#endif
