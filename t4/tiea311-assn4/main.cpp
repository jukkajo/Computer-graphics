#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <string.h>

#include "SceneParser.h"
#include "Image.h"
#include "Camera.h"
#include "bitmap_image.hpp"
#include "Light.h"
#include "Ray.h"
#include "Hit.h"

using namespace std;

int main(int argc, char* argv[])
{
    std::string inp_tiedosto="";
    std::string output_tiedosto="";
    int kuvan_L=0;
    int kuvan_K=0;
    
    
    if(argc != 8) { std::cout << "Syötteen oltava muotoa: ./a4 -input scene01_plane.txt  -size 200 200 -output 1.bmp" << std::endl; return 1; } 
    // Fill in your implementation here.

    // This loop loops over each of the input arguments.
    // argNum is initialized to 1 because the first
    // "argument" provided to the program is actually the
    // name of the executable (in our case, "a4").
    //  0      1          2              3  4     5  6       7
    // ./a4 -input scene01_plane.txt  -size 200 200 -output 1.bmp
    
    //tämä on vähän tyhmästi tehty mutta menköön
    for (int argNum = 1; argNum < argc; ++argNum)
    {
    //======================================================================
    if(argNum == 1) {
   // if(argv[argNum] == "-input") {
    if(strcmp(argv[argNum], "-input") == 0) {
       inp_tiedosto = argv[argNum + 1];
       argNum += 1;
    }
    else { std::cout << "Toisen argumentin oltava: -input" << std::endl; }
    }
    //======================================================================
    if(argNum == 3) {
   // if(argv[argNum] == "-size") {
    if(strcmp(argv[argNum], "-size") == 0) {
       kuvan_L = stoi(argv[argNum + 1]);
       kuvan_K = stoi(argv[argNum + 2]);
       argNum += 2;
    }
    else { std::cout << "Kolmannen argumentin oltava: -size" << std::endl; }
    }
    //======================================================================
    if(argNum == 6) {
   // if(argv[argNum] == "-output") {
    if(strcmp(argv[argNum], "-output") == 0) {
       output_tiedosto = argv[argNum + 1];
    }
    else { std::cout << "Kolmannen argumentin oltava: -output" << std::endl; }
    }
    //======================================================================
    
    }


    // First, parse the scene using SceneParser.
    
    // Then loop over each pixel in the image, shooting a ray
    // through that pixel and finding its intersection with
    // the scene.
    
    // color  * getDiffuseColor() 
    
    SceneParser SP_otus = SceneParser(inp_tiedosto.c_str());
    
    Camera* camera = SP_otus.getCamera();
    Vector3f amb_valo = SP_otus.getAmbientLight();
    Group* g = SP_otus.getGroup();
    Image kuva = Image(kuvan_L, kuvan_K);
    float fk = 2.f;
    float fy = 1.f;
   //depth-ominaisuus tähän ehkä
         
    for(int i = 0; i < kuvan_L; i++)  {
       for(int j = 0; j < kuvan_K; j++)  {
           Vector3f color = amb_valo;
           //image.SetPixel(i, j, pixelColor);
           Hit hit = Hit();
           Ray ray = camera->generateRay(Vector2f( (-fy + (fk / kuvan_L) * i), (-fy + (fk / kuvan_K) * j)));

          float f = -fy + (fk / kuvan_L) * i;
          float e = -fy + (fk / kuvan_K) * j; 
               std::cout << f << ' ' << e << std::endl;
               
           //Ray => intersect
           if(g->intersect(ray, hit, camera->getTMin())) { //tässä kaatuu
              float h_T = hit.getT();
              Vector3f point = ray.pointAtParameter(h_T);
              Material* material = hit.getMaterial();
              color = color * material->getDiffuseColor();
           
           for(int k = 0; k < SP_otus.getNumLights(); ++k) {
              Vector3f suunta, l_color; 
              float etaisyys;
              Light* valo = SP_otus.getLight(k); 
              valo->getIllumination(point, suunta, l_color, etaisyys);
              color += material->Shade(ray, hit, suunta, l_color);
           }
         
           kuva.SetPixel(i, j, color);
      //   Jos normaali, niin: edit:enpäs tätä teekään
      /*   float a = std::abs(hit.getNormal().x());
           float b = std::abs(hit.getNormal().y());
           float c = std::abs(hit.getNormal().z());
           Vector3f color_std = Vector3f(a, b, c);
           
           kuva_n.SetPixel(i, j, color_std); */
           } 
           else { kuva.SetPixel(i,j,SP_otus.getBackgroundColor()); }
       }
    }

    kuva.SaveBMP(output_tiedosto.c_str());
    


    /*
    //TODO: below demonstrates how to use the provided Image class
    ///Should be removed when you start
    Vector3f pixelColor(1.0f, 0, 0);
    //width and height
    Image image(10, 15);
    image.SetPixel(5, 5, pixelColor);
    image.SaveImage("demo.bmp");
    return 0;*/
}
