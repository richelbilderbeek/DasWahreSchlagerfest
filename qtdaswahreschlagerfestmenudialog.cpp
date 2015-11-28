//---------------------------------------------------------------------------
/*
Das Wahre Schlagerfest, a simple game
Copyright (C) 2003-2015 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/GameDasWahreSchlagerfest.htm
//---------------------------------------------------------------------------
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
/*

QDialog {
  background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0  #ff6500, stop: 1 #ffc500);
}

QPushButton {
  font: 32px, "Verdana";
  border-color: #000;
}
QPushButton:focus {
  font: 64px, "Verdana";
  border: 4px solid #000;
}

QPushButton#button_start{
  color: #fff;
  background-color: #000;
}
QPushButton#button_about{
  color: #800;
  background-color: #f00;
}
QPushButton#button_quit{
  color: #880;
  background-color: #ff0;
}


*/

QKeyEvent CreateDown() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_Down,Qt::NoModifier); }
QKeyEvent CreateUp() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_Up,Qt::NoModifier); }
QKeyEvent CreateTab() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_Tab,Qt::NoModifier); }
QKeyEvent CreateEnter() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_Enter,Qt::NoModifier); }

ribi::dws::QtMenuDialog::QtMenuDialog(QWidget *parent) noexcept
  : QtHideAndShowDialog(parent),
    ui(new Ui::QtDwsMenuDialog),
    m_timer_joystick{new QTimer(this)}
{
  #ifndef NDEBUG
  Test();
  #endif
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

#ifndef NDEBUG
void ribi::dws::QtMenuDialog::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  {
    QtDwsWidget();
  }
  const TestTimer test_timer(__func__,__FILE__,1.0);
}
#endif
