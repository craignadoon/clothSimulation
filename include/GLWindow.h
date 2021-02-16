#ifndef __GL_WINDOW_H__
#define __GL_WINDOW_H__


#include "ngl/Camera.h"
#include "ngl/Colour.h"
#include "ngl/TransformStack.h"
#include "Cloth.h"
#include "Props.h"
#include <math.h>

// must be included after our stuff becuase GLEW needs to be first
#include <QtOpenGL>

//----------------------------------------------------------------------------------------------------------------------
/// @file GLWindow.h
/// @brief a basic Qt GL window class where all the drawing and updating of objects takes place
/// @author Jonathan Macey
/// @version 1.0
/// @date 10/10/10
/// Revision History :
/// Initial Version 10/10/10 (Binary day ;-0 )
/// @class GLWindow
/// @brief our main glwindow widget for NGL applications all drawing elements are
///         put in this file
///         the cloth is created, drawn in this class. it is updated in the timer function
///         and checks for collisions between the cloth and the meshes
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for x/y translation with mouse movement
//----------------------------------------------------------------------------------------------------------------------
const static float INCREMENT=0.01;
//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for the wheel zoom
//----------------------------------------------------------------------------------------------------------------------
const static float ZOOM=1.0;
//----------------------------------------------------------------------------------------------------------------------

class GLWindow : public QGLWidget
{
Q_OBJECT        // must include this if you use Qt signals/slots
public :

