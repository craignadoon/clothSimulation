#include <boost/foreach.hpp>
#include "ParticleOctree.h"
#include "ClothParticle.h"
//----------------------------------------------------------------------------------------------------------------------
/// @file ParticleOctree
/// @brief applies the octree for self collision checks
//----------------------------------------------------------------------------------------------------------------------
void  ParticleOctree::checkCollisionOnNode(TreeNode <ClothParticle> *node)
{
    if(node->m_height !=1)
    {
        for(int i=0;i<8;++i)
        {
            checkCollisionOnNode(node->m_child[i]);
        }
    }
    else
    {
        if(node->m_objectList.size()<=1)
        {
            return;
        }
        ngl::Vec3 P, Q, Vp, N, force;
        float Rp, Rq, dist, speed;
        BOOST_FOREACH(ClothParticle *currentParticle,node->m_objectList)
        {
            P = currentParticle->getPosition();
            Vp = currentParticle->getVelocity();
            speed = Vp.length();
            Rp = 0.1;
            //currentParticle->getRadius();
            force.m_x = force.m_y = force.m_z = 0.0;

            BOOST_FOREACH(ClothParticle *testParticle,node->m_objectList )
            {
                // no need to self test
                if(testParticle==currentParticle)
                {
                  // continue
                  continue;
                 }
                Q = testParticle->getPosition();
                Rq = 0.1;
                        //testParticle->getRadius();
                dist = Rq + Rq - (P-Q).length();
                if(dist > 0) // collision between P and Q
                {
                    N = P-Q;
                    N.normalize();
                    force += dist*N;
                }
            }
            Vp += force;
            Vp.normalize();
            currentParticle->setVelocity(force);
        }
    }
}

