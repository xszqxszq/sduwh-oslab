/*
 *  linux/kernel/semaphore.c
 *
 *  (C) 2024  XSZQ
 */
#include <errno.h>
#include <string.h>
#include <linux/sched.h>
#include <asm/segment.h>
#include <asm/system.h>

#define MAX_SEMAPHORE 30
struct semaphore
{
	char name[20];
	int used;
	int value;
	struct task_struct *queue[20], **head, **tail;
} sems[MAX_SEMAPHORE];

void enqueue(int id, struct task_struct *task) {
	*sems[id].tail++ = task;
}
struct task_struct *dequeue(int id) {
	if (sems[id].head == sems[id].tail)
		return NULL;
	return *sems[id].head++;
}
int get_fs_string(char *buf, const char *str, int size) {
    char c, *p = buf;
    while ((c = get_fs_byte(str++)) != '\0') {
        if (p - buf >= size)
            return -EINVAL;
        *p++ = c;
    }
    *p = '\0';
    return p - buf;
}
void sem_init() { 
	int i;
	for(i = 0; i < MAX_SEMAPHORE; i ++) {
		memset(sems[i].name, 0, sizeof(sems[i].name));
		sems[i].used = 0;
		sems[i].value = 0;
		sems[i].head = sems[i].tail = sems[i].queue;
	}
}
int sem_find(char *name) {
	int i;
	for (i = 0; i < MAX_SEMAPHORE; i ++) {
		if (sems[i].used && strcmp(name, sems[i].name) == 0) {
			return i;
		}
	}
	return -1;
}
int sys_sem_create(char *name) {
	char buf[20];
	get_fs_string(buf, name, 20);
	int i;
	if ((i = sem_find(buf)) != -1)
		return i;
	for (i = 0; i < MAX_SEMAPHORE; i ++) {
		if (!sems[i].used) {
			strcpy(sems[i].name, buf);
			sems[i].used = 1;
			return i;
		}
	}
	return -1;
}
int sys_sem_set(int id, int value) {
	sems[id].value = value;
	return 0;
}
int sys_sem_wait(int id) {
	cli();
	sems[id].value --;
	sti();
	if (sems[id].value < 0) {
		printk("The process with ID=%d SLEEPON the semaphore with name=%s\n", current->pid, sems[id].name);
		enqueue(id, current);
		current->state = TASK_UNINTERRUPTIBLE;
		schedule();
	}
	printk("The process with ID=%d GET the semaphore with name=%s\n", current->pid, sems[id].name);
	return 0;
}
int sys_sem_signal(int id) {
	struct task_struct *p;
	cli();
	sems[id].value ++;
	sti();
	if (sems[id].value <= 0) {
		p = dequeue(id);
		if (p != NULL) {
			printk("The process with ID=%d WAKEUP the process with ID=%d on the semaphore with name=%s\n", 
				current->pid, p->pid, sems[id].name);
			p->state = TASK_RUNNING;
			schedule();
		}
	}
	printk("The process with ID=%d PUT the semaphore with name=%s\n", current->pid, sems[id].name);
	return 0;
}

int items[3], *now = items;
int sys_get_item() {
	printk("The process with ID=%d CONSUME a item=%d from PCBuffers[%d]\n", current->pid, *now, now - items);
	return *now--;
}

void sys_put_item(int item) {
	printk("The process with ID=%d PRODUCE a item=%d from PCBuffers[%d]\n", current->pid, *now, now - items);
	*now++ = item;
}