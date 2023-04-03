import numpy

from QueueManager import QueueManager
import multiprocessing


class JobDistribiutor:

    def __init__(self, matrix: numpy.numarray):
        self.__matrix__ = matrix
        self.__queue_manager__ = QueueManager
        self.__connection__ = None
        self.__job_queue__ = None
        self.__result_queue__ = None

    def register(self, job_queue_name: str, job_queue_name_2, server_ip: str, port_num: int, auth_key: bytes):
        self.__queue_manager__.register(job_queue_name)
        self.__queue_manager__.register(job_queue_name_2)
        self.__connection__ = QueueManager(address=(server_ip, port_num), authkey=auth_key)
        self.__connection__.connect()
        self.__job_queue__ = self.__connection__.job_queue()
        self.__result_queue__ = self.__connection__.result_queue()
        self.distribiute_jobs_by_cpus()
        print("Done uploading data to queue")

    def distribiute_jobs_by_cpus(self):
        num_of_cpus = multiprocessing.cpu_count()
        num_of_rows_available = self.__matrix__.shape[0]
        job_to_worker_ratio = num_of_rows_available // num_of_cpus
        if num_of_cpus > num_of_rows_available:
            num_of_cpus = num_of_rows_available
        for i in range(0, num_of_cpus, job_to_worker_ratio):
            #print(i)
            print((self.__matrix__[i:i + job_to_worker_ratio], i, i + job_to_worker_ratio - 1))
            self.__job_queue__.put((numpy.asarray(self.__matrix__[i:i + job_to_worker_ratio]).tolist(), i, i + job_to_worker_ratio - 1))

    def distribiute_by_num_of_rows(self):
        num_of_rows_available = self.__matrix__.shape[0]
        for i in range(num_of_rows_available):
            self.__job_queue__.put(self.matrix[i])
        self.__job_queue__.join()

    def collect(self) -> numpy.numarray:
        result_vector = numpy.zeros((self.__matrix__.shape[0], 1))
        while not self.__result_queue__.empty():
            result = self.__result_queue__.get()
            for i in range(result[1], result[2]):
                result_vector[i] = result[0][i]
        return result_vector
