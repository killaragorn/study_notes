from ctypes import *
import  time

msvcrt = cdll.msvcrt
count = 0

while   1:
    msvcrt.printf("Loop iteration {} \n".format(count))
    time.sleep(2)
    count += 1
