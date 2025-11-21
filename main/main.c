/* Multiple build configurations example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <main_app.h>

void app_main(void)
{
   /* This will call main_app() either from main_write_bot_info.cpp or
      main_robot.cpp, depending on the build configuration.
   */
   main_app();
}