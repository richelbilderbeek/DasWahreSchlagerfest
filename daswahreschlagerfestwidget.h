#ifndef DASWAHRESCHLAGERFESTWIDGET_H
#define DASWAHRESCHLAGERFESTWIDGET_H

#include <iosfwd>
#include <vector>
//Cannot use Boost, due to libnds

#include "daswahreschlagerfestkey.h"
#include "daswahreschlagerfestcursor.h"


namespace ribi {
namespace dws {

struct Display;

///Widget contains the game logic of Das Wahre Schlagerfest
struct Widget
{
  Widget(
    Display * const display = nullptr,
    const int width = 9,
    const int height = 5
  );
  Widget(const Widget&) = delete;
  Widget& operator=(const Widget&) = delete;

  void DoDisplay() const;

  ///Play the game
  void Execute();

  ///Obtain the cursor
  const Cursor& GetCursor() const { return m_cursor; }

  ///Obtain the Y-X ordered tiles, without the cursor
  const std::vector<std::vector<Tile> >& GetTiles() const { return m_v; }

  ///Respond to the user pressing a key
  void PressKey(const ribi::dws::Key key);

  void SetDisplay(Display * const display);

  private:
  ///The cursor
  Cursor m_cursor;

  ///The display of the game, can be console, GUI or NDS
  mutable Display * m_display;

  ///The Y-X ordered tiles
  std::vector<std::vector<Tile>> m_v;

  //Check for three in a rows
  void CheckThree();
};

std::ostream& operator<<(std::ostream& os,const Widget& w);

} //~namespace dws
} //~namespace ribi

#endif // DASWAHRESCHLAGERFESTWIDGET_H
