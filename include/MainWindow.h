#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "GLWindow.h"
//#include "RK4Spring.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	//----------------------------------------------------------------------------------------------------------------------
	/// @brief ctor
	/// @param[in] parent the parent window
	//----------------------------------------------------------------------------------------------------------------------
    explicit MainWindow(QWidget *parent = 0);
	//----------------------------------------------------------------------------------------------------------------------
	/// @brief dtor
	//----------------------------------------------------------------------------------------------------------------------
	~MainWindow();

private:
	//----------------------------------------------------------------------------------------------------------------------
	/// @brief a pointer to our user interface
	//----------------------------------------------------------------------------------------------------------------------
	Ui::MainWindow *m_ui;
	//----------------------------------------------------------------------------------------------------------------------
	/// @brief our open gl window
	//----------------------------------------------------------------------------------------------------------------------
	GLWindow *m_gl;

public slots :
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief slot to toggle the sim on and off
    //----------------------------------------------------------------------------------------------------------------------
    //void toggleSim(bool _s);
    void drawWireframe(bool _mode);
    void loadTexFile();

protected:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief override the keyPressEvent inherited from QObject so we can handle key presses.
    /// @param [in] _event the event to process
    //----------------------------------------------------------------------------------------------------------------------
    void keyPressEvent(
                       QKeyEvent *_event
                      );
    //----------------------------------------------------------------------------------------------------------------------
};

#endif // MAINWINDOW_H
