#include "GLWindow.h"
#include <iostream>
#include <ngl/Light.h>
#include <ngl/Mat4.h>
#include <ngl/Transformation.h>
#include <ngl/Material.h>
#include <ngl/NGLInit.h>
#include <ngl/Obj.h>
#include <ngl/Random.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <boost/foreach.hpp>

//----------------------------------------------------------------------------------------------------------------------
/// @file GLWindow.cpp
/// @brief class for main drawing operations and it integrates all the member classes for the simulation
//----------------------------------------------------------------------------------------------------------------------
GLWindow::GLWindow(
        int _timer,
                                    QWidget *_parent
                  ) : QGLWidget( new CreateCoreGLContext(QGLFormat::defaultFormat()), _parent )
{

     // set this widget to have the initial keyboard focus
     setFocus();
     // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
     this->resize(_parent->size());
     // Now set the initial GLWindow attributes to default values
     // Roate is false
     m_rotate=false;
     // mouse rotation values set to 0
     m_spinXFace=0;
     m_spinYFace=0;
     m_timerValue=_timer;
     startSimTimer();
     m_sim=false;
     m_sphereCollision=false;
     m_tableCollision = false;
     m_draw = false;
     m_reset=true;
     m_cp = 10.0f;
     clothtex = "texture/table.png";
}

GLWindow::~GLWindow()
{
    ngl::NGLInit *init = ngl::NGLInit::instance();
    init->NGLQuit();
}
//----------------------------------------------------------------------------------------------------------------------
// This virtual function is called once before the first call to paintGL() or resizeGL(),
//and then once whenever the widget has been assigned a new QGLContext.
// This function should set up any required OpenGL context rendering flags, defining display lists, etc.
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::initializeGL()
{

glClearColor(0.4f, 0.4f, 0.4f, 1.0f);			   // Grey Background
// enable depth testing for drawing
 glEnable(GL_DEPTH_TEST);
 // Now we will create a basic Camera from the graphics library
 // This is a static camera so it only needs to be set once
 // First create Values for the camera position
 //ngl::Vec3 from(0,0,20);
 ngl::Vec3 from(-10,5,30);
 ngl::Vec3 to(0,0,0);
 ngl::Vec3 up(0,1,0);
 ngl::NGLInit *Init = ngl::NGLInit::instance();
 #ifdef WIN32
	glewInit();
 #endif
 Init->initGlew();
 m_cam= new ngl::Camera(from,to,up,ngl::PERSPECTIVE);
 // set the shape using FOV 45 Aspect Ratio based on Width and Height
 // The final two are near and far clipping planes of 0.5 and 10
 m_cam->setShape(45,(float)720.0/576.0,0.5,150,ngl::PERSPECTIVE);
 // now to load the shader and set the values
 // grab an instance of shader manager
 ngl::ShaderLib *shader=ngl::ShaderLib::instance();
 // we are creating a shader called Phong
 shader->createShaderProgram("Phong");
 // now we are going to create empty shaders for Frag and Vert
 shader->attachShader("PhongVertex",ngl::VERTEX);
 shader->attachShader("PhongFragment",ngl::FRAGMENT);
 // attach the source
 shader->loadShaderSource("PhongVertex","shaders/Phong.vs");
 shader->loadShaderSource("PhongFragment","shaders/Phong.fs");
 // compile the shaders
 shader->compileShader("PhongVertex");
 shader->compileShader("PhongFragment");
 // add them to the program
 shader->attachShaderToProgram("Phong","PhongVertex");
 shader->attachShaderToProgram("Phong","PhongFragment");
 // now bind the shader attributes for most NGL primitives we use the following
 // layout attribute 0 is the vertex data (x,y,z)
 shader->bindAttribute("Phong",0,"inVert");
 // attribute 1 is the UV data u,v (if present)
 shader->bindAttribute("Phong",1,"inUV");
 // attribute 2 are the normals x,y,z
 shader->bindAttribute("Phong",2,"inNormal");

 // now we have associated this data we can link the shader
 shader->linkProgramObject("Phong");
 // and make it active ready to load values
 (*shader)["Phong"]->use();
 shader->setShaderParam1i("Normalize",1);

 //------
 // texture shader
 ngl::ShaderLib *shader1=ngl::ShaderLib::instance();
 // we are creating a shader called Texture
 shader1->createShaderProgram("Texture");
 // now we are going to create empty shaders for Frag and Vert
 shader1->attachShader("TextureVertex",ngl::VERTEX);
 shader1->attachShader("TextureFragment",ngl::FRAGMENT);
 // attach the source
 shader1->loadShaderSource("TextureVertex","shaders/TextureVertex.glsl");
 shader1->loadShaderSource("TextureFragment","shaders/TextureFragment.glsl");
 // compile the shaders
 shader1->compileShader("TextureVertex");
 shader1->compileShader("TextureFragment");
 // add them to the program
 shader1->attachShaderToProgram("Texture","TextureVertex");
 shader1->attachShaderToProgram("Texture","TextureFragment");
 // now we have associated this data we can link the shader
 shader1->linkProgramObject("Texture");
 (*shader1)["Texture"]->use();
 //------
 // now pass the modelView and projection values to the shader
 // the shader will use the currently active material and light0 so set them
 ngl::Material m(ngl::SILVER);
 m.loadToShader("material");
 ngl::Light light(ngl::Vec3(4,4,0),ngl::Colour(1,1,1,1),ngl::Colour(1,1,1,1),ngl::POINTLIGHT);
 ngl::Light spot(ngl::Vec3(10,2,2),ngl::Colour(1,1,1,1),ngl::Colour(1,1,1,1),ngl::SPOTLIGHT);
 // now create our light this is done after the camera so we can pass the
 // transpose of the projection matrix to the light to do correct eye space
 // transformations
 ngl::Mat4 iv=m_cam->getViewMatrix();
 iv.transpose();
 light.setTransform(iv);
 light.setAttenuation(1,0,0);
 light.enable();
 // load these values to the shader as well
 light.loadToShader("light");

 //SPOTLIGHT
 spot.setTransform(iv);
 spot.setAttenuation(1,0,0);
 spot.enable();
 // load these values to the shader as well
 spot.loadToShader("light");

 shader->createShaderProgram("Colour");
 shader->attachShader("ColourVertex",ngl::VERTEX);
 shader->attachShader("ColourFragment",ngl::FRAGMENT);
 shader->loadShaderSource("ColourVertex","shaders/Colour.vs");
 shader->loadShaderSource("ColourFragment","shaders/Colour.fs");
 shader->compileShader("ColourVertex");
 shader->compileShader("ColourFragment");
 shader->attachShaderToProgram("Colour","ColourVertex");
 shader->attachShaderToProgram("Colour","ColourFragment");
 shader->bindAttribute("Colour",0,"inVert");
 shader->linkProgramObject("Colour");

 //m_cloth = new Cloth("texture/table.png");
 m_cloth = new Cloth(clothtex);

 m_prop.addProp("psphere","props/sphere_1.obj","texture/silk.png");

 m_prop.addProp("ptable","props/table_1.obj","texture/wood.jpg");


 //primitive instance for draw function of sphere
 ngl::VAOPrimitives *prim =  ngl::VAOPrimitives::instance();

   prim->createSphere("sphere",1.0,40);
   ngl::Vec3 n;
   n.set(0,-1,0);
   prim->createTrianglePlane("table",1,1,1,1,n);

}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::setClothPartNum(double _n)
{
    m_cp = _n;
    if(m_reset==true)
    {
        m_cloth->m_fix = true;
        m_cloth->createCloth(m_cp,clothtex);
        m_reset=false;

        emit clothcreated();
    }
}
//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget has been resized.
// The new size is passed in width and height.
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::resizeGL(
                        int _w,
                        int _h
                       )
{
  glViewport(0,0,_w,_h);
  m_cam->setShape(45,(float)_w/_h,0.5,150,ngl::PERSPECTIVE);
}

