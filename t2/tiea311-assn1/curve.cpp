#include "curve.h"
#include "extra.h"
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
using namespace std;

namespace
{
    // Approximately equal to.  We don't want to use == because of
    // precision issues with floating point.
    inline bool approx(const Vector3f& lhs, const Vector3f& rhs)
    {
        const float eps = 1e-8f;
        return (lhs - rhs).absSquared() < eps;
    }


}


Curve evalBezier(const vector< Vector3f >& P, unsigned steps)
{   
    // Check
    if (P.size() < 4 || P.size() % 3 != 1)
    {
        cerr << "evalBezier must be called with 3n+1 control points." << endl;
        exit(0);
    }

    //steps = montako pistetta kaaressa
    // palauta Curve "otus" jossa tavaraa
    
    // TODO:
    // You should implement this function so that it returns a Curve
    // (e.g., a vector< CurvePoint >).  The variable "steps" tells you
    // the number of points to generate on each piece of the spline.
    // At least, that's how the sample solution is implemented and how
    // the SWP files are written.  But you are free to interpret this
    // variable however you want, so long as you can control the
    // "resolution" of the discretized spline curve with it.

    // Make sure that this function computes all the appropriate
    // Vector3fs for each CurvePoint: V,T,N,B.
    // [NBT] should be unit and orthogonal.

    // Also note that you may assume that all Bezier curves that you
    // receive have G1 continuity.  Otherwise, the TNB will not be
    // be defined at points where this does not hold.

    cerr << "\t>>> evalBezier has been called with the following input:" << endl;

    cerr << "\t>>> Control points (type vector< Vector3f >): " << endl;
    for (unsigned i = 0; i < P.size(); ++i)
    {
        cerr << "\t>>> " << P[i] << endl;
    }

    cerr << "\t>>> Steps (type steps): " << steps << endl;
    cerr << "\t>>> Returning filled curve." << endl;
    
    // Preallocate a curve with steps+1 CurvePoints
    
    //TODO: laske bezier alikaarien määrä jotka linkitetään
    int alikaariLaskuri = 0;
    int karkiLaskuri = 0;
    for(unsigned i = 0; i < P.size(); i++) {
    
       karkiLaskuri++;
       if(karkiLaskuri == 4) { karkiLaskuri = 1; alikaariLaskuri += 1; }
      // alikaariLaskuri += 4;
    }

    Curve R(alikaariLaskuri * steps + 1);
    //Bernsteinin mstriisi
    Matrix4f matriisi (1.0f, -3.0f, 3.0f, -1.0f,
                       0.0f, 3.0f, -6.0f, 3.0f,
                       0.0f, 0.0f, 3.0f, -3.0f, 
                       0.0f, 0.0f, 0.0f, 1.0f);
    //spline matriisi
    Matrix4f matriisi_B (0.0f, -3.0f, 6.0f, -3.0f,
                       0.0f, 3.0f, -12.0f, 9.0f,
                       0.0f, 0.0f, 6.0f, -9.0f, 
                       0.0f, 0.0f, 0.0f, 3.0f);
                       
    Vector3f binormaali;
    Vector3f ed_binormaali;
    
    int k = 0;
    
    for (unsigned i = 0; i < P.size()-3; i += 3) {
       binormaali = (i == 0) ? Vector3f(0.0f,0.0f,1.0f) : ed_binormaali;
       
       Matrix4f kontrolliPisteet(P[i][0], P[i+1][0], P[i+2][0], P[i+3][0],
                                 P[i][1], P[i+1][1], P[i+2][1], P[i+3][1],
                                 P[i][2], P[i+1][2], P[i+2][2], P[i+3][2],
                                 0.0f, 0.0f, 0.0f, 0.0f);
       
       for (unsigned j= 0; j <= steps; j++) {

          float t = float(j) / steps;
          
          Vector4f monoBasis(1,t,t*t,t*t*t);
          // Initialize position 


          // lasketaan V
          R[k].V = Vector3f((kontrolliPisteet * matriisi * monoBasis)[0],
                            (kontrolliPisteet * matriisi * monoBasis)[1],
                            (kontrolliPisteet * matriisi * monoBasis)[2]);


          // Lasketaan T
          R[k].T = Vector3f((kontrolliPisteet * matriisi_B * monoBasis)[0],
                            (kontrolliPisteet * matriisi_B * monoBasis)[1],
                            (kontrolliPisteet * matriisi_B * monoBasis)[2]).normalized();
          
          R[k].N = Vector3f::cross(binormaali, R[k].T).normalized();

          R[k].B = Vector3f::cross(R[k].T, R[k].N).normalized();
          ed_binormaali = R[k].B;
          
          k++;
        }

    }    

    return R;
}

