#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
//change num to hex
static char HEX_CHARACTERS[] = "0123456789ABCDEF";
#define BIT_WIDE_HEX 8
//define append, which add a char to buffer
#define append(x) {out[j++]=x; if (j >= n) {break;}}
//Other functions can be implemented based on vsnprintf
int vsnprintf(char *out, size_t n, const char *fmt, va_list ap);
int printf(const char *fmt, ...) {
  char buffer[128];
  va_list tmp_list;
  va_start(tmp_list , fmt);
  int res = vsnprintf(buffer, -1, fmt, tmp_list);
  putstr(buffer);
  va_end(tmp_list);
  return res;
  //panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  int res = vsnprintf(out, -1, fmt, ap);
  return res;
  //panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
  //char buffer[2048];
  va_list tmp_list;
  va_start(tmp_list , fmt);
  int res = vsnprintf(out, -1, fmt, tmp_list);
  va_end(tmp_list);
  return res;
  //panic("Not implemented");
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  va_list tmp_list;
  va_start(tmp_list , fmt);
  int res = vsnprintf(out, n, fmt, tmp_list);
  va_end(tmp_list);
  return res;
  //panic("Not implemented");
}


int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
    int state = 0, i, j = 0;

    for (i = 0; fmt[i] != '\0'; ++i) {
        if (state == 0) {
            if (fmt[i] != '%') {
                append(fmt[i]);
            } else {
                state = 1;
            }
        } else {
            switch (fmt[i]) {
                case 's': {
                    const char *tmp_char = va_arg(ap, const char *);
                    while (*tmp_char) {
                        append(*tmp_char++);
                    }
                    state = 0;
                    break;
                }
                case 'd': {
                    int tmp_num = va_arg(ap, int);
                    if (tmp_num < 0) {
                        append('-');
                        tmp_num = -tmp_num;
                    }
                    char num_buffer[20]; 
                    int len = 0;
                    while (tmp_num) {
                        num_buffer[len++] = HEX_CHARACTERS[tmp_num % 10];
                        tmp_num /= 10;
                    }
                    for (int k = len - 1; k >= 0; --k) {
                        append(num_buffer[k]);
                    }
                    state = 0;
                    break;
                }
                case 'c': {
                    char cha = (char)va_arg(ap, int);
                    append(cha);
                    state = 0;
                    break;
                }
                default:
                    append('%');
                    append(fmt[i]);
                    state = 0;
                    break;
            }
        }
    }
    out[j] = '\0'; 
    return j;
}
#endif
