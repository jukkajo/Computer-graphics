#ifndef GROUP_H
#define GROUP_H


#include "Object3D.h"
#include "Ray.h"
#include "Hit.h"
#include <iostream>

using  namespace std;

///TODO: 
///Implement Group
///Add data structure to store a list of Object* 
class Group : public Object3D
{
public:

    Group() {

    }

    Group(int num_objects) {
       lkm = num_objects;
       g = vector<Object3D*>(lkm);
    }

    ~Group() {

    }

    virtual bool intersect(const Ray& r, Hit& h, float tmin) {
        bool t_or_f = false;
        for(int i = 0; i < lkm; i++) {
        if(g[i]->intersect(r,h,tmin)) { t_or_f = true; }
        }
        return t_or_f;
    }

    void addObject(int index, Object3D* obj) {
        g[index] = obj;
    }

    int getGroupSize() {
       return lkm;
    }

private:
  int lkm;
  vector<Object3D*> g;
};

#endif
