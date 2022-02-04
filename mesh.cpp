#include "mesh.h"
#include <fstream>
#include <string>
#include <limits>

// Consider a triangle to intersect a ray if the ray intersects the plane of the
// triangle with barycentric weights in [-weight_tolerance, 1+weight_tolerance]
static const double weight_tolerance = 1e-4;

// Read in a mesh from an obj file.  Populates the bounding box and registers
// one part per triangle (by setting number_parts).
void Mesh::Read_Obj(const char* file)
{
    std::ifstream fin(file);
    if(!fin)
    {
        exit(EXIT_FAILURE);
    }
    std::string line;
    ivec3 e;
    vec3 v;
    box.Make_Empty();
    while(fin)
    {
        getline(fin,line);

        if(sscanf(line.c_str(), "v %lg %lg %lg", &v[0], &v[1], &v[2]) == 3)
        {
            vertices.push_back(v);
            box.Include_Point(v);
        }

        if(sscanf(line.c_str(), "f %d %d %d", &e[0], &e[1], &e[2]) == 3)
        {
            for(int i=0;i<3;i++) e[i]--;
            triangles.push_back(e);
        }
    }
    number_parts=triangles.size();
}

// Check for an intersection against the ray.  See the base class for details.
Hit Mesh::Intersection(const Ray& ray, int part) const
{
    Hit closest = {0,std::numeric_limits<double>::max(),0};
    double dist = std::numeric_limits<double>::max();

    for(int i = 0; i < triangles.size(); ++i) {
        if( Intersect_Triangle(ray, i, dist) ) {
            closest = {this, dist, i};
            return closest;
        }
    }
    
    return closest;
}

// Compute the normal direction for the triangle with index part.
vec3 Mesh::Normal(const vec3& point, int part) const
{
    assert(part>=0);
    ivec3 x = triangles.at(part); //triangle with index part
    //normal is (B-A)x(C-A) normalized
    vec3 normal = cross( vertices.at(x[1])-vertices.at(x[0]), vertices.at(x[2])-vertices.at(x[0]) );
    normal = normal.normalized();

    return normal; 
}

// This is a helper routine whose purpose is to simplify the implementation
// of the Intersection routine.  It should test for an intersection between
// the ray and the triangle with index tri.  If an intersection exists,
// record the distance and return true.  Otherwise, return false.
// This intersection should be computed by determining the intersection of
// the ray and the plane of the triangle.  From this, determine (1) where
// along the ray the intersection point occurs (dist) and (2) the barycentric
// coordinates within the triangle where the intersection occurs.  The
// triangle intersects the ray if dist>small_t and the barycentric weights are
// larger than -weight_tolerance.  The use of small_t avoid the self-shadowing
// bug, and the use of weight_tolerance prevents rays from passing in between
// two triangles.
bool Mesh::Intersect_Triangle(const Ray& ray, int tri, double& dist) const
{
    vec3 normal = Normal(ray.endpoint, tri);
    double uDotn = dot(ray.direction, normal);
    if(uDotn > -small_t && uDotn < small_t) {
        return false; //0 or inf intersections, treat as no intersection
    } 

    vec3 A = vertices.at(triangles.at(tri)[0]); //point A of triangle
    double t = dot(A - ray.endpoint, normal) / uDotn;
    if(t > small_t) {
        dist = t;
    } 
    else { //t is too small
        return false;
    }

    //barycentric weight = area(subTriangleP)/area(ABC)
    //area = 1.0/2.0 * dot( (B-A)x(C-A), n)
    vec3 P = ray.Point(t); //point P (intersection)
    double alpha, beta, gamma;
    ivec3 x = triangles.at(tri); //triangle with index tri
    vec3 B = vertices.at(x[1]); 
    vec3 C = vertices.at(x[2]);
    double triArea = dot(cross(B-A, C-A) , normal);

    alpha = dot(cross(B-P, C-P) , normal) / triArea;
    beta = dot(cross(P-A, C-A) , normal) / triArea;
    gamma = 1.0 - alpha - beta;

    if(alpha > -weight_tolerance && beta > -weight_tolerance && gamma > -weight_tolerance) {
        return true;
    }

    return false;
}

// Compute the bounding box.  Return the bounding box of only the triangle whose
// index is part.
Box Mesh::Bounding_Box(int part) const
{
    Box b;
    TODO;
    return b;
}
