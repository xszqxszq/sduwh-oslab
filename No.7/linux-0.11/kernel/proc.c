/*
 *  linux/kernel/proc.c
 *
 *  (C) 2024  XSZQ
 */
#define __LIBRARY__
#include <unistd.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <asm/segment.h>

#define BUF_MAX 1024

extern int sprintf();

char proc_buf[BUF_MAX];
char path_buf[BUF_MAX];

void proc_init() {
	_syscall2(int,mkdir,const char*,_path,mode_t,mode)
	_syscall3(int,mknod,const char*,filename,mode_t,mode,int,dev)
	mkdir("/proc", 0755);
	mknod("/proc/psinfo", S_IFPROC | 0444, 0);
}

int put_fs_string(char *buf, const char *s, int count) {
    char *p = s, *q = buf;
    while (*p != '\0' && q - buf < count && p - s < BUF_MAX) {
        put_fs_byte(*p++, q++);
    }
    return q - buf;
}

int proc_read(int dev, char * buf, int count, unsigned long * pos) {
	int offset, length;
	struct task_struct **p;

	offset = 0;
	offset += sprintf(proc_buf + offset, "Processes List:\n");
	offset += sprintf(proc_buf + offset, "pid\texec\tstate\tuid\tfather\tstart_time\t\n");

	for (p = &LAST_TASK ; p > &FIRST_TASK ; --p) {
		if ((*p)) {
			offset += sprintf(proc_buf + offset, 
				"%d\t%s\t%d\t%d\t%d\t%d\t\n", 
				(*p)->pid, (*p)->exec, (*p)->state, (*p)->uid, (*p)->father, (*p)->start_time);
		}
	}

	length = put_fs_string(buf + *pos, proc_buf + *pos, count);
	*pos += length;
	return length;
}