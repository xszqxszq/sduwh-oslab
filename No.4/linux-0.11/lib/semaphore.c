/*
 *  linux/lib/semaphore.c
 *
 *  (C) 2024  XSZQ
 */

#define __LIBRARY__
#include <unistd.h>

_syscall1(int,sem_create,char*,semname)
_syscall2(int,sem_set,int,semid,int,value)
_syscall1(int,sem_wait,int,semid)
_syscall1(int,sem_signal,int,semid)
_syscall0(int,get_item)
_syscall1(int,put_item,int,item)