#define __LIBRARY__
#include <unistd.h>
#include <stdio.h>

_syscall1(int,share_memory_create,int,size)
_syscall1(int,share_memory_with,pid_t,pid)

int to_int(const char *str) {
	int value = 0;
	char *p = str;
	while (*p != '\0') {
		value = value * 10 + *p++ - '0';
	}
	return value;
}

unsigned long *addr;
int main(int argc, char *argv[]) {
	int pid;
	pid = to_int(argv[1]);

	addr = (unsigned long *)share_memory_with(pid);
	*addr = 2;
	printf("Value: %d\r\n", *addr);
	return 0;
}