/*
 *  kernel/share.c
 *
 *  (C) 2024  XSZQ
 */

#include <linux/mm.h>
#include <linux/sched.h>

int sys_share_memory_create(int size) {
	int tmp;
	if (size > PAGE_SIZE)
		return -1;
	tmp = get_free_page();
	put_page(tmp, current->start_code + current->brk);
	current->share_page = tmp;
	return current->brk;
}

int sys_share_memory_with(pid_t pid) {
	struct task_struct **p, **now;

	for (p = &LAST_TASK; p > &FIRST_TASK; --p)
		if (*p) {
			if ((*p)->pid == pid) {
				now = p;
				break;
			}
		}

	while ((*now)->share_page == 0);
	put_page((*now)->share_page, current->start_code + current->brk);
	return current->brk;
}