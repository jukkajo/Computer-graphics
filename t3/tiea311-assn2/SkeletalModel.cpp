#include "SkeletalModel.h"
#include <string>
#include <FL/Fl.H>

using namespace std;

void SkeletalModel::load(const char* skeletonFile, const char* meshFile, const char* attachmentsFile)
{
    loadSkeleton(skeletonFile);

    m_mesh.load(meshFile);
    m_mesh.loadAttachments(attachmentsFile, m_joints.size());

    computeBindWorldToJointTransforms();
    updateCurrentJointToWorldTransforms();
}

void SkeletalModel::draw(Matrix4f cameraMatrix, bool skeletonVisible)
{
    // draw() gets called whenever a redraw is required
    // (after an update() occurs, when the camera moves, the window is resized, etc)

    m_matrixStack.clear();
    m_matrixStack.push(cameraMatrix);

    if (skeletonVisible)
    {
        drawJoints();

        drawSkeleton();
    }
    else
    {
        // Clear out any weird matrix we may have been using for drawing the bones and revert to the camera matrix.
        glLoadMatrixf(m_matrixStack.top());

        // Tell the mesh to draw itself.
        m_mesh.draw();
    }
}

// *joint, missä '*' = osoite muistipaikkaan
//3 x float (suhde/parent) & parentin indeksi
void SkeletalModel::loadSkeleton(const char* filename)
{  
   ifstream inp_virta;
   inp_virta.open(filename);
   
   if(inp_virta.is_open()) {
      //tähän 3 ekaa floattia
      Vector3f vektori;
      int sijainti;
      string sailo;
     
      if(getline(inp_virta, sailo)) {
         stringstream ss;
         ss.str(sailo); // 1.XXXX 2.YYYY 3.ZZZZ 4.DDDD
         ss >> vektori[0] >> vektori[1] >> vektori[2] >> sijainti;
         Joint* joint = new Joint;
         joint->transform = Matrix4f::translation(vektori);
         joint->children.push_back(joint);
         // save root joint 

         m_rootJoint = joint;
         
         m_joints.push_back(joint);
      }
   /*================================================================================
                -1
              / /\ \
             1 3 12 15
            /      \ \
           2       13 16
          / \        \ \
         4   8       14 17
        /     \
       5       9
      /         \
     6          10
                  \
                  11
   ================================================================================*/
      while(getline(inp_virta, sailo)/*np_virta.is_open() == true*/) {
      //if(inp_virta) {
         stringstream ss;
         ss.str(sailo); // 1.XXXX 2.YYYY 3.ZZZZ 4.DDDD
         ss >> vektori[0] >> vektori[1] >> vektori[2] >> sijainti; //korvaa edelliset
         
         Joint* joint = new Joint;
         joint->transform = Matrix4f::translation(vektori);
    
         m_joints.push_back(joint);
         m_joints[sijainti]->children.push_back(joint);
       }
   }
}

void SkeletalModel::rekursio(Joint *solmu) {
   m_matrixStack.push(solmu->transform);
   glLoadMatrixf(m_matrixStack.top());
   glutSolidSphere(0.025f, 12, 12);
   
 //  cout << "tänne asti päästiin/2" << endl;
   
   /* int koko = m_rootJoint->children.size();
   for(int i = 0; i < koko; ++i) {
     rekursio(solmu->children[i]);
    }*/ 
    
  // cout << "tänne asti päästiin/3" << endl;
   
   std::vector< Joint* >::iterator iteraattori;
   for(iteraattori = solmu->children.begin(); iteraattori != solmu->children.end(); ++iteraattori){
      rekursio(*iteraattori);
   }
    
    m_matrixStack.pop();
} 