Curve evalBspline(const vector< Vector3f >& P, unsigned steps)
{
    // Check
    if (P.size() < 4)
    {
        cerr << "evalBspline must be called with 4 or more control points." << endl;
        exit(0);
    }

    cerr << "\t>>> evalBSpline has been called with the following input:" << endl;

    cerr << "\t>>> Control points (type vector< Vector3f >): " << endl;
    for (unsigned i = 0; i < P.size(); ++i)
    {
        cerr << "\t>>> " << P[i] << endl;
    }

    cerr << "\t>>> Steps (type steps): " << steps << endl;
    Curve R;
    bool yks;
    Matrix4f matBsplini(1.0f / 6, -3.0f / 6, 3.0f / 6, -1.0f / 6,
                       4.0f / 6, 0.0f, -6.0f / 6, 3.0f / 6,
                       1.0f / 6, 3.0f / 6, 3.0f / 6, -3.0f / 6, 
                       0.0f, 0.0f, 0.0f, 1.0f / 6);
                       
    Matrix4f matriisi2 (1.0f, -3.0f, 3.0f, -1.0f,
                       0.0f, 3.0f, -6.0f, 3.0f,
                       0.0f, 0.0f, 3.0f, -3.0f, 
                       0.0f, 0.0f, 0.0f, 1.0f);
                       
    Matrix4f kaanteinen_matriisi2 = matriisi2.inverse(&yks);

    //int k = 0;
    for (unsigned i = 0; i < P.size()-3; i++) {
    
       Matrix4f kontrolliPisteet(P[i][0], P[i+1][0], P[i+2][0], P[i+3][0],
                                 P[i][1], P[i+1][1], P[i+2][1], P[i+3][1],
                                 P[i][2], P[i+1][2], P[i+2][2], P[i+3][2],
                                 0.0f, 0.0f, 0.0f, 0.0f);
                                 
       Matrix4f gb = kontrolliPisteet * matBsplini * kaanteinen_matriisi2;
       vector<Vector3f> vee;
       Vector3f tmp;
       Vector4f  cl;
       
       for (unsigned ind = 0; ind <= 3; ind++) {
       cl = gb.getCol(ind);
       tmp[0] = cl[0];
       tmp[1] = cl[1];
       tmp[2] = cl[2];
       
       vee.push_back(tmp);
       }

       Curve aliKaari(4*steps);
       //Neljän pisteen data lähetetty Bezierille ja sen palauttama
       //kaari tallennetaan muuttujaan alikaari
       
       aliKaari = evalBezier(vee, steps);
       
       	for(unsigned p = 1; p < aliKaari.size(); p++) {
       	   CurvePoint cp;
       	   cp.V = aliKaari[p].V;
       	   cp.T = aliKaari[p].T;
       	   cp.N = aliKaari[p].N;
       	   cp.B = aliKaari[p].B;
       	   
       	   R.push_back(cp);
       	   
       	}
    }
    //alun kontrollipisteet, eli siis ne jotka tulisivat em. silmukan
    //ensimmäisellä iteraatiolla kun i = 0;
    //=> suljettu kappale
    Matrix4f kontrolliPisteet(P[0][0], P[1][0], P[2][0], P[3][0],
                              P[0][1], P[1][1], P[2][1], P[3][1],
                              P[0][2], P[1][2], P[2][2], P[3][2],
                              0.0f, 0.0f, 0.0f, 0.0f);
       
       Matrix4f gb = kontrolliPisteet * matBsplini * kaanteinen_matriisi2;
       vector<Vector3f> vee;
       Vector3f tmp;
       Vector4f  cl;
       
       for (unsigned ind = 0; ind <= 3; ind++) {
       cl = gb.getCol(ind);
       tmp[0] = cl[0];
       tmp[1] = cl[1];
       tmp[2] = cl[2];
       
       vee.push_back(tmp);
       }

       Curve aliKaari(4*steps);
       //Neljän pisteen data lähetetty Bezierille ja sen palauttama
       //kaari tallennetaan muuttujaan alikaari
       
       aliKaari = evalBezier(vee, steps);
       
       	for(unsigned p = 1; p < aliKaari.size(); p++) {
       	   CurvePoint cp;
       	   cp.V = aliKaari[p].V;
       	   cp.T = aliKaari[p].T;
       	   cp.N = aliKaari[p].N;
       	   cp.B = aliKaari[p].B;
       	   
       	   R.push_back(cp);
       	   
       	}

    return R;
}

