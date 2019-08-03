

#include "qtdaswahreschlagerfestwidget.h"

#include <QApplication>
#include <QKeyEvent>
#include <QPainter>
#include <QPixmap>
#include <QTimer>

#ifdef USE_SFML_FOR_JOYSTICK_SUPPORT
#include <SFML/Window/Joystick.hpp>
#endif // USE_SFML_FOR_JOYSTICK_SUPPORT

#include "daswahreschlagerfestwidget.h"
#include "qtgraphics.h"
#include "testtimer.h"
#include "trace.h"


ribi::dws::QtDwsWidget::QtDwsWidget(QWidget *parent) noexcept
  : QWidget(parent),
    m_beer(":/images/DasWahreSchlagerfestBeer.png"),
    m_bratwurst(":/images/DasWahreSchlagerfestBratwurst.png"),
    m_empty(":/images/DasWahreSchlagerfestSmiley.png"),
    m_image{QtGraphics().CreateImage(1000,600,0)},
    m_keys{},
    m_timer_joystick{new QTimer(this)}
{
  assert(m_beer.width() == 102);
  assert(m_bratwurst.width() == 102);
  assert(m_empty.width() == 102);

  //Timer for checking the joystick input
  {
    QObject::connect(m_timer_joystick,SIGNAL(timeout()),this,SLOT(OnJoystickCheck()));
    m_timer_joystick->setInterval(100);
    m_timer_joystick->start();
  }
}

ribi::dws::QtDwsWidget::~QtDwsWidget() noexcept
{
  m_timer_joystick->stop();
}

void ribi::dws::QtDwsWidget::DoDisplay(const Widget& /* widget */)
{
  qApp->processEvents();
  this->repaint();
  this->show();
}

const QPixmap& ribi::dws::QtDwsWidget::GetPixmap(const Tile& tile) const noexcept
{
  switch (tile)
  {
    case Tile::beer     : return m_beer;
    case Tile::bratwurst: return m_bratwurst;
    default: break;
  }
  assert(tile == Tile::empty);
  return m_empty;
}


void ribi::dws::QtDwsWidget::keyPressEvent(QKeyEvent * e) noexcept
{
  switch (e->key())
  {
    case Qt::Key_Right:  m_keys.push_back(Key::right); break;
    case Qt::Key_Down:   m_keys.push_back(Key::down); break;
    case Qt::Key_Left:   m_keys.push_back(Key::left); break;
    case Qt::Key_Escape: m_keys.push_back(Key::quit);
      this->close();
      return;
    default: return;
  }
}

void ribi::dws::QtDwsWidget::OnChanged(const Widget& widget)
{
  const std::vector<std::vector<Tile> > & v = widget.GetTiles();
  assert(!v.empty());
  const int rows = static_cast<int>(v.size()   );
  const int cols = static_cast<int>(v[0].size());
  if (width() != cols * m_beer.width() || height() != rows * m_beer.height())
  {
    const int width {cols * m_beer.width() };
    const int height{rows * m_beer.height()};
    this->setGeometry(0,0,width,height);
    m_image = QtGraphics().CreateImage(width,height,0);
  }
  const int block_width  = static_cast<double>(width() ) / static_cast<double>(cols);
  const int block_height = static_cast<double>(height()) / static_cast<double>(rows);
  for (int y=0; y!=rows; ++y)
  {
    assert(y < static_cast<int>(v.size()));
    const std::vector<Tile>& line = v[y];
    assert(cols == static_cast<int>(line.size()));
    const int y1 = static_cast<int>(block_height * static_cast<double>(y  ));
    for (int x=0; x!=cols; ++x)
    {
      assert(x < static_cast<int>(line.size()));
      const QPixmap& pixmap = GetPixmap(line[x]);
      const int x1 = static_cast<int>(block_width * static_cast<double>(x  ));
      QtGraphics().DrawImage(m_image,pixmap.toImage(),x1,y1);
    }
  }
  //Draw cursor
  {
    const Cursor cursor = widget.GetCursor();
    const int x = cursor.x * block_width;
    const int y = cursor.y * block_height;
    const QPixmap& pixmap = GetPixmap(cursor.tile);
    QtGraphics().DrawImage(m_image,pixmap.toImage(),x,y);
  }

  this->repaint();
}

void ribi::dws::QtDwsWidget::OnJoystickCheck()
{
  #ifdef USE_SFML_FOR_JOYSTICK_SUPPORT
  sf::Joystick::update();
  const int joystick_index{0};
  if (sf::Joystick::isConnected(joystick_index))
  {
    if (sf::Joystick::hasAxis(joystick_index, sf::Joystick::X))
    {
      const double dx{sf::Joystick::getAxisPosition(joystick_index, sf::Joystick::X)};
      if (dx < -50.0) { m_keys.push_back(Key::left); }
      if (dx >  50.0) { m_keys.push_back(Key::right); }
    }
    if (sf::Joystick::hasAxis(joystick_index, sf::Joystick::Y))
    {
      const double dy{sf::Joystick::getAxisPosition(joystick_index, sf::Joystick::Y)};
      if (dy > 50.0) { m_keys.push_back(Key::down); }
    }
    if (sf::Joystick::isButtonPressed(joystick_index,0)) {
      close();
    }
  }
  #endif // USE_SFML_FOR_JOYSTICK_SUPPORT
}

void ribi::dws::QtDwsWidget::paintEvent(QPaintEvent *) noexcept
{
  QPainter painter(this);
  painter.drawImage(0,0,m_image);
}


ribi::dws::Key ribi::dws::QtDwsWidget::RequestKey()
{
  while (1)
  {
    qApp->processEvents();
    if (m_keys.empty()) continue;
    const auto key = m_keys.back();
    m_keys.clear();
    return key;
  }
}
