#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"
#include <vecmath.h>
#include <float.h>
#include <cmath>


class Camera
{
public:
    //generate rays for each screen-space coordinate
    virtual Ray generateRay(const Vector2f& point) = 0;

    virtual float getTMin() const = 0;
    virtual ~Camera() {}
protected:
    Vector3f center;
    Vector3f direction;
    Vector3f up;
    Vector3f horizontal;

};

///TODO: Implement Perspective camera
///Fill in functions and add more fields if necessary

//suuntavektori(t)=(pinnanpiste-cameracenter).normalized
class PerspectiveCamera : public Camera
{
public:
    PerspectiveCamera(const Vector3f& center, const Vector3f& direction, const Vector3f& up, float angle) {
    //tämä nolla niin ei normalisoida
    this->center=center; 
    this->direction=direction.normalized();
    this->up=up.normalized();
    this->horizontal = Vector3f::cross(this->direction, up).normalized();
    k_del = n / std::tanh(angle/n2);
    }

    virtual Ray generateRay(const Vector2f& point) {
       // Vector3f rat = (k_del * direction + point.x() * horizontal + point.y() * up).normalized();
        Vector3f rat = point.x() * horizontal + point.y() * up + k_del * direction;
        rat.normalize();
        
        //return Ray(Vector3f(1, 0, 0), Vector3f(1, 0, 0));
        return Ray(center, rat);
    }

    virtual float getTMin() const {
        return 0.0f;
    }

private:
   float k_del;
   float n = 1.0f;
   float n2 = 2.0f;
};

#endif //CAMERA_H
