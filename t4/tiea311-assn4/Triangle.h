#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>
#include <iostream>

using namespace std;
///TODO: implement this class.
///Add more fields as necessary,
///but do not remove hasTex, normals or texCoords
///they are filled in by other components
class Triangle : public Object3D
{
public:
    Triangle();

    ///@param a b c are three vertex positions of the triangle
    Triangle(const Vector3f& a, const Vector3f& b, const Vector3f& c, Material* m) :Object3D(m) {
        this->a = a;
        this->b = b;
        this->c = c;
        hasTex = false;
    }
    
    
    /* implement the ray-triangle intersection, preferably using barycentric coordinates
    
    barycentric coordinates λ0, λ1, λ2
    vertex positions a, b, c,
                     |  |   |
    vertex normals  n0, n1, n2,
    
    interpolated normal => λ0n0 + λ1n1 + λ2n2 */
    virtual bool intersect(const Ray& ray, Hit& hit, float tmin) {
        Vector3f ray_dir = ray.getDirection();
        Vector3f ray_orig= ray.getOrigin();

        
        //vector= det(Ai) / det(A) i = 1..n
        /*
        
  det(A)  x a-b, a-c, a-ray_dir
          y -||-
          z -||-
         
         β x a-ray_orig, a-c, ray_dir
           y -||-
           z -||-
         
         γ x a-b, a-ray_orig, ray_dir
           y -||-
           z -||-
        
       b_k x a-b, a-c, a-ray_orig
           y -||-
           z -||-

        */
        
        Matrix3f matr_A = Matrix3f( a.x() - b.x(), a.x() - c.x(), ray_dir.x(),
		        	    a.y() - b.y(), a.y() - c.y(), ray_dir.y(),
			            a.z() - b.z(), a.z() - c.z(), ray_dir.z() );
			                    
	Matrix3f gamma_matr = Matrix3f(a.x() - b.x(), a.x() - ray_orig.x(), ray_dir.x(),
			               a.y() - b.y(), a.y() - ray_orig.y(), ray_dir.y(),
			               a.z() - b.z(), a.z() - ray_orig.z(), ray_dir.z());
        
	Matrix3f beeta_matr = Matrix3f(a.x() - ray_orig.x(), a.x() - c.x(), ray_dir.x(),
			               a.y() - ray_orig.y(), a.y() - c.y(), ray_dir.y(),
			               a.z() - ray_orig.z(), a.z() - c.z(), ray_dir.z());
			                
	Matrix3f b_k_matr = Matrix3f(a.x() - b.x(), a.x() - c.x(), a.x() - ray_orig.x(),
			             a.y() - b.y(), a.y() - c.y(), a.y() - ray_orig.y(),
			             a.z() - b.z(), a.z() - c.z(), a.z() - ray_orig.z());

        //  λ0n0 + λ1n1 + λ2n2
        
        float determin_A = matr_A.determinant();
        float alpha = 1.f;
        float beeta = beeta_matr.determinant() / determin_A;
        float gamma = gamma_matr.determinant() / determin_A;
        float b_k = b_k_matr.determinant() / determin_A;
        alpha = alpha - beeta - gamma;
        //==================================================
        //  float z = 0.0f; float o = 1.0f;
        //  if(b_k >= tmin && b_k < hit.getT()) {
        //     if(o >= (beeta + gamma) && gamma >= z && beeta >= z) {
        if (b_k >= tmin && b_k < hit.getT() && beeta + gamma <= 1 && beeta >= 0 && gamma >= 0) {     
              Vector3f norm = (alpha * normals[0] + beeta * normals[1] + gamma * normals[2]).normalized();
              //set(float _t, Material* m, const Vector3f& n)
              hit.setTexCoord(alpha * texCoords[0] + beeta * texCoords[1] + gamma * texCoords[2]);
              hit.set(b_k, this->material, norm);
              return true;
         }
        
        return false;
    }
    bool hasTex;
    Vector3f normals[3];
    Vector2f texCoords[3];
protected:
   Vector3f a, b, c;
};

#endif //TRIANGLE_H
