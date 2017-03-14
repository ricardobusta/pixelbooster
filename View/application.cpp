#include "application.h"

#include "mainwindow.h"

Application::Application(int argc, char **argv)
  :QApplication(argc,argv)
{
  mainWindow = new MainWindow();
  mainWindow->show();
}
