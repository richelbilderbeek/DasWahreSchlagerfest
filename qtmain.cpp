

#include <QApplication>
#include "qtdaswahreschlagerfestmenudialog.h"


int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  ribi::dws::QtMenuDialog w;
  w.show();
  return a.exec();
}