void SkeletalModel::drawJoints()
{
    // Draw a sphere at each joint. You will need to add a recursive helper function to traverse the joint hierarchy.
    //
    // We recommend using glutSolidSphere( 0.025f, 12, 12 )
    // to draw a sphere of reasonable size.
    //
    // You are *not* permitted to use the OpenGL matrix stack commands
    // (glPushMatrix, glPopMatrix, glMultMatrix).
     
    // You should use your MatrixStack class
    // and use glLoadMatrix() before your drawing call.
   /*  
   m_matrixStack.push(Matrix4f::translation(1,0,0));
   glLoadMatrixf(m_matrixStack.top());
   glutSolidSphere( 0.025f, 12, 12 ); */
   
   //valgrind --leak-check=full --track-origins=yes ./a2 data/model1
   //valgrind -v ./a2 data/model1
   
    //Access not within mapped region at address 0x0
 
    Matrix4f tr = m_rootJoint->transform;
    m_matrixStack.push(tr);
    tr = m_rootJoint->children[1]->transform;
    
    glLoadMatrixf(m_matrixStack.top());
    //juuri on ns. itsensä lapsi ja positiossa 0, joten aloitus indeksistä 1
    int koko = m_rootJoint->children.size();
    /* std::string s = std::to_string(koko);
    cout << s << endl; */
    
    for(int i = 1; i < koko; ++i) {
        rekursio(m_rootJoint->children[i]);

    }
    m_matrixStack.pop();
    glLoadMatrixf(m_matrixStack.top());
}

//rekursiivinen funktio "luiden"-piirrolle
//z = [−0.5, −0.5, 0] to [0.5, 0.5, 1]
//box = [−0.025, −0.025, 0] to [0.025, 0.025, etäisyys seuraavan niveleen] 
void SkeletalModel::rekursio2(Joint *solmu) {


   if(!solmu->children.empty()) {
   

      m_matrixStack.push(solmu->transform);
      int koko = solmu->children.size();
      for(int i = 0; i < koko; ++i) {
      
        
      //  x1 y1 z1 0
      //  x2 y2 z2 0
      //  x3 y3 z3 0
      //  0  0  0  1
        
      //  x = y x z 
      //  y = z x round
      //  z = x y z
        
        //vector3f * 3 => matrix4f
       
      Vector3f round(0, 0, 1);
        

     //   N N N 0
     //   N N N 0
     //   N N N 0
     //   N N N N
     
        Vector3f tr_z = solmu->children[i]->transform.getCol(3).xyz();
        tr_z.normalize();
        
        Vector3f y = Vector3f::cross(tr_z, round).normalized();
        Vector3f x = Vector3f::cross(y, tr_z).normalized();
        
        Matrix3f k_v_m3x3(x, y, tr_z);
        
        //kannanvaihto matriisi
        Matrix4f k_v_m4x4 = Matrix4f::identity();
        
        k_v_m4x4.setSubmatrix3x3(0, 0, k_v_m3x3);
        
        m_matrixStack.push(k_v_m4x4);
       // m_matrixStack.pop();
        
        m_matrixStack.push(Matrix4f::scaling(0.025, 0.025, solmu->children[i]->transform.getCol(3).xyz().abs()));
        
        m_matrixStack.push(Matrix4f::translation(0.0, 0.0, 0.5));
        
        glLoadMatrixf(m_matrixStack.top());
        glutSolidCube(1);
        
        //skaalaus, translaatio, rotaatio "ulos"
        m_matrixStack.pop();
        m_matrixStack.pop();
        m_matrixStack.pop();
        
        glLoadMatrixf(m_matrixStack.top());
        rekursio2(solmu->children[i]);

      }
      m_matrixStack.pop();
   }
}

void SkeletalModel::drawSkeleton()
{
   Matrix4f transformoitu = m_rootJoint->transform;
    m_matrixStack.push(transformoitu); // "aloitus" juuresta
   
    //melkein voisi tehdä yleisen rekursio a-ohjelman
    int koko = m_rootJoint->children.size();
    //juuri on ns. itsensä lapsi ja positiossa 0, joten aloitus indeksistä 1
    for(int j = 1; j < koko; ++j) {
       rekursio2(m_rootJoint->children[j]);
    }
    
    // Draw boxes between the joints. You will need to add a recursive helper function to traverse the joint hierarchy.
}

