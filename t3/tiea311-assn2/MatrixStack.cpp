#include "MatrixStack.h"

MatrixStack::MatrixStack()
{
    m_matrices.push_back(Matrix4f::identity());
    // Initialize the matrix stack with the identity matrix.
}

void MatrixStack::clear()
{
    m_matrices.clear(); //tyhj. matriisi
    m_matrices.push_back(Matrix4f::identity());  // lis. id-matriisi
}

Matrix4f MatrixStack::top()
{
    // palauta pinon ylin
    return m_matrices.back();
}

void MatrixStack::push(const Matrix4f& m)
{
    //ent. pinon yli * uusi => lisätään pinoon
   m_matrices.push_back(m_matrices.back() * m);

}

void MatrixStack::pop()
{
    // poista pinon ylin
    m_matrices.pop_back();
}
//renderöidessä kutsu  glLoadMatrixf( m matrixStack.top() ) 
