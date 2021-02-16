#ifndef PARTICLEOCTRRE_H__
#define PARTICLEOCTRRE_H__

#include "AbstractOctree.h"

class ClothParticle;

class ParticleOctree : public AbstractOctree <ClothParticle,ngl::Vec3>
{
  public :
     ParticleOctree(int _height, BoundingBox _limit)
       :
       AbstractOctree <ClothParticle,ngl::Vec3> ( _height,  _limit)
     {}

     virtual void  checkCollisionOnNode(TreeNode <ClothParticle> *node);



};

#endif

