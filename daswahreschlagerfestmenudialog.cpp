#include "daswahreschlagerfestmenudialog.h"

#include <cassert>
#include <iostream>

#include "daswahreschlagerfestterminal.h"
#include "daswahreschlagerfestwidget.h"

int ribi::dws::MenuDialog::ExecuteSpecific(const std::vector<std::string>& argv) noexcept
{
  const int argc = static_cast<int>(argv.size());
  if (argc == 1)
  {
    std::cout << GetHelp() << '\n';
    return 0;
  }
  Terminal terminal;

  if (argc == 2 && (argv[1] == "-d" || argv[1] == "--demo"))
  {
    terminal.SetAutoPlay(true);
  }
  Widget w(&terminal);
  w.Execute();
  return 0;
}

ribi::About ribi::dws::MenuDialog::GetAbout() const noexcept
{
  About a(
    "Richel Bilderbeek",
    "Das Wahre Schlagerfest",
    "a truely fun game",
    "November 28th of 2015",
    "2003-2015",
    "http://www.richelbilderbeek.nl/GameDasWahreSchlagerfest.htm",
    GetVersion(),
    GetVersionHistory()
  );
  return a;
}

ribi::Help ribi::dws::MenuDialog::GetHelp() const noexcept
{
  return Help(
    GetAbout().GetFileTitle(),
    GetAbout().GetFileDescription(),
    {
      Help::Option('d',"demo","See a demo"),
      Help::Option('p',"play","Play the game"),
    },
    {
      GetAbout().GetFileTitle() + " -d",
      GetAbout().GetFileTitle() + " --demo",
      GetAbout().GetFileTitle() + " -p",
      GetAbout().GetFileTitle() + " --play"
    }
  );
}

std::string ribi::dws::MenuDialog::GetVersion() const noexcept
{
  return "4.1";
}

std::vector<std::string> ribi::dws::MenuDialog::GetVersionHistory() const noexcept
{
  return {
    "2003-09-30: version 1.1: first VCL (Windows-only) version",
    "2012-07-21: version 2.0: ported to Qt (cross-platform)",
    "2012-07-29: version 2.1: changed graphics to old-school",
    "2012-08-09: version 2.2: changed graphics, changed new tile selection (rendering the eat-it all tile) obsolete"
    "2014-01-30: version 2.3: added command-line demo and play mode",
    "2014-01-31: version 2.4: added terminal-like mode from command-line",
    "2014-02-10: version 2.5: added terminal-like mode in desktop version",
    "2015-08-28: version 3.0: removed terminal-like mode in desktop version, ported to NDS",
    "2015-11-15: version 4.0: moved to own GitHub",
    "2015-11-28: version 4.1: added joystick support"
  };
}
