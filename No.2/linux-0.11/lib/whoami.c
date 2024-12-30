/*
 *  linux/lib/whoami.c
 *
 *  (C) 2024  XSZQ
 */

#define __LIBRARY__
#include <unistd.h>
#include <stdarg.h>


int whoami(char * name, unsigned int size)
{
    register int res;
    __asm__("int $0x80"
        :"=a" (res)
        :"0" (__NR_iam),"b" (name),"c" (size));
    if (res>=0)
        return res;
    errno = -res;
    return -1;
}

