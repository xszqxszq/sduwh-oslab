import sys

if len(sys.argv) <= 1:
	print('Usage: python analyze_schedule.py path/to/schedule.csv')
	exit(1)

with open(sys.argv[1], 'r') as f:
	lines = [i.split(',') for i in f.read().strip().split('\n')]

processes = {}

for pid, status, time in lines:
	time = int(time)
	if pid not in processes:
		processes[pid] = {
			'previous': None,
			'wait': 0,
			'lastReady': time,
			'createdTime': time,
			'lastTime': time
		}

	now = processes[pid]
	if now['previous'] == 'READY':
		now['wait'] += time - now['lastReady']
	if status == 'READY':
		now['lastReady'] = time
	now['previous'] = status
	now['lastTime'] = time

total = len(processes)
lastTime = max([processes[i]['lastTime'] for i in processes])
totalWait = sum([now['wait'] for pid, now in processes.items()])
totalFinish = sum([now['lastTime'] - now['createdTime'] for pid, now in processes.items()])

print('平均等待时间: {:.2f} ms'.format(totalWait * 10 / total))
print('平均完成时间: {:.2f} ms'.format(totalFinish * 10 / total))
print('平均吞吐量: {:.2f}/s'.format(total * 100 / lastTime))