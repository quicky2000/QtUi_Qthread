#include <QApplication>
#include "main_window.h"

int main(int argc, char *argv[])
{
  QApplication l_app(argc, argv);

  main_window l_window;
  l_window.show();
  return l_app.exec();
}
//EOF

