#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#define __LIBRARY__
#include <unistd.h>

_syscall1(int,sem_create,char*,name)
_syscall2(int,sem_set,int,id,int,value)
_syscall1(int,sem_wait,int,id)
_syscall1(int,sem_signal,int,id)
_syscall0(int,get_item)
_syscall1(int,put_item,int,item)
#define NR_BUFFERS 3
#define NR_Produces 10
#define NR_Consumes 10

int mutex, empty, full;

void produce(int produces) {
	int i;
	for (i = 0; i < produces; i ++) {
		sem_wait(empty);
		sem_wait(mutex);
		put_item(i);
		sem_signal(mutex);
		sem_signal(full);
	}
}

void consume(int consumes) {
	int i, item;
	for (i = 0; i < consumes; i ++) {
		sem_wait(full);
		sem_wait(mutex);
		item = get_item();
		sem_signal(mutex);
		sem_signal(empty);
	}
}


int main() {
	int pid;

	printf("---------------This is a Demo of Producer-Consumer problem-------------------\n");
	printf("The number of Producer-Consumer buffers is: %d\n", NR_BUFFERS);

	mutex = sem_create("PCMutex");
	sem_set(mutex, 1);
	empty = sem_create("PCEmpty");
	sem_set(empty, NR_BUFFERS);
	full = sem_create("PCFull");
	sem_set(full, 0); 
	if ((pid = fork()) == 0) {
		printf("The number of Producers is: %d\n", NR_Produces);
		produce(NR_Produces);
	} else {
		printf("The now_id of Producer is: %d\n", pid);
		if ((pid = fork()) == 0) {
			printf("The number of Consumers is: %d\n", NR_Consumes);
			consume(NR_Consumes);
		} else {
			printf("The now_id of Consumer is: %d\n", pid);
		}
	}
	exit(0);
}