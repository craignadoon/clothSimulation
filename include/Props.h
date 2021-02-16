#ifndef PROPS_H
#define PROPS_H

/// @file   Props.h
/// @brief  this class imports all the meshes used in the simulation for collisions
/// @author Navpreet Kaur Pawar
/// @version 1.0
/// @date   09/05/13 Updated to NCCA Coding standard
/// Revision History : \n
/// Initial Version : 09/05/13
/// @class Cloth
/// @brief his class contains all meshes required in the simulation
/// using this class, we load meshes once which can be attached to objects in simulation
//----------------------------------------------------------------------------------------------------------------------

#include <string>
#include <map>
#include <ngl/Obj.h>
class Props
{
public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Constructor
    //----------------------------------------------------------------------------------------------------------------------
    Props() {;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief deconstructor
    //----------------------------------------------------------------------------------------------------------------------
    ~Props();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief method to add a mesh with no texture
    /// @param[in] _name    the name of the model to use for lookup
    /// @param[in] _mesh    the path / name of the mesh
    //----------------------------------------------------------------------------------------------------------------------
    void addProp(const std::string &_name,const std::string &_mesh);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief add a mesh with a texture
    /// @param[in] the name of the model we wish to use for lookup
    /// @param[in] the path / name of the mesh
    /// @param[in] the path / name of the texture
    //----------------------------------------------------------------------------------------------------------------------
    void addProp(const std::string &_name,
                 const std::string &_mesh,
                 const std::string &_texture);
                 //const ngl::Vec3 &_s);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief accesor to the model, incase the caller
    /// wishes to modify the mesh etc
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Obj *getProp(std::string _name);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief method to draw the mesh, all tx must be executed before
    /// the call to draw
    ///  @param[in] the name of the mesh to draw
    //----------------------------------------------------------------------------------------------------------------------
    void draw(std::string _name) const;
    //GLuint m_texID;
private:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a map to hold our meshes by name
    //----------------------------------------------------------------------------------------------------------------------
    std::map <std::string, ngl::Obj *> m_props;
    //Table, m_texIDSphere;
};

#endif // PROPS_H
