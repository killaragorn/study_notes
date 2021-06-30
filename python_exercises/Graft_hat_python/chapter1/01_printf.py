from ctypes import *
msvcrt = cdll.msvcrt
message_string = "hello world\n"
# 原书使用printf打印，只会输出一个T，猜测是编码问题s
msvcrt.wprintf("Testing string : %s", message_string)