from QueueManager import QueueManager
QueueManager.register('job_queue')
QueueManager.register('result_queue')
m = QueueManager(address=('', 5000), authkey= b'blah')
m.connect()
queue_r = m.result_queue()
queue_j = m.job_queue()
print(queue_j.get())
