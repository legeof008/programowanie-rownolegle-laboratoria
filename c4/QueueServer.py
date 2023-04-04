from queue import Queue
from QueueManager import QueueManager
import multiprocessing
import numpy


class Server:
    def __init__(self, ip, port):
        self.__result_queue__ = Queue()
        self.__job_queue__ = Queue()
        QueueManager.register('result_queue', callable=lambda: self.__result_queue__)
        QueueManager.register('job_queue', callable=lambda: self.__job_queue__)
        self.__manager__ = QueueManager(address=(ip, port), authkey=b'blah')
        self.__server__ = None

    def start(self):
        print("The server has started")
        self.__server__ = self.__manager__.get_server()
        self.__server__.serve_forever()


if __name__ == '__main__':
    server = Server("127.0.0.1", 5000)
    server.start()
