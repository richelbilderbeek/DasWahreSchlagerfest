#pragma GCC diagnostic push


#include "daswahreschlagerfestwidget.h"

#include <algorithm>
#include <cassert>
#include <numeric>

#include "daswahreschlagerfestdisplay.h"
#pragma GCC diagnostic pop

ribi::dws::Widget::Widget(
  Display * const display,
  const int width, const int height
)
  : m_cursor{Cursor(width / 2,0,Tile::beer)},
    m_display(display),
    m_v(height,std::vector<Tile>(width,Tile::empty))
{
  assert(height == static_cast<int>(m_v.size()));
  assert(width == static_cast<int>(m_v[0].size()));
}

void ribi::dws::Widget::CheckThree()
{
  assert(!m_v.empty());
  assert(!m_v[0].empty());
  const int height = m_v.size();
  const int width = m_v[0].size();
  std::vector<std::vector<bool> > w(height,std::vector<bool>(width,false));
  assert(w.size() == m_v.size());
  assert(w[0].size() == m_v[0].size());
  {
    bool has_three = false;
    for (int y=0; y!=height; ++y)
    {
      for (int x=0; x!=width; ++x)
      {
        if (m_v[y][x] == Tile::empty) continue;

        if (x < width-2 && m_v[y][x] == m_v[y][x+1] && m_v[y][x+1] == m_v[y][x+2])
        {
          w[y][x] = true; w[y][x+1] = true; w[y][x+2] = true; has_three = true;
        }
        if (y < height - 2 && m_v[y][x] == m_v[y+1][x] && m_v[y+1][x] == m_v[y+2][x])
        {
          w[y][x] = true; w[y+1][x] = true; w[y+2][x] = true; has_three = true;
        }
        if (x < width -2 && y < height -2 && m_v[y][x] == m_v[y+1][x+1] && m_v[y+1][x+1] == m_v[y+2][x+2])
        {
          w[y][x] = true; w[y+1][x+1] = true; w[y+2][x+2] = true; has_three = true;
        }
        if (x < width -2 && y > 2 && m_v[y][x] == m_v[y-1][x+1] && m_v[y-1][x+1] == m_v[y-2][x+2])
        {
          w[y][x] = true; w[y-1][x+1] = true; w[y-2][x+2] = true; has_three = true;
        }
      }
    }
    if (!has_three) return;
  }
  //Remove the tiles
  for (int y=0; y!=height; ++y)
  {
    for (int x=0; x!=width; ++x)
    {
      if (w[y][x]) m_v[y][x] = Tile::empty;
    }
  }

  //Make the tiles fall down
  {
    bool tile_has_fallen = true;
    while (tile_has_fallen)
    {
      tile_has_fallen = false;
      for (int y=0; y!=height-1; ++y)
      {
        for (int x=0; x!=width; ++x)
        {
          if (m_v[y][x] != Tile::empty && m_v[y+1][x] == Tile::empty)
          {
            std::swap(m_v[y][x],m_v[y+1][x]);
            tile_has_fallen = true;
          }
        }
      }
    }
  }
  //Check if all tile have fallen down
  #ifndef NDEBUG
  {
    for (int y=0; y!=height-1; ++y)
    {
      for (int x=0; x!=width; ++x)
      {
        if (m_v[y][x] != Tile::empty) assert(m_v[y+1][x] != Tile::empty && "Assume no empty space beneath tile");
      }
    }
  }
  #endif
  CheckThree();
}

void ribi::dws::Widget::DoDisplay() const
{
  assert(m_display);
  m_display->DoDisplay(*this);
}

void ribi::dws::Widget::Execute()
{

  while (1)
  {
    DoDisplay();
    ribi::dws::Key key = m_display->RequestKey();
    PressKey(key);
    if (key == ribi::dws::Key::quit) break;
  }
}

void ribi::dws::Widget::PressKey(const ribi::dws::Key key)
{
  switch (key)
  {
    case Key::right:
    {
      if (m_cursor.x + 1 < static_cast<int>(m_v[m_cursor.y].size()))
      {
        if (m_v[m_cursor.y][m_cursor.x + 1] == Tile::empty)
        {
          ++m_cursor.x;
          m_display->OnChanged(*this);
        }
      }
    }
    break;
    case Key::down:
    {
      if (m_cursor.y + 1 != static_cast<int>(m_v.size()))
      {
        if (m_v[m_cursor.y+1][m_cursor.x] == Tile::empty)
        {
          ++m_cursor.y;
          m_display->OnChanged(*this);
          return;
        }
      }
      if (m_cursor.y + 1 == static_cast<int>(m_v.size()) || m_v[m_cursor.y+1][m_cursor.x] != Tile::empty)
      {
        m_v[m_cursor.y][m_cursor.x] = m_cursor.tile;
        //Check for three in a row
        CheckThree();
        //Place cursor
        m_cursor.x = static_cast<int>(m_v[0].size()) / 2;
        m_cursor.y = 0;
        while (m_v[m_cursor.y+1][m_cursor.x] != Tile::empty)
        {
          //Look for another spot
          m_cursor.x = std::rand() % static_cast<int>(m_v[0].size());
        }

        if (std::accumulate(m_v.begin(),m_v.end(),0,
            [](int& sum, const std::vector<Tile>& w)
            { return sum + std::count(w.begin(),w.end(),Tile::beer); }
          ) > 5) { m_cursor.tile = Tile::beer; }
        else if (std::accumulate(m_v.begin(),m_v.end(),0,
            [](int& sum, const std::vector<Tile>& w)
            { return sum + std::count(w.begin(),w.end(),Tile::bratwurst); }
          ) > 5) { m_cursor.tile = Tile::bratwurst; }
        else m_cursor.tile = (std::rand() >> 4) % 2 ? Tile::beer : Tile::bratwurst;
        m_display->OnChanged(*this);
      }

    }
    break;
    case Key::left:
    {
      if (m_cursor.x > 0
        && m_v[m_cursor.y][m_cursor.x - 1] == Tile::empty)
      {
        --m_cursor.x;
        m_display->OnChanged(*this);
      }
    }
    break;
    case Key::quit: break; //Cannot handle that here

  }
}

void ribi::dws::Widget::SetDisplay(Display * const display)
{
  assert(display);
  m_display = display;
  m_display->DoDisplay(*this);
}

