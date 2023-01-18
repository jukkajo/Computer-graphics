#ifndef SPHERE_H
#define SPHERE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>

#include <iostream>
using namespace std;
///TODO:
///Implement functions and add more fields as necessary
class Sphere : public Object3D
{
public:
    Sphere() {
        //yksikköpallo
        center = Vector3f(0.0f, 0.0f, 0.0f);
        radius = 1.0f;
    }

    Sphere(Vector3f center, float radius, Material* material) : Object3D(material) {
    //tänne tallenna syötteen arvot
       this->center=center;
       this->radius=radius;
       this->material=material;
    }


    ~Sphere() {}
    
   /* ========================
     n=normaali 
     P(r)=origin
     P(0)=pinnan piste
     u(r)=suunta-vektori
     t = (n * (P(r)-P(0))) / n * u(r)
    =================================

   =================================== 
   t = h.getT()
   =================================== */ 
    
    
    
    
    
    virtual bool intersect(const Ray& r, Hit& h, float tmin) {
        float t, k_a, k_b, k_c, n, r_tulo, k_d, n4, n1, n0; //onhan tämä muistihukkaa, mutta selkeyttää
        
        //2r
        r_tulo = radius * radius;
        n1 = 1.0f;
        n0 = 0.0f;
        n = 2.0f;
        n4 = 4.0f;

        //suunta p-tulo suunta
        k_a = Vector3f::dot(r.getDirection(), r.getDirection());
        // (origin p-tulo suunta) - (suunta p-tulo center)
        k_b = Vector3f::dot(r.getOrigin(), r.getDirection()) - Vector3f::dot(r.getDirection(), center);

        k_b = n * k_b;
        // k_c = sqrt(r.getOrigin()) - n * Vector3f::dot(r.getOrigin(),center) + sqrt(center) - r_tulo;
        k_c = r.getOrigin().absSquared() - n * Vector3f::dot(r.getOrigin(),center) + center.absSquared() - r_tulo;
        k_d = (k_b * k_b) - (n4 * k_a * k_c);
        
        /*   if((t > tmin) && (t < ?)) {
        
        return true;
        } */
        if(k_d < n0) {
           return false;
        }        
        
        
        if(k_d == n0) {
           t = (-n1 * k_b) / (n * k_a);    
        }
        
        else {
           float uusi_t_1 = (-k_b + sqrt(k_d)) / (n * k_a);
           float uusi_t_2 = (-k_b - sqrt(k_d)) / (n * k_a);
           if(uusi_t_1 < uusi_t_2 && uusi_t_1 > n0) { t = uusi_t_1; }
           else { t = uusi_t_2; }
        }
        
        //päivitetään intersection-piste
        if((t > tmin) && (t < h.getT())) {
           //Hit.h => void set(float _t, Material* m, const Vector3f& n)
           //Ray.h => Vector3f pointAtParameter(float t) const
           //n?
           
           h.set(t, this->material, (r.pointAtParameter(t) - center).normalized() );
           return true;
        }        

        return false;        

    }

protected:
   float radius;
   Vector3f center;

};


#endif
