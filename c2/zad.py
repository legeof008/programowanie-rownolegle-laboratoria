import threading
import numpy as np
import math

def read_file(filename):
   with open(filename, 'r') as f:
      col = int(f.readline()[0])
      row = int(f.readline()[0])
      l = [[float(num) for num in line.split()] for line in f]
      matrix = np.array(l)     
   return matrix

def create_result_matrix(numOfCol,numOfRow):
   matrix = np.zeros((numOfCol,numOfRow),dtype=float)
   return matrix

def multiply_matrix(A,B):
   C = np.matmul(A,B)
   return C

def multiply(i,start_index, end_index):
   if(end_index > number_of_elements or i == number_of_threads-1):
      end_index = number_of_elements
   row_size = C.shape[1]
   for j in range(start_index, end_index):
      c = int(j/row_size)
      r = j-c*row_size
      res = sum(a*b for a,b in zip(A[c],B[:,r])) 
      C[c,r] = res
      lock.acquire()
      global sum_of_elements_in_C
      global frobenius_norm
      sum_of_elements_in_C = sum_of_elements_in_C + res
      frobenius_norm = frobenius_norm + pow(res,2)
      lock.release()

A = read_file("A.txt")
B = read_file("B.txt")
if  A.shape[1] != B.shape[0]:
   print("Sorry, cannot multiply A and B.")
   exit()
C = create_result_matrix(A.shape[0],B.shape[1])

sum_of_elements_in_C = 0
frobenius_norm = 0

lock = threading.Lock()

#Data to validate
target_matrix = multiply_matrix(A,B)
target_sum = np.sum(target_matrix)
#print("Target matrix\n"+ str(target_matrix))
print("Target sum: "+str(target_sum))


number_of_threads = 4
number_of_elements = A.shape[0]*B.shape[1]
ration = round(number_of_elements/number_of_threads)
 
threads = []

#Create threads
for i in range(number_of_threads):
   thread = threading.Thread(target=multiply,args=(i,i*ration,(i+1)*ration))
   thread.start()
   threads.append(thread)

#Wait for end of each thread
for t in threads:
   t.join()

frobenius_norm = math.sqrt(frobenius_norm)

print("Result matrix:\n"+str(C))
print("Sum of elements: "+str(sum_of_elements_in_C))
print("Frobenius norm: "+str(frobenius_norm))
