import numpy

from QueueManager import QueueManager


class Worker:
    def __init__(self):
        self.__queue_manager__ = QueueManager
        self.__queue_manager__.register('job_queue')
        self.__queue_manager__.register('result_queue')
        self.__connection__ = None
        self.__job_queue__ = None
        self.__result_queue__ = None

    def register(self, ip, port, auth_key):
        print("init")
        self.__connection__ = QueueManager(address=(ip, port), authkey=auth_key)
        print("init")
        self.__connection__.connect()
        print("init")
        self.__job_queue__ = self.__connection__.job_queue()
        print("init")
        self.__result_queue__ = self.__connection__.result_queue()

    def do_work(self):
        print("Working ....")
        print(self.__job_queue__.empty())
        if self.__job_queue__.empty == True:
            print("The queue is empty")
            return
        data = self.__job_queue__.get()
        self.__job_queue__.task_done()
        print(data)

        rows = numpy.asarray(data[0])
        vector = numpy.asarray(data[3])
        start_row = data[1]
        end_row = data[2]
        results = []
        if start_row == end_row:
            result = numpy.asarray(rows).dot(vector)[0]
            results.append(result)
            print("Result is ", result)
        else:
            for i in range(end_row - start_row + 1):
                result = numpy.asarray(rows[i]).dot(vector)[0]
                results.append(result)
                print("Result is ", result)
        self.__result_queue__.put((results, start_row, end_row))
        self.__result_queue__.task_done()


if __name__ == '__main__':
    worker = Worker()
    worker.register('127.0.0.1', 5000, b'blah')
    worker.do_work()
