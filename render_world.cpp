#include "render_world.h"
#include "flat_shader.h"
#include "object.h"
#include "light.h"
#include "ray.h"

extern bool disable_hierarchy;

Render_World::Render_World()
    :background_shader(0),ambient_intensity(0),enable_shadows(true),
    recursion_depth_limit(3)
{}

Render_World::~Render_World()
{
    delete background_shader;
    for(size_t i=0;i<objects.size();i++) delete objects[i];
    for(size_t i=0;i<lights.size();i++) delete lights[i];
}

// Find and return the Hit structure for the closest intersection.  Be careful
// to ensure that hit.dist>=small_t.
Hit Render_World::Closest_Intersection(const Ray& ray)
{
    double min_t = std::numeric_limits<double>::max();
    Hit closest_hit; Hit hit;
    closest_hit = {0, 0, 0};
    
    for(auto i : objects) {
        hit = i->Intersection(ray, 0);
        if(hit.object != 0) {
            if(hit.dist < min_t && hit.dist >= small_t) {
                closest_hit = hit; 
                min_t = hit.dist;
            }
        }
    }

    return closest_hit;
}

// set up the initial view ray and call
void Render_World::Render_Pixel(const ivec2& pixel_index)
{
    vec3 direction = camera.World_Position(pixel_index) - camera.position;
    Ray ray(camera.position, direction.normalized() );

    vec3 color=Cast_Ray(ray, 1);
    camera.Set_Pixel(pixel_index,Pixel_Color(color));
}

void Render_World::Render()
{
    if(!disable_hierarchy)
        Initialize_Hierarchy();

    for(int j=0;j<camera.number_pixels[1];j++)
        for(int i=0;i<camera.number_pixels[0];i++)
            Render_Pixel(ivec2(i,j));
}

// cast ray and return the color of the closest intersected surface point,
// or the background color if there is no object intersection
vec3 Render_World::Cast_Ray(const Ray& ray,int recursion_depth)
{
    vec3 color;
    Hit close = Closest_Intersection(ray);
    if(close.object != 0) {
        vec3 point = ray.Point(close.dist);
        vec3 normal = close.object->Normal(point, close.part);
        color = close.object->material_shader->Shade_Surface( ray, point, normal, recursion_depth );
    }
    else {
        color = this->background_shader->Shade_Surface( ray, {1,2,3}, {4,5,6}, recursion_depth );
    }

    return color;
}

void Render_World::Initialize_Hierarchy()
{
    //TODO; // Fill in hierarchy.entries; there should be one entry for
    // each part of each object.

    hierarchy.Reorder_Entries();
    hierarchy.Build_Tree();
}
