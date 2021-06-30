#coding=utf-8
import my_debugger
import psutil
import os
import my_debugger_defines

def find_tartget_pid(name):
    for i in psutil.process_iter():
        try:
            if i.name() == name:
                return i.pid
        except Exception:
            pass
    return 0

def main():
    debugger = my_debugger.debugger()
    # debugger.load(r'C:\Users\killa\Desktop\peTest.exe')
    #pid = raw_input('Enter the pid: ')
    pid = find_tartget_pid('demo.exe')
    debugger.attach(int(pid))
    try:
        #printf_address = debugger.func_resolve('msvcr90.dll','printf')
        printf_address = 0x6a3a7ef0
        va_a_address = 0x00417138
        #debugger.bp_set(printf_address)
        #debugger.bp_set_hw(printf_address, 1, my_debugger_defines.HW_EXECUTE)
        debugger.bp_set_mem(va_a_address, 4)
        debugger.run()
    # list_threads = debugger.enumerate_threads()
    #
    # for thread in list_threads:
    #     thread_context = debugger.get_thread_context(thread)
    #     print 'th32ThreadID: {}'.format(thread)
    #     print 'eax: 0x%08x' % thread_context.Eax
    #     print 'ebx: 0x%08x' % thread_context.Ebx
    #     print 'ecx: 0x%08x' % thread_context.Ecx
    #     print 'edx: 0x%08x' % thread_context.Edx
    #     print 'esp: 0x%08x' % thread_context.Esp
    finally:
        debugger.detach()

if __name__ == '__main__':
    main()