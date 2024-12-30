#define __LIBRARY__
#include <unistd.h>
#include <stdio.h>

_syscall1(int,share_memory_create,int,size)
_syscall1(int,share_memory_with,pid_t,pid)

char *p, *q;

int to_int(const char *str) {
	int value = 0;
	char *p = str;
	while (*p != '\0') {
		value = value * 10 + *p++ - '0';
	}
	return value;
}

int main(int argc, char * argv[]) {
	int sid;
   
	if (argc < 3) 
		printf("error");
	q = argv[2];
	sid = to_int(argv[1]);
	if ((p = (char *)share_memory_with(sid)) != -1) {
		while (*q != '\0') {
			*p++ = *q++;
		} 
	}
	printf("Send complete.\r\n");
	return 0;
}