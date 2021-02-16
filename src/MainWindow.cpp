#include "MainWindow.h"
#include "ui_MainWindow.h"

//----------------------------------------------------------------------------------------------------------------------
/// @file MainWindow.cpp
/// @brief main class for user interface and input output operations
//----------------------------------------------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
	// setup the user interface
	m_ui->setupUi(this);
	// create our GL window for drawing the spring
    //m_gl=new  GLWindow(m_ui->m_timerValue->value(),this);
    m_gl = new GLWindow(1,this);

	// add the glWindow to the UI
	m_ui->s_mainWindowGridLayout->addWidget(m_gl,0,0,4,1);

    connect(m_ui->m_simulate,SIGNAL(clicked()),m_gl,SLOT(setSim()));
    connect(m_ui->m_cSphere,SIGNAL(toggled(bool)),m_gl,SLOT(setSphereCollision(bool)));
    //connect(m_ui->m_cTable,SIGNAL(clicked(bool)),m_gl,SLOT(setTableCollision()));
    connect(m_ui->m_cTable,SIGNAL(toggled(bool)),m_gl,SLOT(setTableCollision(bool)));

    connect(m_ui->m_forceWind,SIGNAL(toggled(bool)),m_gl,SLOT(setApplyWind(bool)));
    connect(m_ui->m_noOfParticle,SIGNAL(valueChanged(double)),m_gl,SLOT(setClothPartNum(double)));
    connect(m_gl,SIGNAL(clothcreated()),m_gl,SLOT(setDrawFlag()));
    connect(m_gl,SIGNAL(resetCloth()),m_gl,SLOT(setClothPartNum(m_gl->m_cp;)));


    connect(m_ui->m_width,SIGNAL(valueChanged(double)),m_gl,SLOT(setClothWidth(double)));
    connect(m_ui->m_depth,SIGNAL(valueChanged(double)),m_gl,SLOT(setClothDepth(double)));
    connect(m_ui->m_octree,SIGNAL(toggled(bool)),m_gl,SLOT(applyOctree(bool)));

    connect(m_ui->m_windmag,SIGNAL(valueChanged(double)),m_gl,SLOT(addWindMag(double)));

    connect(m_ui->m_wireframe,SIGNAL(clicked(bool)),this,SLOT(drawWireframe(bool)));
    connect(m_ui->m_browse,SIGNAL(clicked()),this,SLOT(loadTexFile()));
}

//----------------------------------------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete m_ui;
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::keyPressEvent(
                               QKeyEvent *_event
                              )
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
  case Qt::Key_Escape : QApplication::exit(EXIT_SUCCESS); break;
  case Qt::Key_W : glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); break;
  case Qt::Key_S : glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); break;


  default : break;

  }
   m_gl->updateGL();
}
//----------------------------------------------------------------------------------------------------------------------

void MainWindow::drawWireframe(bool _mode)
{
    if(_mode == true)
    {
         glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    }

}
//----------------------------------------------------------------------------------------------------------------------
void MainWindow::loadTexFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open File"), "/path/to/file/");
    m_ui->m_filepath->setText(fileName);
    m_gl->setTexFileName(fileName.toStdString());
}
//----------------------------------------------------------------------------------------------------------------------
