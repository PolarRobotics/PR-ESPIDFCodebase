// components/Utilities/log_wrap.c
#include "esp_log.h"
#include <stdarg.h>

// Declarations of the *real* functions so we can forward to them.
int __real_esp_log_write(esp_log_level_t level, const char *tag, const char *format, ...);
int __real_esp_log_writev(esp_log_level_t level, const char *tag, const char *format, va_list args);

// Wrapper used because of -Wl,--wrap=esp_log_write
int __wrap_esp_log_write(esp_log_level_t level, const char *tag, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    // Forward to the real varargs version to avoid reformatting
    int ret = __real_esp_log_writev(level, tag, format, args);
    va_end(args);
    return ret;
}

// Wrapper used because of -Wl,--wrap=esp_log_writev
int __wrap_esp_log_writev(esp_log_level_t level, const char *tag, const char *format, va_list args)
{
    // 🔧 Choose one of the behaviors below:

    // 1) Forward logs normally (default/recommended)
    return __real_esp_log_writev(level, tag, format, args);

    // 2) Or completely drop all logs:
    // return 0;
}

// kill me