/*
 *  linux/kernel/printk.c
 *
 *  (C) 1991  Linus Torvalds
 */

/*
 * When in kernel-mode, we cannot use printf, as fs is liable to
 * point to 'interesting' things. Make a printf with fs-saving, and
 * all is well.
 */

#include <fcntl.h>
#include <errno.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>

#include <linux/kernel.h>
#include <linux/sched.h>

static char buf[1024];

extern int vsprintf(char * buf, const char * fmt, va_list args);

int printk(const char *fmt, ...)
{
	va_list args;
	int i;

	va_start(args, fmt);
	i=vsprintf(buf,fmt,args);
	va_end(args);
	__asm__("push %%fs\n\t"
		"push %%ds\n\t"
		"pop %%fs\n\t"
		"pushl %0\n\t"
		"pushl $buf\n\t"
		"pushl $0\n\t"
		"call tty_write\n\t"
		"addl $8,%%esp\n\t"
		"popl %0\n\t"
		"pop %%fs"
		::"r" (i):"ax","cx","dx");
	return i;
}

static char nowBuf[1024];

int printl(const char *fmt, ...)
{
	va_list args;
	int i;

	va_start(args, fmt);
	i=vsprintf(nowBuf, fmt, args);
	va_end(args);

	struct file * file;
	struct m_inode * inode;
	if (!(file=task[0]->filp[3]))
		return 0;
	inode=file->f_inode;
	__asm__("push %%fs\n\t"
		"push %%ds\n\t"
		"pop %%fs\n\t"
		"pushl %0\n\t"
		"pushl $nowBuf\n\t"
		"pushl %1\n\t"
		"pushl %2\n\t"
		"call file_write\n\t"
		"addl $12,%%esp\n\t"				// pushed 3 arguments, therefore 12, otherwise system keeps rebooting
		"popl %0\n\t"
		"pop %%fs"
		::"r" (i), "r"(file), "r"(inode):"ax","cx","dx");
	// file_write(inode, file, nowBuf, i);	// this method will mess up the log file
	// write(3, nowBuf, i);					// direct call of file_write can cause kernel panic
	return i;
}

static char nowBuf[1024];

int print_csv(const char *fmt, ...)
{
	va_list args;
	int i;

	va_start(args, fmt);
	i=vsprintf(nowBuf, fmt, args);
	va_end(args);

	struct file * file;
	struct m_inode * inode;
	if (!(file=task[0]->filp[4]))
		return 0;
	inode=file->f_inode;
	__asm__("push %%fs\n\t"
		"push %%ds\n\t"
		"pop %%fs\n\t"
		"pushl %0\n\t"
		"pushl $nowBuf\n\t"
		"pushl %1\n\t"
		"pushl %2\n\t"
		"call file_write\n\t"
		"addl $12,%%esp\n\t"				// pushed 3 arguments, therefore 12, otherwise system keeps rebooting
		"popl %0\n\t"
		"pop %%fs"
		::"r" (i), "r"(file), "r"(inode):"ax","cx","dx");
	// file_write(inode, file, nowBuf, i);	// this method will mess up the log file
	// write(3, nowBuf, i);					// direct call of file_write can cause kernel panic
	return i;
}