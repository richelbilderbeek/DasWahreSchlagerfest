#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "qtdaswahreschlagerfestmenudialog.h"

#include <QKeyEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QTimer>

#ifdef USE_SFML_FOR_JOYSTICK_SUPPORT
#include <SFML/Window/Joystick.hpp>
#endif //USE_SFML_FOR_JOYSTICK_SUPPORT

#include "daswahreschlagerfestmenudialog.h"
#include "qtaboutdialog.h"
#include "qtdaswahreschlagerfestwidget.h"
#include "daswahreschlagerfestwidget.h"
#include "testtimer.h"
#include "ui_qtdaswahreschlagerfestmenudialog.h"
#include "trace.h"
#pragma GCC diagnostic pop

QKeyEvent CreateDown() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_Down,Qt::NoModifier); }
QKeyEvent CreateUp() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_Up,Qt::NoModifier); }
QKeyEvent CreateTab() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_Tab,Qt::NoModifier); }
QKeyEvent CreateEnter() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_Enter,Qt::NoModifier); }

ribi::dws::QtMenuDialog::QtMenuDialog(QWidget *parent) noexcept
  : QtHideAndShowDialog(parent),
    ui(new Ui::QtDwsMenuDialog),
    m_timer_joystick{new QTimer(this)}
{
  ui->setupUi(this);

  //Timer for checking the joystick input
  {
    QObject::connect(m_timer_joystick,SIGNAL(timeout()),this,SLOT(OnJoystickCheck()));
    m_timer_joystick->setInterval(100);
    m_timer_joystick->start();
  }
  ui->button_start->setFocus();
}

ribi::dws::QtMenuDialog::~QtMenuDialog() noexcept
{
  delete ui;
}

void ribi::dws::QtMenuDialog::keyPressEvent(QKeyEvent * e)
{
  if (e->key() == Qt::Key_Escape) { close(); }
  QtHideAndShowDialog::keyPressEvent(e);
}

void ribi::dws::QtMenuDialog::on_button_start_clicked() noexcept
{
  QtDwsWidget d;
  Widget w(&d);
  d.OnChanged(w);
  {
    QRect screen = QApplication::desktop()->screenGeometry();
    d.move( screen.center() - d.rect().center() );
  }
  d.setWindowTitle("Das Wahre Schlagerfest");
  d.show();
  w.Execute();
}

void ribi::dws::QtMenuDialog::on_button_about_clicked() noexcept
{
  QtAboutDialog d(MenuDialog().GetAbout());
  d.setWindowIcon(windowIcon());
  d.setStyleSheet(styleSheet());
  ShowChild(&d);
}

void ribi::dws::QtMenuDialog::OnJoystickCheck()
{
  #ifdef USE_SFML_FOR_JOYSTICK_SUPPORT
  sf::Joystick::update();
  const int joystick_index{0};
  if (sf::Joystick::isConnected(joystick_index))
  {
    if (sf::Joystick::hasAxis(joystick_index, sf::Joystick::Y))
    {
      const double dy{sf::Joystick::getAxisPosition(joystick_index, sf::Joystick::Y)};
      if (dy > 50.0) {
        this->focusNextChild();
      }
      if (dy < -50.0) {
        this->focusPreviousChild();
      }
    }
    if (sf::Joystick::isButtonPressed(joystick_index,0)) {
      auto key = CreateEnter();
      keyPressEvent(&key);
    }
  }
  #endif // USE_SFML_FOR_JOYSTICK_SUPPORT
}


void ribi::dws::QtMenuDialog::on_button_quit_clicked() noexcept
{
  close();
}

