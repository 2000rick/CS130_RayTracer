#include "reflective_shader.h"
#include "ray.h"
#include "render_world.h"

vec3 Reflective_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    vec3 color; 
    color = (1-reflectivity) * shader->Shade_Surface(ray, intersection_point, normal, recursion_depth);
    if(recursion_depth < world.recursion_depth_limit) {
        color = shader->Shade_Surface(ray, intersection_point, normal, recursion_depth); //Phong shading
        vec3 u = ray.direction;
        vec3 r = ( -2*(dot(normal, u))*normal+u ).normalized(); //Reflected direction
        Ray reflected(intersection_point, r); //reflected ray
        color = (1-reflectivity)*color + reflectivity*world.Cast_Ray(reflected, recursion_depth+1);
    }
    
    return color;
}