Curve evalCircle(float radius, unsigned steps)
{
    // This is a sample function on how to properly initialize a Curve
    // (which is a vector< CurvePoint >).

    // Preallocate a curve with steps+1 CurvePoints
    Curve R(steps + 1);

    // Fill it in counterclockwise
    for (unsigned i = 0; i <= steps; ++i)
    {
        // step from 0 to 2pi
        float t = 2.0f * M_PI * float(i) / steps;

        // Initialize position
        // We're pivoting counterclockwise around the y-axis
        R[i].V = radius * Vector3f(cos(t), sin(t), 0);

        // Tangent vector is first derivative
        R[i].T = Vector3f(-sin(t), cos(t), 0);

        // Normal vector is second derivative
        R[i].N = Vector3f(-cos(t), -sin(t), 0);

        // Finally, binormal is facing up.
        R[i].B = Vector3f(0, 0, 1);
    }

    return R;
}

void drawCurve(const Curve& curve, float framesize)
{
    // Save current state of OpenGL
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    // Setup for line drawing
    glDisable(GL_LIGHTING);
    glColor4f(1, 1, 1, 1);
    glLineWidth(1);

    // Draw curve
    glBegin(GL_LINE_STRIP);
    for (unsigned i = 0; i < curve.size(); ++i)
    {
        glVertex(curve[i].V);
    }
    glEnd();

    glLineWidth(1);

    // Draw coordinate frames if framesize nonzero
    if (framesize != 0.0f)
    {
        Matrix4f M;

        for (unsigned i = 0; i < curve.size(); ++i)
        {
            M.setCol(0, Vector4f(curve[i].N, 0));
            M.setCol(1, Vector4f(curve[i].B, 0));
            M.setCol(2, Vector4f(curve[i].T, 0));
            M.setCol(3, Vector4f(curve[i].V, 1));

            glPushMatrix();
            glMultMatrixf(M);
            glScaled(framesize, framesize, framesize);
            glBegin(GL_LINES);
            glColor3f(1, 0, 0); glVertex3d(0, 0, 0); glVertex3d(1, 0, 0);
            glColor3f(0, 1, 0); glVertex3d(0, 0, 0); glVertex3d(0, 1, 0);
            glColor3f(0, 0, 1); glVertex3d(0, 0, 0); glVertex3d(0, 0, 1);
            glEnd();
            glPopMatrix();
        }
    }

    // Pop state
    glPopAttrib();
}

