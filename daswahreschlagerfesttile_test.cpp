#include "daswahreschlagerfesttile.h"

#include <boost/test/unit_test.hpp>

using namespace ribi::dws;

BOOST_AUTO_TEST_CASE(dsw_tile_to_char)
{
  BOOST_CHECK_EQUAL(ToChar(Tile::beer), 'V');
  BOOST_CHECK_EQUAL(ToChar(Tile::bratwurst), 'U');
  BOOST_CHECK_EQUAL(ToChar(Tile::empty), '.');
}
