// Code formatting: 4 space indent, no tabs (spaces only!)
// Opening brace of functions and control structures on end-of-line.
// Might have some other nuances..
// try to check that your git diff plays nicely with original!
#ifdef WIN32
#include <GL/freeglut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <vecmath.h>
#include <string>

using namespace std;

#define MAX_BUFFER_SIZE 8981

// Globaalit
bool liikkeessa = false;
int onoff = 0;
const float p_nopeus = 2;
float kulma = 0;

int aste = 0;
int vali = 40;
int o = 0;

GLfloat dC[4] = { 0.5, 0.5, 0.9, 1.0 };

GLfloat dCa[10][4] = {{0.5, 0.5, 0.9, 1.0},   // +0.4   em +0.4 
               {0.9, 0.5, 0.5, 1.0},   // -0.4 +0.4 -0.2
               {0.5, 0.9, 0.3, 1.0},   // +0.5 +0.1 -0.3
               {1.0, 1.0, 0.0, 1.0},   //   em -0.8   em
               {1.0, 0.2, 0.0, 1.0},   //   em +0.2   em
               {1.0, 0.0, 0.0, 1.0},   // -0.5 +0.5 +0.5
               {0.5, 0.5, 0.5, 1.0},   // -0.5   em   em
               {0.0, 0.5, 0.5, 1.0},   //   em +0.5 -0.5
               {0.0, 1.0, 0.0, 1.0},   // +0.3 +0.2 +0.9
               {0.3, 0.8, 0.9, 1.0} };

GLfloat yksi = 1.0f;
GLfloat kaksi = 1.0f;
// This is the list of points (3D vectors)
vector<Vector3f> vecv;

// This is the list of normals (also 3D vectors)
vector<Vector3f> vecn;

// This is the list of faces (indices into vecv and vecn)
vector<vector<unsigned> > vecf;


void updateFunc(int syote) {
    if (syote != onoff)
        return;

    glutTimerFunc(1000 / 60, updateFunc, syote);

    kulma = fmod(kulma + p_nopeus, 360);

    glutPostRedisplay();
}

// These are convenience functions which allow us to call OpenGL 
// methods on Vec3d objects
inline void glVertex(const Vector3f& a) { glVertex3fv(a); }

inline void glNormal(const Vector3f& a) { glNormal3fv(a); }


// This function is called whenever a "Normal" key press is received.
void keyboardFunc(unsigned char key, int x, int y) {

    switch (key) {
    case 27: // Escape key
        exit(0);
        break;

    case 'c':
        aste += 1;
        if(aste < vali) { o = 0;}
        if(vali < aste) { o = 1;}
        if((vali*2) < aste) { o = 2;}
        if((vali*3) < aste) { o = 3;}
        if((vali*4) < aste) { o = 4;}
        if((vali*5) < aste) { o = 5;}
        if((vali*6) < aste) { o = 6;}
        if((vali*7) < aste) { o = 7;}
        if((vali*8) < aste) { o = 8;}
        if((vali*9) < aste) { o = 9;}
        if(aste == (vali*10)) {
           aste = 0; dC[0] = dCa[0][0];
           dC[1] = dCa[0][1]; dC[2] = dCa[0][2];
           dC[3] = dCa[0][3];
         }

        dC[0] += ((dCa[o+1][0] - dCa[o][0]) / vali);
        dC[1] += ((dCa[o+1][1] - dCa[o][1]) / vali);
        dC[2] += ((dCa[o+1][2] - dCa[o][2]) / vali); 
        dC[3] = 1.0;
        cout << dC[0] << "  " << dC[1] << "  " << dC[2] << endl;
       // {0.5, 0.5, 0.9, 1.0},   // +0.4   em +0.4 
       // {0.9, 0.5, 0.5, 1.0},   // -0.4 +0.4 -0.2
       // {0.5, 0.9, 0.3, 1.0},   // +0.5 +0.1 -0.3
       // {1.0, 1.0, 0.0, 1.0},   //   em -0.8   em
       // {1.0, 0.2, 0.0, 1.0},   //   em +0.2   em
       // {1.0, 0.0, 0.0, 1.0},   // -0.5 +0.5 +0.5
       // {0.5, 0.5, 0.5, 1.0},   // -0.5   em   em
       // {0.0, 0.5, 0.5, 1.0},   //   em +0.5 -0.5
       // {0.0, 1.0, 0.0, 1.0},   // +0.3 +0.2 +0.9
       // {0.3, 0.8, 0.9, 1.0} };
        break;
    case 'r':
        liikkeessa =!liikkeessa;
        if (liikkeessa) {
            updateFunc(onoff);
        }
        else {
            onoff++;
        }

        break;
        
    default:
        cout << "Unhandled key press " << key << "." << endl;
    }

    // this will refresh the screen so that the user sees the color change
    glutPostRedisplay();
}



