import numpy

from QueueManager import QueueManager
import multiprocessing
from queue import Queue


class JobDistribiutor:

    def __init__(self, matrix: numpy.numarray, vector: numpy.numarray):
        self.__matrix__ = matrix
        self.__common_vector__ = vector
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
        self.distribiute_by_num_of_rows()
        print("Done uploading data to queue")
        print(self.collect())

    def distribiute_jobs_by_cpus(self):
        num_of_cpus = multiprocessing.cpu_count()
        num_of_rows_available = self.__matrix__.shape[0]
        job_to_worker_ratio = num_of_rows_available // num_of_cpus
        print("Ratio is ", job_to_worker_ratio)
        if num_of_cpus > num_of_rows_available:
            num_of_cpus = num_of_rows_available
        for i in range(0, num_of_rows_available, job_to_worker_ratio):
            print((self.__matrix__[i:i + job_to_worker_ratio], i, i + job_to_worker_ratio - 1))
            self.__job_queue__.put(
                (numpy.asarray(self.__matrix__[i:i + job_to_worker_ratio]).tolist(), i,
                 i + len(self.__matrix__[i:i + job_to_worker_ratio]) - 1,
                 self.__common_vector__))
            print("Has been put in")
        # print(self.__job_queue__)

    def distribiute_by_num_of_rows(self):
        num_of_rows_available = self.__matrix__.shape[0]
        for i in range(num_of_rows_available):
            self.__job_queue__.put((numpy.asarray(self.__matrix__[i]).tolist(), i, i, self.__common_vector__))

    def collect(self) -> numpy.numarray:
        print("Waiting to collect ...")
        self.__job_queue__.join()
        result_vector = numpy.zeros((self.__matrix__.shape[0], 1))
        j = 0
        while not self.__result_queue__.empty() and j < self.__matrix__.shape[0]:
            result = self.__result_queue__.get()
            print(result[0])
            if result[1] == result[2]:
                result_vector[result[1]] = result[0][0]
            for i in range(len(result[0])):
                result_vector[j] = result[0][i]
                j += 1
        return result_vector


if __name__ == '__main__':
    dist = JobDistribiutor(numpy.random.rand(40, 40), numpy.random.rand(40, 1))
    dist.register("job_queue", "result_queue", "127.0.0.1", 5000, b'blah')
