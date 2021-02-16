#include "Cloth.h"
//----------------------------------------------------------------------------------------------------------------------
/// @file   Cloth.cpp
/// @brief  class creating and updating the cloth
//----------------------------------------------------------------------------------------------------------------------
Cloth::Cloth(){
    m_horiParticles = 10.0f; //10 20    60
    m_vertParticles = 10.0f; //10 20    60
    mWidth = 10.0f;         //5
    mHeight = 10.0f;        //5
}
//----------------------------------------------------------------------------------------------------------------------
Cloth::Cloth(
            std::string _texture
            )
{
    m_horiParticles = 10.0f; //10 20    60
    m_vertParticles = 10.0f; //10 20    60
    mWidth = 5.0f;      //5
    mHeight = 5.0f;     //5

    //texture
    ngl::Texture tex(_texture);
    m_textureID=tex.setTextureGL();
    BoundingBox bb;
        bb.m_minx = bb.m_miny = bb.m_minz = -10.0;
        bb.m_maxx = bb.m_maxy = bb.m_maxz = 10.0;
        collisionTree = new ParticleOctree (4, bb); // control the height of the octree
        m_applyOctree = false;
    m_windMag = 0.0;
}
//----------------------------------------------------------------------------------------------------------------------
Cloth::~Cloth()
{
    //delete m_clothSpring;
}
//----------------------------------------------------------------------------------------------------------------------
void Cloth::createCloth(double _n, std::string _texture)
{
    ngl::Texture tex(_texture);
    m_textureID=tex.setTextureGL();
    m_horiParticles = _n;
    m_vertParticles = _n;
    int id = 0;
    m_massVert.resize(m_horiParticles * m_vertParticles);
    for(int x =0;x<m_horiParticles;++x)
    {
        for(int y=0;y<m_vertParticles;++y)
        {
            ngl::Vec3 pos;
            pos.set(mWidth *(x/(float)m_horiParticles)*2,2,mHeight*(y/(float)m_vertParticles)*2);
            ngl::Vec3 vel;
            vel.set(0.0f,0.0f,0.0f);
            m_massVert.at(y*m_horiParticles+x)=new ClothParticle(pos,vel,1.0f,false);
            if(m_fix == true)
            {
                if(y==0)
                {
    //                if ((x == 0) || (x == 4) || (x == 8) || (x== m_horiParticles-9) || (x== m_horiParticles-5) || (x== m_horiParticles-1))
                      if ((x == 0) || (x == 1) || (x == 4) || (x == 5) || (x == 8) || (x == 9) ||
                          (x== m_horiParticles-10) || (x== m_horiParticles-9) || (x== m_horiParticles-6) || (x== m_horiParticles-5) || (x== m_horiParticles-2) || (x== m_horiParticles-1))

                    {
                        m_massVert.at(y*m_horiParticles+x)->m_fixed = true;                       
                    }
                }
            }
        }
    }


    //connections
    for(int x =0;x<m_horiParticles;++x)
    {
        for(int y=0;y<m_vertParticles;++y)
        {
            //structural
            if (x<m_horiParticles-1)
            {
                createSpring(id++,m_massVert[y*m_horiParticles+x],m_massVert[y*m_horiParticles+x +1],0);
            }
            if (y<m_vertParticles-1)
            {
                createSpring(id++,m_massVert[y*m_horiParticles+x],m_massVert[y*m_horiParticles+x +m_horiParticles],0);
            }

            //shear
            if (x<m_horiParticles-1 && y<m_vertParticles-1)
            {
                createSpring(id++,m_massVert[y*m_horiParticles+x],m_massVert[y*m_horiParticles+x +m_horiParticles+1],1);
                createSpring(id++,m_massVert[y*m_horiParticles+x+1],m_massVert[y*m_horiParticles+x +m_horiParticles],1);
            }

            //bending
            if (x<m_horiParticles-2)
            { createSpring(id++,m_massVert[y*m_horiParticles+x],m_massVert[y*m_horiParticles+x+2],2); }
            if (y<m_horiParticles-2)
            { createSpring(id++,m_massVert[y*m_horiParticles+x],m_massVert[y*m_horiParticles+x +m_horiParticles +m_horiParticles],2); }
            if (x<m_horiParticles-2 && y<m_vertParticles-2)
            {
                createSpring(id++,m_massVert[y*m_horiParticles+x],m_massVert[y*m_horiParticles+x +m_horiParticles +m_horiParticles+2],2);
                createSpring(id++,m_massVert[y*m_horiParticles+x+2],m_massVert[y*m_horiParticles+x +m_horiParticles +m_horiParticles],2);
            }
        }
    }
 }
