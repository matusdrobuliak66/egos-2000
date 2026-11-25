/*
 * (C) 2026, Cornell University
 * All rights reserved.
 *
 * Description: starter code for implementing printf()
 */

void terminal_write(const char *str, int len) {
    for (int i = 0; i < len; i++) {
        *(char*)(0x10000000) = str[i];
    }
}

/* Uncomment the code block below when implementing formatted output.
 */

#include <stdlib.h>  // for itoa() and utoa()
#include <string.h>  // for strlen() and strcat()
#include <stdarg.h>  // for va_start(), va_end(), va_arg() and va_copy()

void format_to_str(char* out, const char* fmt, va_list args) {
    for(out[0] = 0; *fmt != '\0'; fmt++) {
        if (*fmt != '%') {
            strncat(out, fmt, 1);
        } else {
            fmt++;
            if (*fmt == 's') {
                strcat(out, va_arg(args, char*));
            } else if (*fmt == 'd') {
                itoa(va_arg(args, int), out + strlen(out), 10);
            } else if (*fmt == 'c') {
                char c = (char)va_arg(args, int);
                strncat(out, &c, 1);
            } else if (*fmt == 'x') {
                itoa(va_arg(args, int), out + strlen(out), 16);
            } else if (*fmt == 'u') {
                utoa(va_arg(args, unsigned int), out + strlen(out), 10);
            } else if (*fmt == 'p') {
                itoa((unsigned long)va_arg(args, void*), out + strlen(out), 16);
            } else if (*fmt == 'l' && *(fmt + 1) == 'l' && *(fmt + 2) == 'u') {
                fmt += 2;
                utoa(va_arg(args, unsigned long long), out + strlen(out), 10);
            }
        }
    }
}

unsigned int format_to_str_len(const char* fmt, va_list args) {
    unsigned int len = 0;
    for(; *fmt != '\0'; fmt++) {
        if (*fmt != '%') {
            len++;
        } else {
            fmt++;
            if (*fmt == 's') {
                len += strlen(va_arg(args, char*));
            } else if (*fmt == 'd') {
                char buf[32];
                itoa(va_arg(args, int), buf, 10);
                len += strlen(buf);
            } else if (*fmt == 'c') {
                len += 1;
            } else if (*fmt == 'x') {
                char buf[32];
                itoa(va_arg(args, int), buf, 16);
                len += strlen(buf);
            } else if (*fmt == 'u') {
                char buf[32];
                utoa(va_arg(args, unsigned int), buf, 10);
                len += strlen(buf);
            } else if (*fmt == 'p') {
                char buf[32];
                itoa((unsigned long)va_arg(args, void*), buf, 16);
                len += strlen(buf);
            } else if (*fmt == 'l' && *(fmt + 1) == 'l' && *(fmt + 2) == 'u') {
                fmt += 2;
                char buf[32];
                utoa(va_arg(args, unsigned long long), buf, 10);
                len += strlen(buf);
            }
        }
    }
    return len + 1;  // +1 for null terminator
}

int printf(const char* format, ...) {
    // char buf[512];  This was for stack allocation, now replaced with dynamic allocation.
    va_list args;
    va_start(args, format);

    va_list args_copy;
    va_copy(args_copy, args);
    unsigned int len = format_to_str_len(format, args_copy);
    char * buf = malloc(len);

    format_to_str(buf, format, args);
    va_end(args);
    terminal_write(buf, strlen(buf));

    va_end(args_copy);
    free(buf);

    return 0;
}


/* Uncomment the code block below when implementing dynamic memory allocation.
 */

extern char __heap_start, __heap_end;
static char* brk = &__heap_start;
char* _sbrk(int size) {
    if (brk + size > (char*)&__heap_end) {
        terminal_write("_sbrk: heap grows too large\r\n", 29);
        return NULL;
    }

    char* old_brk = brk;
    brk += size;
    return old_brk;
}


int main() {
    char* msg = "Hello, World!\n\r";
    terminal_write(msg, 15);

    /* Uncomment this line of code when implementing formatted output. */
    printf("%s-%d is awesome!\n\r", "egos", 2000); 

    printf("%c is character $\n\r", '$');
    printf("%c is character 0\n\r", (char)48);
    printf("%x is integer 1234 in hexadecimal\n\r", 1234);
    printf("%u is the maximum of unsigned int\n\r", (unsigned int)0xFFFFFFFF);
    printf("%p is the hexadecimal address of the hello-world string\n\r", msg);
    printf("%llu is the maximum of unsigned long long\n\r", 0xFFFFFFFFFFFFFFFFULL);

    return 0;
}
