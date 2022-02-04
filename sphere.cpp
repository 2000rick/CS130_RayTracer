#include "sphere.h"
#include "ray.h"

// Determine if the ray intersects with the sphere
Hit Sphere::Intersection(const Ray& ray, int part) const
{
     double b, c; //a is 1 so omitted
     b = dot(2.0*ray.direction, (ray.endpoint-center));
     c = (dot(ray.endpoint - center, ray.endpoint - center) - radius*radius);

     double discriminant = b*b - 4*c;
     if(discriminant < small_t) return {0, 0, 0}; //Treat as no intersection if dis = 0 or negative

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
    normal = (point - center).normalized();

    return normal;
}

Box Sphere::Bounding_Box(int part) const
{
    Box box;
    TODO; // calculate bounding box
    return box;
}