	//----------------------------------------------------------------------------------------------------------------------
    /// @brief Constructor for GLWindow
	/// @param[in] _timer the time value for simulation updates
    /// @param [in] _parent the parent window to create the GL context in
	//----------------------------------------------------------------------------------------------------------------------
    GLWindow(
               int _timer,
               QWidget *_parent
            );
	//----------------------------------------------------------------------------------------------------------------------
	/// @brief dtor
	//----------------------------------------------------------------------------------------------------------------------
	~GLWindow();
	//----------------------------------------------------------------------------------------------------------------------
    /// @brief start the simulation timer
	//----------------------------------------------------------------------------------------------------------------------
	void startSimTimer();
	//----------------------------------------------------------------------------------------------------------------------
	/// @brief stop the simulation timer
	//----------------------------------------------------------------------------------------------------------------------
	void stopSimTimer();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief variable to recieve user input for number of cloth partiles
    //----------------------------------------------------------------------------------------------------------------------
    double m_cp;
signals:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief signal emitted when the cloth has been created
    //----------------------------------------------------------------------------------------------------------------------
    void clothcreated();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief signal to reset the cloth to its initila state
    //----------------------------------------------------------------------------------------------------------------------
    void resetCloth();
public slots :
	//----------------------------------------------------------------------------------------------------------------------
	/// @brief set the timer duration value
	/// @param[in] _v the timer value in ms
	//----------------------------------------------------------------------------------------------------------------------
	inline void setTimerDuration(int _v){m_timerValue=_v;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief to start simulation
    //----------------------------------------------------------------------------------------------------------------------
    void setSim();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief method for cloth's collision with sphere
    //----------------------------------------------------------------------------------------------------------------------
    void setSphereCollision(bool _mode);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief method for cloth's collision with table
    //----------------------------------------------------------------------------------------------------------------------
    void setTableCollision(bool _mode);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief method to set the bool for applying wind on cloth
    /// @return bool to set wind or not
    //----------------------------------------------------------------------------------------------------------------------
    void setApplyWind(bool _mode);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief method to set number of cloth particles
    ///         it emits a signal once it has been creating, setting a flag to draw the cloth
    //----------------------------------------------------------------------------------------------------------------------
    void setClothPartNum(double _n);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief method to set the draw flag for drawing the cloth
    //----------------------------------------------------------------------------------------------------------------------
    void setDrawFlag();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief method to set cloth width
    //----------------------------------------------------------------------------------------------------------------------
    void setClothWidth(double _w);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief method to set cloth depth
    //----------------------------------------------------------------------------------------------------------------------
    void setClothDepth(double _h);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief method to apply octree
    //----------------------------------------------------------------------------------------------------------------------
    void applyOctree(bool _mode);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief method to add wind magnitude
    //----------------------------------------------------------------------------------------------------------------------
    void addWindMag(double _mag);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief method to texture file from UI
    //----------------------------------------------------------------------------------------------------------------------
    void setTexFileName(std::string _filename);

private :
	//----------------------------------------------------------------------------------------------------------------------
	/// @brief the timer
	//----------------------------------------------------------------------------------------------------------------------
	int m_timer;
	//----------------------------------------------------------------------------------------------------------------------
    /// @brief used to store the x rotation mouse value
	//----------------------------------------------------------------------------------------------------------------------
    int m_spinXFace;
	//----------------------------------------------------------------------------------------------------------------------
    /// @brief used to store the y rotation mouse value
	//----------------------------------------------------------------------------------------------------------------------
    int m_spinYFace;
	//----------------------------------------------------------------------------------------------------------------------
    /// @brief flag to indicate if the mouse button is pressed when dragging
	//----------------------------------------------------------------------------------------------------------------------
    bool m_rotate;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief flag to indicate if the Right mouse button is pressed when dragging
    //----------------------------------------------------------------------------------------------------------------------
     bool m_translate;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the previous x mouse value
    //----------------------------------------------------------------------------------------------------------------------
     int m_origX;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the previous y mouse value
    //----------------------------------------------------------------------------------------------------------------------
      int m_origY;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the previous x mouse value for Position changes
    //----------------------------------------------------------------------------------------------------------------------
    int m_origXPos;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the previous y mouse value for Position changes
    //----------------------------------------------------------------------------------------------------------------------
    int m_origYPos;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the model position for mouse movement
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 m_modelPos;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Our Camera
	//----------------------------------------------------------------------------------------------------------------------
    ngl::Camera *m_cam;
	//----------------------------------------------------------------------------------------------------------------------
     /// @brief transformation stack for the gl transformations etc
    //----------------------------------------------------------------------------------------------------------------------
    ngl::TransformStack m_transformStack;
	//----------------------------------------------------------------------------------------------------------------------
    /// @brief pointer to Cloth
	//----------------------------------------------------------------------------------------------------------------------
    Cloth *m_cloth;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief meshes
    //----------------------------------------------------------------------------------------------------------------------
    Props m_prop;
	//----------------------------------------------------------------------------------------------------------------------
	/// @brief the timer value in ms
    //----------------------------------------------------------------------------------------------------------------------
    int m_timerValue;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief table's coordinate: tcenter - table center; t1,t2,t3,t4 - vertices; tnormal - table normal
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 tcenter,t1,t2,t3,t4,tnormal;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief table's width and depth
    //----------------------------------------------------------------------------------------------------------------------
    float twidth,tdepth;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief texture id for table and sphere;
    //----------------------------------------------------------------------------------------------------------------------
    GLuint m_texIDTable, m_texIDSphere;

    bool m_sim;
    bool m_sphereCollision;
    bool m_tableCollision;
    bool m_applyWind;
    bool m_draw;
    bool m_reset;
    std::string clothtex;

protected:

	//----------------------------------------------------------------------------------------------------------------------
    /// @brief  The following methods must be implimented in the sub class
    /// this is called when the window is created
	//----------------------------------------------------------------------------------------------------------------------
    void initializeGL();

	//----------------------------------------------------------------------------------------------------------------------
    /// @brief this is called whenever the window is re-sized
    /// @param[in] _w the width of the resized window
    /// @param[in] _h the height of the resized window
	//----------------------------------------------------------------------------------------------------------------------
    void resizeGL(
                const int _w,
                const int _h
               );
	//----------------------------------------------------------------------------------------------------------------------
    /// @brief this is the main gl drawing routine which is called whenever the window needs to
    /// be re-drawn
	//----------------------------------------------------------------------------------------------------------------------
    void paintGL();

private :
	//----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called every time a mouse is moved
    /// @param _event the Qt Event structure
	//----------------------------------------------------------------------------------------------------------------------
    void mouseMoveEvent (
                       QMouseEvent * _event
                      );
	//----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse button is pressed
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
	//----------------------------------------------------------------------------------------------------------------------

    void mousePressEvent (
                        QMouseEvent *_event
                       );

	//----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse button is released
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
	//----------------------------------------------------------------------------------------------------------------------
    void mouseReleaseEvent (
                          QMouseEvent *_event
                         );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse wheel is moved
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void wheelEvent(
                    QWheelEvent *_event
                 );

	//----------------------------------------------------------------------------------------------------------------------
	/// @brief timer event trigered by startTimer
	//----------------------------------------------------------------------------------------------------------------------
	void timerEvent(
										QTimerEvent *_event
									);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief method to load the tx stack (+MVP) to the shader
    /// @param _tx the transform stack to use for the model part of the matrix
    //----------------------------------------------------------------------------------------------------------------------
    void loadMatricesToShader(
                             ngl::TransformStack &_tx
                            );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief method load matrices to the colour shader
    //----------------------------------------------------------------------------------------------------------------------
    void loadMatricesToColourShader(
                                     ngl::TransformStack &_tx
                                  );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief method for cloth's collision with sphere
    //----------------------------------------------------------------------------------------------------------------------
    void clothSphereCollision();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief method to apply wind on cloth
    //----------------------------------------------------------------------------------------------------------------------
    //void applyWindForce();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief method for cloth's collision with table
    //----------------------------------------------------------------------------------------------------------------------
    void dropOnTable();
};

#endif