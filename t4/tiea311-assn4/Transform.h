#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vecmath.h>
#include "Object3D.h"
///TODO implement this class
///So that the intersect function first transforms the ray
///Add more fields as necessary
class Transform : public Object3D
{
public:
    Transform() {}
    Transform(const Matrix4f& m, Object3D* obj) :o(obj) {
       this->m = m;
    }
    ~Transform() {
    }
    virtual bool intersect(const Ray& r, Hit& h, float tmin) {
    Matrix4f m_inv = this->m.inverse();
    //Ray(const Vector3f& orig, const Vector3f& dir)
    
    
    Ray uusi_ray = Ray((m_inv * Vector4f(r.getOrigin(),1.0f)).xyz(), (m_inv * Vector4f(r.getDirection(), 0.0f)).xyz());
    if(o->intersect(uusi_ray, h, tmin)) {
        return true;
    }
    h.set(h.getT(), h.getMaterial(), (m_inv.transposed() * Vector4f(h.getNormal(),   0.0f)).xyz().normalized());
    return false;
    }

protected:
    Matrix4f m;
    Object3D* o; //un-transformed object	
};

#endif //TRANSFORM_H
