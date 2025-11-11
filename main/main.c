/* Multiple build configurations example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "main.h"

void app_main(void)
{
    /* This will call func() either from func_dev.c or func_prod.c, depending on
     * the build configuration.
     */
    main();
}