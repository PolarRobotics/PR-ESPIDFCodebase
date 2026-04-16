// Polar Robotics ESP-IDF Robotic Football Codebase
// Copyright (C) 2026 Polar Robotics

// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU Affero General Public License as published by the Free
// Software Foundation, either version 3 of the License, or(at your option) any
// later version.

// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
// details.

// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#include <main_app.h>

void app_main(void)
{
   /* This will call main_app() either from main_write_bot_info.cpp or
      main_robot.cpp, depending on the build configuration.
   */
   main_app();
}