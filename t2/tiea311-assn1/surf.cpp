#include "surf.h"
#include "extra.h"
using namespace std;

namespace
{

    // We're only implenting swept surfaces where the profile curve is
    // flat on the xy-plane.  This is a check function.
    static bool checkFlat(const Curve& profile)
    {
        for (unsigned i = 0; i < profile.size(); i++)
            if (profile[i].V[2] != 0.0 ||
                profile[i].T[2] != 0.0 ||
                profile[i].N[2] != 0.0)
                return false;

        return true;
    }
}

/*pyöräytys xy
1. (kaaren pisteet * M_PI * 2.0f) / steps + indeksi 
2. määritä normaalit ja pinnat
syötteestä normaalit vektorit N 


2.kärjistä pinta =>
loop profiilin koon verran
*/
Surface makeSurfRev(const Curve& profile, unsigned steps)
{
    Surface surface;

    if (!checkFlat(profile))
    {
        //cerr << "surfRev profile curve must be flat on xy plane." << endl;
        exit(0);
    }
    
   for (unsigned i = 0; i < profile.size(); i++) {
   // for (unsigned j = 0; j < profile.size(); j++) {
         for (unsigned j = 0; j <= steps; j++) {
         //paljonko käännetään,                 
         float kulma = 2.0f * 3.14159265359 * float( j ) / steps;
         
         //Normaali   
         Matrix4f htm = Matrix4f::rotateY(kulma);
         Matrix3f htm_a_mat = htm.getSubmatrix3x3(0,0);
         Matrix3f htm_a_mat_trans = htm_a_mat.transposed();
         Matrix3f n_kaanteinen = htm_a_mat_trans.inverse();
         
         //Pinnan kärki
         Vector4f pinta = Vector4f(profile[i].V[0], profile[i].V[1], profile[i].V[2], 1.0f);
         Vector4f pinnan_v_al = htm*pinta;
         Vector3f pinnan_v = Vector3f(pinnan_v_al[0], pinnan_v_al[1], pinnan_v_al[2]);
         
         //pinnan normaali
         Vector3f p_VN = n_kaanteinen * profile[i].N;

	 surface.VV.push_back(pinnan_v);
	 surface.VN.push_back(-1*p_VN);
      }
   }
   
    for (unsigned o=0; o < surface.VV.size() - (steps+1);o++){
	Tup3u ekaKolmio, tokaKolmio;

	if ((o+1)%(steps+1) != 0)
	{
	    ekaKolmio = Tup3u(o+1, o, o+steps+1);
            tokaKolmio = Tup3u(o+1, o+1+steps, o+2+steps);   
        }

	surface.VF.push_back(ekaKolmio);
	surface.VF.push_back(tokaKolmio);  
    }


    return surface;
}

Surface makeGenCyl(const Curve& profile, const Curve& sweep)
{
    Surface surface;

    if (!checkFlat(profile))
    {
        cerr << "genCyl profile curve must be flat on xy plane." << endl;
        exit(0);
    }
   for (unsigned i = 0; i < profile.size(); i++) {
   // for (unsigned j = 0; j < profile.size(); j++) {
         for (unsigned j = 0; j <= sweep.size(); j++) {
         Matrix4f koord_m(sweep[j].N[0], sweep[j].B[0], sweep[j].T[0], sweep[j].V[0], 
			 sweep[j].N[1], sweep[j].B[1], sweep[j].T[1], sweep[j].V[1], 
		         sweep[j].N[2], sweep[j].B[2], sweep[j].T[2], sweep[j].V[2], 
		         0.f, 0.f, 0.f, 1.0f);
         //Normaali   
         //Matrix4f htm = Matrix4f::rotateY(kulma);
         Matrix3f htm_a_mat = koord_m.getSubmatrix3x3(0,0);
         Matrix3f htm_a_mat_trans = htm_a_mat.transposed();
         Matrix3f n_kaanteinen = htm_a_mat_trans.inverse();
         
         //Pinnan kärki
         Vector4f pinta = Vector4f(profile[i].V[0], profile[i].V[1], profile[i].V[2], 1.0f);
         Vector4f pinnan_v_al = koord_m*pinta;
         Vector3f pinnan_v = Vector3f(pinnan_v_al[0], pinnan_v_al[1], pinnan_v_al[2]);
         
         //pinnan normaali
         Vector3f p_VN = n_kaanteinen * profile[i].N;

	 surface.VV.push_back(pinnan_v);
	 surface.VN.push_back(-1*p_VN);
      }
   }
   
    for (unsigned o=0; o < surface.VV.size() - (sweep.size());o++){
	Tup3u ekaKolmio, tokaKolmio;

	if ((o+1)%(sweep.size()+1) != 0)
	{
	    ekaKolmio = Tup3u(o+1, o, o+sweep.size()+1);
            tokaKolmio = Tup3u(o+1, o+1+sweep.size(), o+2+sweep.size());   
        }

	surface.VF.push_back(ekaKolmio);
	surface.VF.push_back(tokaKolmio);  
    }


    return surface;
}

void drawSurface(const Surface& surface, bool shaded)
{
    // Save current state of OpenGL
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    if (shaded)
    {
        // This will use the current material color and light
        // positions.  Just set these in drawScene();
        glEnable(GL_LIGHTING);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // This tells openGL to *not* draw backwards-facing triangles.
        // This is more efficient, and in addition it will help you
        // make sure that your triangles are drawn in the right order.
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }
    else
    {
        glDisable(GL_LIGHTING);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glColor4f(0.4f, 0.4f, 0.4f, 1.f);
        glLineWidth(1);
    }

    glBegin(GL_TRIANGLES);
    for (unsigned i = 0; i < surface.VF.size(); i++)
    {
        glNormal(surface.VN[surface.VF[i][0]]);
        glVertex(surface.VV[surface.VF[i][0]]);
        glNormal(surface.VN[surface.VF[i][1]]);
        glVertex(surface.VV[surface.VF[i][1]]);
        glNormal(surface.VN[surface.VF[i][2]]);
        glVertex(surface.VV[surface.VF[i][2]]);
    }
    glEnd();

    glPopAttrib();
}

void drawNormals(const Surface& surface, float len)
{
    // Save current state of OpenGL
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    glDisable(GL_LIGHTING);
    glColor4f(0, 1, 1, 1);
    glLineWidth(1);

    glBegin(GL_LINES);
    for (unsigned i = 0; i < surface.VV.size(); i++)
    {
        glVertex(surface.VV[i]);
        glVertex(surface.VV[i] + surface.VN[i] * len);
    }
    glEnd();

    glPopAttrib();
}

void outputObjFile(ostream& out, const Surface& surface)
{

    for (unsigned i = 0; i < surface.VV.size(); i++)
        out << "v  "
        << surface.VV[i][0] << " "
        << surface.VV[i][1] << " "
        << surface.VV[i][2] << endl;

    for (unsigned i = 0; i < surface.VN.size(); i++)
        out << "vn "
        << surface.VN[i][0] << " "
        << surface.VN[i][1] << " "
        << surface.VN[i][2] << endl;

    out << "vt  0 0 0" << endl;

    for (unsigned i = 0; i < surface.VF.size(); i++)
    {
        out << "f  ";
        for (unsigned j = 0; j < 3; j++)
        {
            unsigned a = surface.VF[i][j] + 1;
            out << a << "/" << "1" << "/" << a << " ";
        }
        out << endl;
    }
}
