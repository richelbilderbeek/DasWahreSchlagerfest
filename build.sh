#!/bin/bash
cd ..
git clone https://github.com/richelbilderbeek/RibiClasses
git clone https://github.com/richelbilderbeek/RibiLibraries
cd DasWahreSchlagerfest
qmake DasWahreSchlagerfestConsole.pro
make
qmake DasWahreSchlagerfestDesktop.pro
make