// This function is called whenever a "Special" key press is received.
// Right now, it's handling the arrow keys.
void specialFunc(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        kaksi += 0.5f;
        cout << "siirretään valoa ylös, 0.5f verran" << endl;
        break;
    case GLUT_KEY_DOWN:
        kaksi -= 0.5f;
        cout << "siirretään valoa alas, 0.5f verran" << endl;
        break;
    case GLUT_KEY_LEFT:
        yksi += 0.5f;
        cout << "siirretään valoa vasemmalle, 0.5f verran" << endl;
        break;
    case GLUT_KEY_RIGHT:
        yksi -= 0.5f;
        cout << "siirretään valoa oikealle, 0.5f verran" << endl;
        break;
    }

    // this will refresh the screen so that the user sees the light position
    glutPostRedisplay();
}



// This function is responsible for displaying the object.
void drawScene(void) {

    // Clear the rendering window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Rotate the image
    glMatrixMode(GL_MODELVIEW);  // Current matrix affects objects positions
    glLoadIdentity();              // Initialize to the identity

    // Position the camera at [0,0,5], looking at [0,0,0],
    // with [0,1,0] as the up direction.
    gluLookAt(0.0, 0.0, 5.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    // Set material properties of object

    // Here are some colors you might use - feel free to add more

    // Here we use the first color entry as the diffuse color
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, dC);

    // Define specular color and shininess
    GLfloat specColor[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat shininess[] = { 100.0 };

    // Note that the specular color and shininess can stay constant
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specColor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    // Set light properties

    // Light color (RGBA)
    GLfloat Lt0diff[] = { 1.0,1.0,1.0,1.0 };
    // Light position
    GLfloat Lt0pos[] = { yksi, kaksi, 5.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
    glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);
    glRotatef(kulma, 0, 1, 0);
    // This GLUT method draws a teapot.  You should replace
    // it with code which draws the object you loaded.
  // glutSolidTeapot(1.0);

   glBegin(GL_TRIANGLES);
   for(unsigned int j=0; j < vecv.size(); j++) {
    
    int a,c,d,f,g,i;
    a = vecf[j][0];
    c = vecf[j][2];
    d = vecf[j][3];
    f = vecf[j][5];
    g = vecf[j][6];    
    i = vecf[j][8];
    glBegin(GL_TRIANGLES); 
    glNormal3d(vecn[c-1][0], vecn[c-1][1], vecn[c-1][2]); 
    glVertex3d(vecv[a-1][0], vecv[a-1][1], vecv[a-1][2]); 
    glNormal3d(vecn[f-1][0], vecn[f-1][1], vecn[f-1][2]); 
    glVertex3d(vecv[d-1][0], vecv[d-1][1], vecv[d-1][2]); 
    glNormal3d(vecn[i-1][0], vecn[i-1][1], vecn[i-1][2]); 
    glVertex3d(vecv[g-1][0], vecv[g-1][1], vecv[g-1][2]);
    }
    glEnd();
        

    // Dump the image to the screen.
    glutSwapBuffers();


}


// Initialize OpenGL's rendering modes
void initRendering() {
    glEnable(GL_DEPTH_TEST);   // Depth testing must be turned on
    glEnable(GL_LIGHTING);     // Enable lighting calculations
    glEnable(GL_LIGHT0);       // Turn on light #0.
}


void reshapeFunc(int w, int h) {

    if (w > h) {
        glViewport((w - h) / 2, 0, h, h);
    }
    else {
        glViewport(0, (h - w) / 2, w, w);
    }

    // Set up a perspective view, with square aspect ratio
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // 50 degree fov, uniform aspect ratio, near = 1, far = 100
    gluPerspective(50.0, 1.0, 1.0, 100.0);
}

void loadInput() {
   char buffer[MAX_BUFFER_SIZE];
   while(cin.getline(buffer, MAX_BUFFER_SIZE))
   {
      stringstream ss(buffer);
      Vector3f v;
      vector<unsigned> f;
      string s;
      string kviiva = "/";

      ss >> s; 

      if(s == "v") {
         ss >> v[0] >> v[1] >> v[2];
         vecv.push_back(v);
      }
      
      if(s == "vn") {
         ss >> v[0] >> v[1] >> v[2];
       vecn.push_back(v);
       }
       
      if(s == "f") {
         for(int i = 0; i < 3; i++) {
         ss >> s;
         size_t pA = 0;
         string jii;
         while((pA = s.find(kviiva)) != std::string::npos ) {
            jii = s.substr(0,pA);
            f.push_back(atoi(jii.c_str()));
            s.erase(0, pA + kviiva.length());
      } f.push_back(atoi(s.c_str()));
      }
      vecf.push_back(f);
   }
 }
}


// Main routine.
// Set up OpenGL, define the callbacks and start the main loop
int main(int argc, char** argv) {

    loadInput();

    glutInit(&argc, argv);

    // We're going to animate it, so double buffer 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Initial parameters for window position and size
    glutInitWindowPosition(60, 60);
    glutInitWindowSize(1200, 800);
    glutCreateWindow("Assignment 0");

    // Initialize OpenGL parameters.
    initRendering();

    // Set up callback functions for key presses
    glutKeyboardFunc(keyboardFunc); // Handles "normal" ascii symbols
    glutSpecialFunc(specialFunc);   // Handles "special" keyboard keys

     // Set up the callback function for resizing windows
    glutReshapeFunc(reshapeFunc);

    // Call this whenever window needs redrawing
    glutDisplayFunc(drawScene);

    // Start the main loop.  glutMainLoop never returns.
    glutMainLoop();


    return 0;	// This line is never reached.
}
