#ifndef CLOTH_H
#define CLOTH_H

/// @file   Cloth.h
/// @brief  this class creates the cloth based on the mass-spring model
/// @author Navpreet Kaur Pawar
/// @version 1.0
/// @date   09/05/13 Updated to NCCA Coding standard
/// Revision History : \n
/// Initial Version : 09/05/13
/// @class Cloth
/// @brief this class creates the cloth according to mass-spring model
///         it updates the cloth with respect to the internal spring forces
///         and the external force acting on the cloth
///         the mass particle(connecting the springs) states are updated using the RK4 integrator
//----------------------------------------------------------------------------------------------------------------------
#include "ClothSpring.h"
#include "ngl/Texture.h"
#include "ngl/VAOPrimitives.h"
#include "ngl/VertexArrayObject.h"
#include "ngl/TransformStack.h"
#include "ngl/Camera.h"
#include "ngl/ShaderLib.h"
#include "AbstractOctree.h"
#include "ParticleOctree.h"
//----------------------------------------------------------------------------------------------------------------------
struct massPoints
{
    GLfloat u;
    GLfloat v;
    GLfloat nx;
    GLfloat ny;
    GLfloat nz;
    GLfloat x;
    GLfloat y;
    GLfloat z;
};
//----------------------------------------------------------------------------------------------------------------------
struct tri
{
    ngl::Vec3 t1;
    ngl::Vec3 t2;
    ngl::Vec3 t3;
};
//----------------------------------------------------------------------------------------------------------------------
class Cloth
{
public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Constructor
    //----------------------------------------------------------------------------------------------------------------------
    Cloth();

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Constructor
    ///  @param[in] _texture    cloth texture
    //----------------------------------------------------------------------------------------------------------------------
    Cloth(
          std::string _texture
         );

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief basket destructor
    //----------------------------------------------------------------------------------------------------------------------
    ~Cloth();

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief number of horizontal mass particles
    //----------------------------------------------------------------------------------------------------------------------
    int m_horiParticles;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief number of vertical mass particles
    //----------------------------------------------------------------------------------------------------------------------
    int m_vertParticles;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief cloth width
    //----------------------------------------------------------------------------------------------------------------------
    float mWidth;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief cloth height
    //----------------------------------------------------------------------------------------------------------------------
    float mHeight;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief vector array for the cloth's mass particles
    //----------------------------------------------------------------------------------------------------------------------
    std::vector <ClothParticle*> m_massVert;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief vector array for the cloth's mass particles stored as a traingular mesh
    //----------------------------------------------------------------------------------------------------------------------
    std::vector <ClothParticle*> m_triArray;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief bool to keep cloth fixed for simulations
    //----------------------------------------------------------------------------------------------------------------------
    bool m_fix;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief bool to give user the choie of applying octree
    //----------------------------------------------------------------------------------------------------------------------
    bool m_applyOctree;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief wind magnitude that can be added via user interface
    //----------------------------------------------------------------------------------------------------------------------
    double m_windMag;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief method to create the cloth grid and its spring connections
    ///  @param[in]     _n     number of horizontal and vertical particles
    //----------------------------------------------------------------------------------------------------------------------
    void createCloth(double _n, std::string _texture);

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Draw method for drawing the cloth
    ///  @param[in]     _shaderName     the shader
    ///  @param[in]     _transformStack the transformation stack
    ///  @param[in]     _cam            the camera
    //----------------------------------------------------------------------------------------------------------------------
    void drawTriCloth( const std::string &_shaderName,
                       ngl::TransformStack &_transformStack,
                       ngl::Camera *_cam
                       );

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief method to update the cloth- forces(internal and external)
    ///        and update the mass particles according to RK4 integrator state values
    //----------------------------------------------------------------------------------------------------------------------
    void updateCloth();
    void applyWindForce();
    //----------------------------------------------------------------------------------------------------------------------
    void createSpring(
                    int _id,
                    ClothParticle *_vertA,
                    ClothParticle *_vertB,
                    int _sptype
                    );
    void drawSpring(
                    ngl::Vec3 _vertA,
                    ngl::Vec3 _vertB,
                    int _sptype
                    );
    void drawSpring(ClothSpring _cs);

private:

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief forces
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 m_gravity;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief mass point vertices data - for drawing the triangulated cloth mesh
    //----------------------------------------------------------------------------------------------------------------------
    std::vector <massPoints> m_vertData;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief vector containg all the springs in the cloth
    //----------------------------------------------------------------------------------------------------------------------
    std::vector <ClothSpring> m_clothSpringArr;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief vector containg all the springs in the cloth
    //----------------------------------------------------------------------------------------------------------------------
    ClothParticle m_massVertex;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief cloth texture id
    //----------------------------------------------------------------------------------------------------------------------
    GLuint m_textureID;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief octree for collision detection
    //----------------------------------------------------------------------------------------------------------------------
    ParticleOctree  *collisionTree;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief method to check for self collision detection between cloth mass particles to prevent penetration
    //----------------------------------------------------------------------------------------------------------------------
    void collisionWithBalls();

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief load all the transform values to the shader
    /// @param[in]  _tx     the current transform to load
    ///  @param[in] _cam    the camera
    //----------------------------------------------------------------------------------------------------------------------
    void loadMatricesToShader(
                                  ngl::TransformStack &_tx,
                                  ngl::Camera *_cam
                                ) const;
    //----------------------------------------------------------------------------------------------------------------------
};

#endif // CLOTH_H