#include "ClothSpring.h"
//----------------------------------------------------------------------------------------------------------------------
/// @file   ClothSpring.cpp
/// @brief  class that create the Cloth's springs and calculates the internal spring forces
//----------------------------------------------------------------------------------------------------------------------
ClothSpring::ClothSpring()
{
    m_restLen = 1.0f;
    m_k = 2.0f;
    m_damping = 1.0f;
    m_springType = 0;
}
//----------------------------------------------------------------------------------------------------------------------
ClothSpring::ClothSpring(
                        int _id,
                        ClothParticle* _posA,
                        ClothParticle* _posB,
                        float _k,
                        float _d,
                        int _stype
                        )
{
    m_id   = _id;
    m_posA = _posA;
    m_posB = _posB;
    m_restLen = (m_posB->getPosition() - m_posA->getPosition()).length();
    m_k = _k;
    m_damping = _d;
    m_springType = _stype;
}
//----------------------------------------------------------------------------------------------------------------------
ngl::Vec3 ClothSpring::calSpringForces()
{
    ngl::Vec3 velocity = m_posB->getVelocity() - m_posA->getVelocity();
    ngl::Vec3 distance = m_posB->getPosition() - m_posA->getPosition();
    float length = distance.length();
    return -m_k*(length-m_restLen)*(distance/length)-m_damping*velocity;
}
//----------------------------------------------------------------------------------------------------------------------
