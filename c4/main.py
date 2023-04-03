from JobDistribiutor import JobDistribiutor
import numpy as np

dist = JobDistribiutor(np.random.rand(16, 16))
dist.register("job_queue", "result_queue", "127.0.0.1", 5000, b'blah')
dist.distribiute_jobs_by_cpus()