void GLWindow::loadMatricesToShader(
                                     ngl::TransformStack &_tx
                                   )
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  //(*shader)["Phong"]->use();

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;
  M=_tx.getCurrentTransform().getMatrix();
  MV=_tx.getCurrAndGlobal().getMatrix()*m_cam->getViewMatrix() ;
  MVP=MV*m_cam->getProjectionMatrix();
  normalMatrix=MV;
  normalMatrix.inverse();
  shader->setShaderParamFromMat4("MV",MV);
  shader->setShaderParamFromMat4("MVP",MVP);
  shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
  shader->setShaderParamFromMat4("M",M);
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::loadMatricesToColourShader(
                                           ngl::TransformStack &_tx
                                         )
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["Colour"]->use();
  ngl::Mat4 MV;
  ngl::Mat4 MVP;

  MV=_tx.getCurrAndGlobal().getMatrix()*m_cam->getViewMatrix() ;
  MVP=MV*m_cam->getProjectionMatrix() ;
  shader->setShaderParamFromMat4("MVP",MVP);

}
//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget needs to be painted.
// this is our main drawing routine
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::paintGL()
{

  // grab an instance of the shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  // clear the screen and depth buffer

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // Rotation based on the mouse position for our global
  // transform
  ngl::Transformation trans;
  trans.setRotation(m_spinXFace,m_spinYFace,0);
  // Rotation based on the mouse position for our global
  // transform
  ngl::Mat4 rotX;
  ngl::Mat4 rotY;
  // create the rotation matrices
  rotX.rotateX(m_spinXFace);
  rotY.rotateY(m_spinYFace);
  // multiply the rotations
  ngl::Mat4 final=rotY*rotX;
  // add the translations
  final.m_m[3][0] = m_modelPos.m_x;
  final.m_m[3][1] = m_modelPos.m_y;
  final.m_m[3][2] = m_modelPos.m_z;
  // set this in the TX stack
  trans.setMatrix(final);
  // set this in the TX stack
  m_transformStack.setGlobal(trans);
	// draw spring lines
  shader->use("Phong");


if (m_draw == true || m_sim==true)
{
    //cloth
    m_transformStack.pushTransform();
    {
        ngl::ShaderLib *shader=ngl::ShaderLib::instance();
        (*shader)["Texture"]->use();
        m_transformStack.setPosition(-5.0,0.0,0.0);
        loadMatricesToShader(m_transformStack);
        m_cloth->drawTriCloth("Texture",m_transformStack,m_cam);
    }
    m_transformStack.popTransform();

    //table
    if( m_tableCollision == true)
    {
        m_cloth->m_fix =false;
        m_transformStack.pushTransform();
        {
            ngl::ShaderLib *shader=ngl::ShaderLib::instance();
             (*shader)["Texture"]->use();
            m_transformStack.setPosition(0.0,-5.5,5.0);
            m_transformStack.setScale(5.2,6.0,5.2);
            glEnable (GL_BLEND);
            glBindTexture(GL_TEXTURE_2D,m_texIDTable);
            loadMatricesToShader(m_transformStack);
            m_prop.draw("ptable");
             glDisable(GL_BLEND);
         }
       m_transformStack.popTransform();
    }

    //sphere
    if( m_sphereCollision == true)
    {
        m_cloth->m_fix =true;
        float rad = 1.5;
        m_transformStack.pushTransform();
        {
            ngl::ShaderLib *shader=ngl::ShaderLib::instance();
             (*shader)["Texture"]->use();
            //m_transformStack.setPosition(2,-4,1);
            m_transformStack.setPosition(-3,-4,1);
            m_transformStack.setScale(rad,rad,rad);
            loadMatricesToShader(m_transformStack);
            m_prop.draw("psphere");
        }
        m_transformStack.popTransform();
    }
  }
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mouseMoveEvent (
                               QMouseEvent * _event
                              )
{
  // note the method buttons() is the button state when event was called
  // this is different from button() which is used to check which button was
  // pressed when the mousePress/Release event is generated
    if(m_rotate && _event->buttons() == Qt::LeftButton)
    {
      int diffx=_event->x()-m_origX;
      int diffy=_event->y()-m_origY;
      m_spinXFace += (float) 0.5f * diffy;
      m_spinYFace += (float) 0.5f * diffx;
      m_origX = _event->x();
      m_origY = _event->y();
      updateGL();

    }
    // right mouse translate code
    else if(m_translate && _event->buttons() == Qt::RightButton)
    {
      int diffX = (int)(_event->x() - m_origXPos);
      int diffY = (int)(_event->y() - m_origYPos);
      m_origXPos=_event->x();
      m_origYPos=_event->y();
      m_modelPos.m_x += INCREMENT * diffX;
      m_modelPos.m_y -= INCREMENT * diffY;
      updateGL();
    }
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mousePressEvent (
                                QMouseEvent * _event
                               )
{
  // this method is called when the mouse button is pressed in this case we
  // store the value where the maouse was clicked (x,y) and set the Rotate flag to true
    if(_event->button() == Qt::LeftButton)
    {
      m_origX = _event->x();
      m_origY = _event->y();
      m_rotate =true;
    }
    // right mouse translate mode
    else if(_event->button() == Qt::RightButton)
    {
      m_origXPos = _event->x();
      m_origYPos = _event->y();
      m_translate=true;
    }

}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mouseReleaseEvent (
                                  QMouseEvent * _event
                                 )
{
  // this event is called when the mouse button is released
  // we then set Rotate to false
    if (_event->button() == Qt::LeftButton)
    {
      m_rotate=false;
    }
    // right mouse translate mode
    if (_event->button() == Qt::RightButton)
    {
      m_translate=false;
    }

}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::wheelEvent(QWheelEvent *_event)
{

  // check the diff of the wheel position (0 means no change)
  if(_event->delta() > 0)
  {
    m_modelPos.m_z+=ZOOM;
  }
  else if(_event->delta() <0 )
  {
    m_modelPos.m_z-=ZOOM;
  }
  updateGL();
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::timerEvent(
                        QTimerEvent *_event
                        )
{

    if(m_sim == true || m_draw == true)
    {

        //m_cloth->updateCloth();
        m_cloth->updateCloth();

        if(m_sphereCollision==true)
        {   clothSphereCollision(); m_reset=true;
            m_sim = false;
        }

        if(m_tableCollision==true)
            {
                emit resetCloth();
                m_reset=true;
                for(std::vector<ngl::Vec3>::size_type i=0;i<m_cloth->m_massVert.size();++i)
                {
                    m_cloth->m_massVert[i]->setNotFixed();
                    dropOnTable();
                }
                m_sim=false;

            }

        if(m_applyWind==true && m_cloth->m_fix == true)
        {
            emit resetCloth();
           m_cloth->applyWindForce();
           m_sim=false;
           updateGL();
        }

        updateGL();
    }

}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::startSimTimer()
{

m_timer=startTimer(m_timerValue);
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::clothSphereCollision()
{
    for(std::vector<ngl::Vec3>::size_type i=0;i<m_cloth->m_massVert.size();++i)
       {

           //check for collisions
            ngl::Vec3 relPos;
            GLfloat len,dist,minDist;
            //sphere rad and pos
            ngl::Vec3 sPos;
            sPos.set(2,-4,1);
            minDist =1.65; //
            //cloth particle position
            ngl::Vec3 cpPos = m_cloth->m_massVert[i]->getPosition();
            //cloth particle velocity
            ngl::Vec3 cpVel = m_cloth->m_massVert[i]->getVelocity();

            relPos = (cpPos - sPos);
            //distance
            len = relPos.length();
            if(len<=minDist)
            {
                //collision response
                relPos.normalize();
                ngl::Vec3 pos = m_cloth->m_massVert[i]->getPosition();
                pos += (relPos*(minDist - len));
                m_cloth->m_massVert[i]->setPosition(pos);
                ngl::Vec3 vel = -cpVel.dot(relPos)*relPos+(cpVel-cpVel.dot(relPos)*relPos);
                m_cloth->m_massVert[i]->setVelocity(vel);
            }
         }
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::dropOnTable()
{
    twidth=6.0f;tdepth=6.0f;
    GLfloat d;
    ngl::Vec3 particlePos;
    float ext[4];
    ext[0]=ext[1]=twidth/2.0f;
    ext[2]=ext[3]=tdepth/2.0f;
    tcenter.set(5,-5,5);
    t1.set(tcenter.m_x-ext[0],tcenter.m_y,tcenter.m_z-ext[2]);
    t2.set(tcenter.m_x+ext[0],tcenter.m_y,tcenter.m_z-ext[2]);
    t3.set(tcenter.m_x-ext[0],tcenter.m_y,tcenter.m_z+ext[2]);
    t4.set(tcenter.m_x+ext[0],tcenter.m_y,tcenter.m_z+ext[2]);
    tnormal=calcNormal(t4,t3,t2);
    for(std::vector<ngl::Vec3>::size_type i=0;i<m_cloth->m_massVert.size();++i)
    {
        particlePos = m_cloth->m_massVert[i]->getPosition();
        d = tnormal.dot(particlePos);
        if(d<=tcenter.m_y)
        {
            if((particlePos.m_x>=t1.m_x && particlePos.m_x<=t2.m_x &&
                particlePos.m_z>=t1.m_z && particlePos.m_z<=t4.m_z) &&
                (particlePos.m_y<=-5) &&(particlePos.m_y>=-5.1)  )
                {
                    m_cloth->m_massVert[i]->setFixed();
                }
         }
     }
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::setSim()
{
    m_sim = true;
    setClothPartNum(m_cp);
    updateGL();
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::setSphereCollision(bool _mode)
{
    emit resetCloth();
    m_sphereCollision = _mode;
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::setTableCollision(bool _mode)
{
    m_reset=true;
    emit resetCloth();
    m_tableCollision = _mode;
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::setApplyWind(bool _mode)
{
    m_cloth->m_fix = true;
    emit resetCloth();
    m_applyWind = _mode;
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::setDrawFlag()
{
    m_draw = true;
    updateGL();
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::setClothWidth(double _w)
{
    m_cloth->mWidth = _w;
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::setClothDepth(double _h)
{
    m_cloth->mHeight = _h;
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::applyOctree(bool _mode)
{
    m_cloth->m_applyOctree = _mode;
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::addWindMag(double _mag)
{
    m_cloth->m_windMag = _mag/1000;
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::setTexFileName(std::string _filename)
{
    clothtex = _filename;
    updateGL();
}
//----------------------------------------------------------------------------------------------------------------------
