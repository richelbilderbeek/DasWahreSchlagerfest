#ifndef DASWAHRESCHLAGERFESTMENUDIALOG_H
#define DASWAHRESCHLAGERFESTMENUDIALOG_H

#include "menudialog.h"

namespace ribi {
namespace dws {

struct MenuDialog final : public ::ribi::MenuDialog
{
  About GetAbout() const noexcept override;
  Help GetHelp() const noexcept override;
  std::string GetVersion() const noexcept override;
  std::vector<std::string> GetVersionHistory() const noexcept override;

  private:
  int ExecuteSpecific(const std::vector<std::string>& argv) noexcept override;
};

} //~namespace dws
} //~namespace ribi

#endif // DASWAHRESCHLAGERFESTMENUDIALOG_H
