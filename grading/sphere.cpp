#include "sphere.h"
#include "ray.h"

// Determine if the ray intersects with the sphere
Hit Sphere::Intersection(const Ray& ray, int part) const
{
    //TODO;
     double a, b, c;
     a = 1.0; 
     b = dot(2.0*ray.direction, (ray.endpoint-center));
     c = (dot(ray.endpoint - center, ray.endpoint - center) - pow(radius, 2.0));
     double discriminant = pow(b, 2.0) - 4*a*c;
     if(discriminant < 0) return {0, 0, 0};
     if(discriminant < small_t) return {0, 0, 0}; //Treat as no intersection if dis = 0
     double t;
     t = (-1.0*b - sqrt(discriminant)) / 2.0;
     if(t >= small_t)  return {this, t, 0 };
     t = (-1.0*b + sqrt(discriminant)) / 2.0; 
     if(t >= small_t)  return {this, t, 0 };

    return {0,0,0};
}

vec3 Sphere::Normal(const vec3& point, int part) const
{
    vec3 normal;
    TODO; // compute the normal direction
    return normal;
}

Box Sphere::Bounding_Box(int part) const
{
    Box box;
    TODO; // calculate bounding box
    return box;
}
