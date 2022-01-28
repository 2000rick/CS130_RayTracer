#include "light.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"
#include "object.h"

vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    vec3 color;
    //TODO; //determine the color
    vec3 ambient = color_ambient*world.ambient_color*world.ambient_intensity;
    color += ambient;
    for( auto i : world.lights)
    {
        vec3 lightDirection = i->position-intersection_point;
        vec3 emitted = i->Emitted_Light(lightDirection);
        vec3 diffuse = color_diffuse*emitted*std::max(dot(normal, lightDirection.normalized()), 0.0);

        vec3 r = ( 2*(dot(lightDirection.normalized(), normal))*normal-lightDirection.normalized() ).normalized(); //ReflectedDirection
        vec3 c = (world.camera.position - intersection_point).normalized();
        double intensity = pow(std::max(dot(r, c), 0.0), specular_power);
        vec3 specular = color_specular*emitted*intensity;
        color += diffuse + specular;
    }
    

    // vec3 ambient = color_ambient*world.ambient_color*world.ambient_intensity;

    // vec3 lightDirection = world.lights[0]->position-intersection_point;
    // vec3 emitted = world.lights[0]->Emitted_Light(lightDirection);
    // vec3 diffuse = color_diffuse*emitted*std::max(dot(normal, lightDirection.normalized()), 0.0);

    // vec3 r = ( 2*(dot(lightDirection.normalized(), normal))*normal-lightDirection.normalized() ).normalized(); //ReflectedDirection
    // vec3 c = (world.camera.position - intersection_point).normalized();
    // double intensity = pow(std::max(dot(r, c), 0.0), specular_power);
    // vec3 specular = color_specular*emitted*intensity;
    // color = ambient + diffuse + specular;
    return color;
}