void SkeletalModel::setJointTransform(int jointIndex, float rX, float rY, float rZ)
{
    // Set the rotation part of the joint's transformation matrix based on the passed in Euler angles.
    Matrix4f nivel = m_joints[jointIndex]->transform;


    Matrix4f sailo = Matrix4f::rotateX(rX) * Matrix4f::rotateY(rY) * Matrix4f::rotateZ(rZ);
    
    nivel.setSubmatrix3x3(0, 0, sailo.getSubmatrix3x3(0,0));
    m_joints[jointIndex]->transform = nivel;
}


void SkeletalModel::wToJoint_rekursio(Joint* solmu) {
   m_matrixStack.push(solmu->transform);
   solmu->bindWorldToJointTransform = m_matrixStack.top();
   for(int i = 0; i < solmu->children.size(); ++i) {
      wToJoint_rekursio(solmu->children[i]);
   }
   m_matrixStack.pop(); //pino alkutilaan
}


void SkeletalModel::computeBindWorldToJointTransforms()
{
    // 2.3.1. Implement this method to compute a per-joint transform from
    // world-space to joint space in the BIND POSE.
    //
    // Note that this needs to be computed only once since there is only
    // a single bind pose.
    //
    // This method should update each joint's bindWorldToJointTransform.
    // You will need to add a recursive helper function to traverse the joint hierarchy.
    m_matrixStack.clear();
    m_matrixStack.push(m_rootJoint->transform);
    m_rootJoint->bindWorldToJointTransform = m_rootJoint->transform;
    for(int i = 1; i < m_rootJoint->children.size(); ++i) {
       wToJoint_rekursio(m_rootJoint->children[i]);
    }
     m_matrixStack.pop(); //pino alkutilaan
}


void SkeletalModel::jToWorld_rekursio(Joint* solmu) {
   //ToJoint_rekursio-funktio käänteisenä
   m_matrixStack.push(solmu->transform);
   solmu->currentJointToWorldTransform = m_matrixStack.top();
   
   for(int i = 0; i < solmu->children.size(); ++i) {
      jToWorld_rekursio(solmu->children[i]);
   }
   m_matrixStack.pop(); 
}


void SkeletalModel::updateCurrentJointToWorldTransforms()
{
    // 2.3.2. Implement this method to compute a per-joint transform from
    // joint space to world space in the CURRENT POSE.
    //
    // The current pose is defined by the rotations you've applied to the
    // joints and hence needs to be *updated* every time the joint angles change.
    //
    // This method should update each joint's bindWorldToJointTransform.
    // You will need to add a recursive helper function to traverse the joint hierarchy.
    m_matrixStack.clear();
    m_matrixStack.push(m_rootJoint->transform);
    m_rootJoint->currentJointToWorldTransform = m_rootJoint->transform;
    for(int i = 1; i < m_rootJoint->children.size(); ++i) {
       jToWorld_rekursio(m_rootJoint->children[i]);
    }
    m_matrixStack.pop();  
}

void SkeletalModel::updateMesh()
{
    // 2.3.2. This is the core of SSD.
    // Implement this method to update the vertices of the mesh
    // given the current state of the skeleton.
    // You will need both the bind pose world --> joint transforms.
    // and the current joint --> world transforms.
    vector<Vector3f> kaerjet;
    //iteroidaan verkon k-pisteet
    for(int i = 0; i < m_mesh.currentVertices.size(); ++i) {
       Vector4f p_summa(0.0f);
       Vector4f pisteet(m_mesh.bindVertices[i], 1.0f);
       
    for(int ind = 0; ind < m_joints.size()-1; ++ind) {
       //jos "attachmenteillä" ei "painoa", niin ei lasketa
       if(m_mesh.attachments[i][ind] > 0) {
          //kärkien uudet lokaatio tilassa
          p_summa = p_summa + m_mesh.attachments[i][ind] * (m_joints[ind+1]->currentJointToWorldTransform * m_joints[ind +1]->bindWorldToJointTransform.inverse() * pisteet); // +1 => hypätään juurisolmun yli
       }
     }
     kaerjet.push_back(p_summa.xyz());
     }
     //pävitetään kun laskettu 
     m_mesh.currentVertices = kaerjet;
    
}