//----------------------------------------------------------------------------------------------------------------------
void Cloth::drawTriCloth(
                        const std::string &_shaderName,
                      ngl::TransformStack &_transformStack,
                        ngl::Camera *_cam
                        )
{
    // texture co-ords start at 0,0
    // texture steps
    ngl::Real du=0.9/m_horiParticles;
    ngl::Real dv=0.9/m_vertParticles;

    ngl::Real u=0.0;
    ngl::Real v=0.0;

    // a std::vector to store our verts
    massPoints vert;
    for(int x =0;x<m_horiParticles-1;++x)
    {
        for(int y=0;y<m_vertParticles-1;++y)
        {
          /* tri 1
              // counter clock wise
              3
              | \
              |  \
              |   \
              1____2
              */
              // the normals are always the same so set them for d first

              ngl::Vec3 _vN =calcNormal(m_massVert[y*m_horiParticles+x]->getPosition(),m_massVert[y*m_horiParticles+x + 1]->getPosition(),m_massVert[y*m_horiParticles+x +m_horiParticles]->getPosition());
              vert.nx=_vN.m_x;
              vert.ny=_vN.m_y;
              vert.nz=_vN.m_z;

              vert.u=u; vert.v=v+dv;

              vert.x=m_massVert[y*m_horiParticles+x]->getPosition().m_x;
              vert.y=m_massVert[y*m_horiParticles+x]->getPosition().m_y;
              vert.z=m_massVert[y*m_horiParticles+x]->getPosition().m_z;
              m_triArray.push_back(m_massVert[y*m_horiParticles+x]);

              m_vertData.push_back(vert);
              // 2
              vert.u=u+du; vert.v=v+dv;

              vert.x=m_massVert[y*m_horiParticles+x + 1]->getPosition().m_x;
              vert.y=m_massVert[y*m_horiParticles+x + 1]->getPosition().m_y;
              vert.z=m_massVert[y*m_horiParticles+x + 1]->getPosition().m_z;
              m_triArray.push_back(m_massVert[y*m_horiParticles+x + 1]);
              m_vertData.push_back(vert);
              // 3
              vert.u=u; vert.v=v;

              vert.x=m_massVert[y*m_horiParticles+x +m_horiParticles]->getPosition().m_x;
              vert.y=m_massVert[y*m_horiParticles+x +m_horiParticles]->getPosition().m_y;
              vert.z=m_massVert[y*m_horiParticles+x +m_horiParticles]->getPosition().m_z;
              m_triArray.push_back(m_massVert[y*m_horiParticles+x +m_horiParticles]);
              m_vertData.push_back(vert);


          /* tri 2 w,0,d
        // counter clock wise
         3_____2
          \    |
            \  |
             \ |
              \|
              1

              */
              vert.nx=_vN.m_x;
              vert.ny=_vN.m_y;
              vert.nz=_vN.m_z;
              vert.u=u+du; vert.v=v+dv;

              vert.x=m_massVert[y*m_horiParticles+x + 1]->getPosition().m_x;
              vert.y=m_massVert[y*m_horiParticles+x + 1]->getPosition().m_y;
              vert.z=m_massVert[y*m_horiParticles+x + 1]->getPosition().m_z;
              m_triArray.push_back(m_massVert[y*m_horiParticles+x + 1]);
              m_vertData.push_back(vert);
              // 2
              vert.u=u+du; vert.v=v;

              vert.x=m_massVert[y*m_horiParticles+x + m_horiParticles +1]->getPosition().m_x;
              vert.y=m_massVert[y*m_horiParticles+x + m_horiParticles +1]->getPosition().m_y;
              vert.z=m_massVert[y*m_horiParticles+x + m_horiParticles +1]->getPosition().m_z;
              m_triArray.push_back(m_massVert[y*m_horiParticles+x + m_horiParticles +1]);
              m_vertData.push_back(vert);
              // 3
              vert.u=u; vert.v=v;

              vert.x=m_massVert[y*m_horiParticles+x + m_horiParticles]->getPosition().m_x;
              vert.y=m_massVert[y*m_horiParticles+x + m_horiParticles]->getPosition().m_y;
              vert.z=m_massVert[y*m_horiParticles+x + m_horiParticles]->getPosition().m_z;
              m_triArray.push_back(m_massVert[y*m_horiParticles+x + m_horiParticles]);
              m_vertData.push_back(vert);
            u+=du;
          } // end w loop
          u=0.0;
          v+=du;
      } // end d loop
      // now create the VBO

        ngl::ShaderLib *shader=ngl::ShaderLib::instance();
        glEnable (GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        shader->use(_shaderName);

        ngl::VertexArrayObject *vao_tri = ngl::VertexArrayObject::createVOA(GL_TRIANGLES);
        vao_tri->bind();
        vao_tri->setData(m_vertData.size()*sizeof(massPoints),m_vertData[0].u);
        vao_tri->setVertexAttributePointer(0,3,GL_FLOAT,sizeof(massPoints),5);
        vao_tri->setVertexAttributePointer(1,2,GL_FLOAT,sizeof(massPoints),0);
        vao_tri->setVertexAttributePointer(2,3,GL_FLOAT,sizeof(massPoints),2);
        vao_tri->setNumIndices(m_vertData.size());

        glBindTexture(GL_TEXTURE_2D,m_textureID);
        loadMatricesToShader(_transformStack,_cam);
        vao_tri->draw();
        vao_tri->unbind();
    glDisable(GL_BLEND);
}
//----------------------------------------------------------------------------------------------------------------------
void Cloth::drawSpring(ngl::Vec3 _vertA, ngl::Vec3 _vertB,int _sptype)
{
    std::vector <ngl::Vec3> points(2);
    points[0] = _vertA;
    points[1] = _vertB;

    ngl::VertexArrayObject *vaoSpring = ngl::VertexArrayObject::createVOA(GL_LINES);
    vaoSpring->bind();
    vaoSpring->setData(2*sizeof(ngl::Vec3),points[0].m_x);
    vaoSpring->setNumIndices(2);
    vaoSpring->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
    vaoSpring->draw();
    vaoSpring->unbind();
    delete vaoSpring;
}
//----------------------------------------------------------------------------------------------------------------------
void Cloth::drawSpring(ClothSpring _cs)
{
    std::vector <ngl::Vec3> points(2);
    points[0] = _cs.getPositionA();
    points[1] = _cs.getPositionB();

    ngl::VertexArrayObject *vaoSpring = ngl::VertexArrayObject::createVOA(GL_LINES);
    vaoSpring->bind();
    vaoSpring->setData(2*sizeof(ngl::Vec3),points[0].m_x);
    vaoSpring->setNumIndices(2);
    vaoSpring->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
    vaoSpring->draw();
    vaoSpring->unbind();
    delete vaoSpring;
}
//----------------------------------------------------------------------------------------------------------------------
void Cloth::createSpring(int _id, ClothParticle* _vertA, ClothParticle* _vertB,int _sptype)
{
     ClothSpring cs(_id, _vertA, _vertB, 300.0f, 3.0f,_sptype); //40 , 1.0  150,2 (300-3 stable like a light fab)
     m_clothSpringArr.push_back(cs);
}
//----------------------------------------------------------------------------------------------------------------------
void Cloth::updateCloth()
{
    // 1- spring forces
    for(std::vector<ClothSpring>::size_type i=0;i<m_clothSpringArr.size();++i)
    {
        //springforces- cal forces exerted by spring on both the particles
        //store tht in the force variable of particles
        m_clothSpringArr[i].m_posA->m_force+=(m_clothSpringArr[i].calSpringForces()*-1.0);
        m_clothSpringArr[i].m_posB->m_force+=m_clothSpringArr[i].calSpringForces();
    }

    // 2 - gravity and update using RK4
    for(std::vector<ngl::Vec3>::size_type i=0;i<m_massVert.size();++i)
    {
        m_massVert[i]->applyGravity();
        m_massVert[i]->update();
        m_massVert[i]->m_force.set(0.0f,0.0f,0.0f);
    }

    //3 - self collisions
    if(m_applyOctree == true)
    {
        collisionWithBalls();
    }
    //clear the verdata for drawing
    m_vertData.clear();
}
//----------------------------------------------------------------------------------------------------------------------
void Cloth::loadMatricesToShader(
                                    ngl::TransformStack &_tx,
                                    ngl::Camera *_cam
                                  ) const
{
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();

    ngl::Mat4 MV;
    ngl::Mat4 MVP;
    MV=_tx.getCurrAndGlobal().getMatrix()*_cam->getViewMatrix() ;
    MVP=MV*_cam->getProjectionMatrix();
    shader->setShaderParamFromMat4("MVP",MVP);
}
//----------------------------------------------------------------------------------------------------------------------
void Cloth::collisionWithBalls()
{
    collisionTree->clearTree();

//    for(std::list<ClothParticle *>::iterator itr = m_massVert.begin(); itr!= m_massVert.end(); ++itr)
//    {
        for(std::vector<ClothParticle *>::size_type i=0;i<m_massVert.size()-1;++i)
        {

            collisionTree->addObject(m_massVert[i]);
    }
    collisionTree->checkCollision();
}
//----------------------------------------------------------------------------------------------------------------------
void Cloth::applyWindForce()
{
    //6- wind forces
     for(std::vector<ngl::Vec3>::size_type i=0;i<m_triArray.size();i+=3)
     {
         ngl::Vec3 nor,d,direction;
         ngl::Vec3 p1 = m_triArray[i]->getPosition();
         ngl::Vec3 p2 = m_triArray[i+1]->getPosition();
         ngl::Vec3 p3 = m_triArray[i+2]->getPosition();
        // std::cout<<i<<": p1= "<<p1<<"\t p2="<<p2<<"\t p3 ="<<p3<<std::endl;

         nor = calcNormal(p1,p2,p3);
         //nor.normalize();
         //d.set((nor.normalize();));

         ngl::Vec3 wind;
         wind.set(0.001,0,-0.001);

         wind.m_x += m_windMag;
         wind.m_z += m_windMag;
         direction.set(wind);

         ngl::Vec3 f = nor*direction;
         m_triArray[i]->m_force+=f;
         m_triArray[i+1]->m_force+=f;
         m_triArray[i+2]->m_force+=f;
     }
}

//----------------------------------------------------------------------------------------------------------------------

//    for(std::vector<ngl::Vec3>::size_type i=0;i<m_massVert.size();++i)
//    {
//        //cloth particle position
//        ngl::Vec3 cpPos = m_massVert[i]->m_pos;
//        //cloth particle velocity
//        ngl::Vec3 cpVel = m_massVert[i]->m_vel;

//        relPos = (cpPos - sPos);
//        //distance
//        len = abs(relPos.length());
//        if(len <= minDist)
//        {
//            //collision
//            relPos.normalize();
//            m_massVert[i]->m_pos += relPos*(minDist-len);
//            m_massVert[i]->m_vel = -cpVel.dot(relPos)*relPos+(cpVel-cpVel.dot(relPos)*relPos);
//            //m_cloth->m_massVert[i]->m_pos.set(2,2,2);
//        }
//     }

    //updating the position acc to change in velocity
//    for(std::vector<ngl::Vec3>::size_type i=0;i<m_massVert.size();++i)
//    {
//        //cloth particle position
//        ngl::Vec3 cpPos = m_massVert[i]->m_pos;
//        //cloth particle velocity
//        ngl::Vec3 cpVel = m_massVert[i]->m_vel;

//        relPos = (cpPos - sPos);
//        //distance
//        len = relPos.length();
//        if(len <= minDist)
//        {
//            //collision
//            relPos.normalize();
//            m_massVert[i]->m_pos += relPos*(minDist-len);
//            //m_massVert[i]->m_vel = -cpVel.dot(relPos)*relPos+(cpVel-cpVel.dot(relPos)*relPos);
//            //m_cloth->m_massVert[i]->m_pos.set(2,2,2);
//        }
//        //p=p+v
//       if(m_massVert[i]->m_fixed == false)
//       {
//           m_massVert[i]->m_pos+=m_gravity;
//           m_massVert[i]->m_pos+=m_massVert[i]->m_vel;
//           //std::cout<<"\n m_massVert["<<i<<"]->m_pos = "<<m_massVert[i]->m_pos<<" m_massVert[i]->m_vel = "<<m_massVert[i]->m_vel;
//           //std::cout<<"\n m_massVert[10]->m_pos = "<<m_massVert[10]->m_pos<<" m_massVert[10]->m_vel = "<<m_massVert[10]->m_vel;
//           m_massVert[i]->m_vel.set(0.0f,0.0f,0.0f);
//       }
//    }


    //std::cout<<"\n dont with one loop call;";




/*void Cloth::drawGrid()
{
    //cloth spring instance
    //m_clothSpring = new ClothSpring();


    std::vector <massPoints> m_data;
    massPoints vert;
    //resting length of the spring will the step size for our cloth grid
    //float step = m_clothSpring->getRestLength();
    float step = 10.0f;
    float _length = 10.0f;
    float _width  = 10.0f;
    std::cout<<"\n in createCloth(): springs rest length = "<<step;

    //length - step size for each grid value
    float lstep  = _length/step;
    float lstep2 = _length/2.0f;
    float v1=-lstep2;

    //width - step size
    float wstep  = _width/step;
    float wstep2 = _width/2.0f;
    float v2=-wstep2;

    for(int i=0;i<=step;++i)
    {
        //changing the y value
        //vertex - 1
        vert.x=-lstep2; //x
        vert.y= v1; //y
        vert.z= 0.0f;   //z
        m_data.push_back(vert);
        //vertex - 2
        vert.x=lstep2;  //x
        vert.y=v1;  //y
        m_data.push_back(vert);

        //changing the x value
        //vertex - 1
        vert.x=v2; //x
        vert.y= wstep2; //y
        m_data.push_back(vert);
        //vertex - 2
        vert.x=v2; //x
        vert.y=-wstep2; //y
        m_data.push_back(vert);

        //change in step value
        v1+=lstep;
        v2+=wstep;
        //std::cout<<"\nvert.x = "<<vert.x;
    }

    ngl::VertexArrayObject *vao_cloth = ngl::VertexArrayObject::createVOA(GL_LINES);
    vao_cloth->bind();
    vao_cloth->setData(m_data.size()*sizeof(massPoints),m_data[0].u);
    vao_cloth->setVertexAttributePointer(0,3,GL_FLOAT,sizeof(massPoints),5);
    vao_cloth->setVertexAttributePointer(1,2,GL_FLOAT,sizeof(massPoints),0);
    vao_cloth->setVertexAttributePointer(2,3,GL_FLOAT,sizeof(massPoints),2);
    vao_cloth->setNumIndices(m_data.size());
    vao_cloth->draw();
    vao_cloth->unbind();
}
*/
