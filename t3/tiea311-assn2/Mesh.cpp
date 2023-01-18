#include "Mesh.h"

using namespace std;

void Mesh::load(const char* filename)
{
    // 2.1.1. load() should populate bindVertices, currentVertices, and faces

    // Add your code here.
   ifstream inp_virta;
   inp_virta.open(filename);
   
   if(inp_virta.is_open()) {

      string jono;

      while(getline(inp_virta, jono)/*np_virta.is_open() == true*/) {
      
        Vector3f karki;
        string m_jono;
        stringstream ss;
        Tuple3u sailo;
         
        ss.str(jono);
        ss >> m_jono; //datan tyyppi => kärki/pinta
        if(m_jono == "v") { ss >> karki[0] >> karki[1] >> karki[2];
           bindVertices.push_back(karki);
        }
        
        if(m_jono == "f") { ss >> sailo[0] >> sailo[1] >> sailo[2];
           faces.push_back(sailo);
        }
        

       }
   }

    // make a copy of the bind vertices as the current vertices
    currentVertices = bindVertices;
}

void Mesh::draw()
{
    // Since these meshes don't have normals
    // be sure to generate a normal per triangle. => ristitulo
    // Notice that since we have per-triangle normals
    // rather than the analytical normals from
    // assignment 1, the appearance is "faceted".
    unsigned vmp1, vmp2, vmp3; // säilöt pinnan koordinaateillwe
    glBegin(GL_TRIANGLES);
    for(int i = 0; i < faces.size(); ++i) {
    //glBegin(GL_TRIANGLES); 
    //glNormal3d(vecn[c-1][0], vecn[c-1][1], vecn[c-1][2]);
       vmp1 = faces[i][0]-1;
       vmp2 = faces[i][1]-1;
       vmp3 = faces[i][2]-1;
       /*
           |  
           a 
          /|\<=> .normalized() => 1     
         b | c
       */    
       Vector3f Vvmp1 = currentVertices[vmp2] - currentVertices[vmp1];
       Vector3f Vvmp2 = currentVertices[vmp3] - currentVertices[vmp1];
       Vector3f normaali = Vector3f::cross(Vvmp1, Vvmp2).normalized();
       
       glNormal3d(normaali[0], normaali[1], normaali[2]);
       glVertex3d(currentVertices[vmp1][0], currentVertices[vmp1][1], currentVertices[vmp1][2]);
       glVertex3d(currentVertices[vmp2][0], currentVertices[vmp2][1], currentVertices[vmp2][2]);
       glVertex3d(currentVertices[vmp3][0], currentVertices[vmp3][1], currentVertices[vmp3][2]);
       
    }
    glEnd(); 
}

void Mesh::loadAttachments(const char* filename, int numJoints)
{
    // 2.2. Implement this method to load the per-vertex attachment weights
    // this method should update m_mesh.attachments
    // mesh.h / std::vector< std::vector< float > > attachments;
    
       ifstream inp_virta;
   inp_virta.open(filename);
   
   if(inp_virta.is_open()) {

      string jono;

      while(getline(inp_virta, jono)/*np_virta.is_open() == true*/) {
      
        vector<float> vektori;
        float arvo;
        stringstream ss;

        ss.str(jono);
        while(ss) { // niin kauan kuin virrassa tavaraa, otetaan merkkejä
          ss >> arvo;
          vektori.push_back(arvo);
        }
        attachments.push_back(vektori); //päivitys
       }
   }
}
