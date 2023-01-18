#ifndef PLANE_H
#define PLANE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>

using namespace std;
///TODO: Implement Plane representing an infinite plane
///choose your representation , add more fields and fill in the functions
class Plane : public Object3D
{
public:
    Plane() {}
    Plane(const Vector3f& normal, float d, Material* m) :Object3D(m) {
       this->normal = normal;
       this->d = d;   
    }
    /* The Hit data structure must be initialized
with a very large t value (try FLT MAX) */
    ~Plane() {}
    virtual bool intersect(const Ray& r, Hit& h, float tmin) {
       float nim = Vector3f::dot(normal, r.getDirection());
       float max = FLT_MAX;
      
       if(nim != 0) { max = (d - Vector3f::dot(normal, r.getOrigin())) / Vector3f::dot(normal, r.getDirection()); }
       else {
        return false;
       }
       if (max > tmin && max < h.getT()) {
       	  h.set(max, this->material, normal);
	  return true;
       }

       return false;
       
    }

protected:
   Vector3f normal;
   float d;
};
#endif //PLANE_H


