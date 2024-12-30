#define __LIBRARY__
#include <unistd.h>
#include <stdio.h>

_syscall1(int,share_memory_create,int,size)
_syscall1(int,share_memory_with,pid_t,pid)

unsigned long *p;
int main() {
	p = (unsigned long *)share_memory_create(1);
	printf("Created: %d\r\n", p);
	*p = 1;
	printf("Value: %d\r\n", *p);
	while(*p == 1);
	printf("Value Changed: %d\r\n", *p);
	return 0;
}