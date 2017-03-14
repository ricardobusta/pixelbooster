#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>

class MainWindow;

class Application : public QApplication
{
public:
  Application(int argc, char **argv);

private:
  MainWindow *mainWindow;
};

#endif // APPLICATION_H
