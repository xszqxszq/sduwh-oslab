#define __LIBRARY__
#include <unistd.h>
#define BUFSIZE 1024

_syscall1(int,iam,const char*,name)

int main(int argc, char *argv[]) {
	if (argc < 2) {
		return -1;
	}
	iam(argv[1]);
	return 0;
}