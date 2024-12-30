#define __LIBRARY__
#include <unistd.h>
#include <stdio.h>

_syscall1(int,share_memory_create,int,size)
_syscall1(int,share_memory_with,pid_t,pid)

void clear(char *buf, int size) {
	char *p = buf;
	while (p - buf < size) {
		*p++ = '\0';
	}
}

char *base, *p; 
int main(int argc, char * argv[]) {
	if (!strcmp(argv[1], "startup")) {
		printf("------------------The Server Processor is Startup---------------\r\n");
		printf("The server processor's id=%d\r\n", getpid());
		base = (char *)share_memory_create(1024);
		clear(base, 1024);
		p = base;
		while (1) {
			printf("The server will sleep 20 seconds\r\n");
			sleep(60);
			printf("\r\nReceived: ");
			while (*p != '\0') {
				printf("%c", *p++);
			}
			printf("\r\n");
		}
	}
	return 0;
}