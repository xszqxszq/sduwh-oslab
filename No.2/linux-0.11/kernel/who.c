/*
 *  linux/kernel/who.c
 *
 *  (C) 2024  XSZQ
 */
#include <errno.h>
#include <linux/kernel.h>
#include <asm/segment.h>
#define MYNAME_LENGTH 21
char myname[MYNAME_LENGTH] = "XSZQ";
EXPORT_SYMBOL(myname);

int sys_whoami(char* name, unsigned int size)
{   
    char *p = myname, *q = name;
    while (*p != '\0') {
        if (p - myname >= MYNAME_LENGTH || q - name >= size)
            return -EINVAL;
        put_fs_byte(*p++, q++);
    }
    return q - name;
}


int sys_iam(const char * name)
{
    char c, *p = myname;
    while ((c = get_fs_byte(name++)) != '\0') {
        if (p - myname >= MYNAME_LENGTH)
            return -EINVAL;
        *p++ = c;
    }
    *p = '\0';

    return p - myname;
}