#include "include/Props.h"
//----------------------------------------------------------------------------------------------------------------------
/// @file Props.cpp
/// @brief his class contains all meshes required in the simulation
/// using this class, we load meshes once which can be attached to objects in simulation
//----------------------------------------------------------------------------------------------------------------------
Props::~Props()
{
    std::map <std::string,ngl::Obj *>::iterator pbegin=m_props.begin();
    std::map <std::string,ngl::Obj *>::iterator pend=m_props.end();
    while(pbegin != pend)
    {
      delete pbegin->second;
      ++pbegin;
    }
}
//----------------------------------------------------------------------------------------------------------------------
void Props::addProp(
                        const std::string &_name,
                        const std::string &_mesh
                    )
{
    ngl::Obj *mesh = new ngl::Obj(_mesh);
    mesh->createVAO();
    mesh->calcBoundingSphere();
    m_props[_name]=mesh;
}
//----------------------------------------------------------------------------------------------------------------------
void Props::addProp(const std::string &_name,
                    const std::string &_mesh,
                    const std::string &_texture)
                  //  const ngl::Vec3 &_s)
{
    ngl::Obj *mesh = new ngl::Obj(_mesh,_texture);
    mesh->createVAO();
    mesh->calcBoundingSphere();
    m_props[_name]=mesh;
}
//----------------------------------------------------------------------------------------------------------------------
ngl::Obj * Props::getProp(std::string _name)
{
    std::map <std::string,ngl::Obj *>::const_iterator pbegin=m_props.begin();
    std::map <std::string,ngl::Obj *>::const_iterator prop=m_props.find(_name);

    if(pbegin!=m_props.end() && prop !=m_props.end())
    {
      return prop->second;
    }
}
//----------------------------------------------------------------------------------------------------------------------
void Props::draw(std::string _name) const
{
    std::map <std::string,ngl::Obj *>::const_iterator pbegin=m_props.begin();
    std::map <std::string,ngl::Obj *>::const_iterator prop=m_props.find(_name);

    if(pbegin!=m_props.end() && prop !=m_props.end())
    {
      prop->second->draw();
    }
}
//----------------------------------------------------------------------------------------------------------------------
