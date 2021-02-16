#include "ClothParticle.h"
//----------------------------------------------------------------------------------------------------------------------
/// @file   ClothParticle.cpp
/// @brief  class that create the Cloth's mass Particles. the state of particles is updated via the RK4 integrator
//----------------------------------------------------------------------------------------------------------------------
ClothParticle::ClothParticle()
{
    m_mass = 0.5f;
    m_fixed = false;
    m_force.set(0.0f,0.0f,0.0f);
    m_gravity = ngl::Vec3(0,-9.8,0);
}
//----------------------------------------------------------------------------------------------------------------------
ClothParticle::ClothParticle(
                            ngl::Vec3 _pos,
                            ngl::Vec3 _vel,
                            float _mass, bool _fixed
                            )
{
    m_state.m_position = _pos;
    m_state.m_velocity = _vel;
    m_mass = _mass;
    m_fixed = _fixed;
    m_force.set(0.0f,0.0f,0.0f);
    m_gravity = ngl::Vec3(0,-2.00,0); //2.45 stable and fast
        m_timestep = 0.025;
        m_t=0.0;
}
//----------------------------------------------------------------------------------------------------------------------
void ClothParticle::update()
{
    if(m_fixed == false)
    {
    integrate(m_t,m_timestep);
    m_t+=m_timestep;
    }
}
//----------------------------------------------------------------------------------------------------------------------
ngl::Vec3 ClothParticle::motionFunction(const State &_state, float _t)
{
    Q_UNUSED(_t);
    return m_force*0.5;
}
//----------------------------------------------------------------------------------------------------------------------
void ClothParticle::applyGravity()
{
    m_force+=m_gravity;
}
//----------------------------------------------------------------------------------------------------------------------
