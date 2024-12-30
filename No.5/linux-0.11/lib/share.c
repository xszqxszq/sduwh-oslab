/*
 *  linux/lib/share.c
 *
 *  (C) 2024  XSZQ
 */

#define __LIBRARY__
#include <unistd.h>

_syscall1(int,share_memory_create,int,size)
_syscall1(int,share_memory_with,pid_t,pid)