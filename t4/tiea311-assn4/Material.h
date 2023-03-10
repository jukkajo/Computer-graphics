#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>

#include "Ray.h"
#include "Hit.h"
#include "texture.hpp"
///TODO:
///Implement Shade function that uses ambient, diffuse, specular and texture
class Material
{
public:

    Material(const Vector3f& d_color, const Vector3f& s_color = Vector3f::ZERO, float s = 0) :
        diffuseColor(d_color), specularColor(s_color), shininess(s)
    {

    }

    virtual ~Material()
    {

    }

    virtual Vector3f getDiffuseColor() const
    {
        return  diffuseColor;
    }

    Vector3f Shade(const Ray& ray,
                   const Hit& hit,
                   const Vector3f& dirToLight,
                   const Vector3f& lightColor)
    {
      Vector3f t_vari = diffuseColor;

      if (t.valid()) {
          t_vari = t(hit.texCoord[0], hit.texCoord[1]);
      }

      Vector3f r_vec =  ray.getDirection() - 2.f * (Vector3f::dot(ray.getDirection(), hit.getNormal())) * hit.getNormal();
      Vector3f spec_vec = specularColor * std::pow(std::max(0.f, Vector3f::dot(dirToLight, r_vec)), shininess);
      Vector3f dif_vec = t_vari * std::max(0.f, Vector3f::dot(hit.getNormal(), dirToLight));
      
     
      return  (dif_vec + spec_vec) * lightColor;
		
    }

    void loadTexture(const char* filename) {
        t.load(filename);
    }
protected:
    Vector3f diffuseColor;
    Vector3f specularColor;
    float shininess;
    Texture t;
};



#endif // MATERIAL_H
