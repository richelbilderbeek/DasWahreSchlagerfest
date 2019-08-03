#pragma GCC diagnostic push

#include <QApplication>
#include "qtdaswahreschlagerfestmenudialog.h"
#pragma GCC diagnostic pop

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  ribi::dws::QtMenuDialog w;
  w.show();
  return a.exec();
}
