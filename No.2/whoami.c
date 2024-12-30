#define __LIBRARY__
#include <unistd.h>
#define BUFSIZE 1024

_syscall2(int,whoami,char*,name,unsigned int,size)

char buf[BUFSIZE];
int main() {
	whoami(buf, BUFSIZE);
	printf("%s", buf);
	return 0;
}