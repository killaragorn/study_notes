# coding=utf-8
from my_debugger_defines import *

kernel32 = windll.kernel32


class debugger():
    def __init__(self):
        self.h_process = None
        self.pid = None
        self.debugger_active = False
        self.h_thread = None
        self.context = None
        self.breakpoints = {}
        self.first_breakpoint = True
        self.hardware_breakpoints = {}
        system_info = SYSTEM_INFO()
        kernel32.GetSystemInfo(byref(system_info))
        self.page_size = system_info.dwPageSize
        self.guarded_pages = []
        self.memory_breakpoints = {}

    # 开启一个调试进程
    def load(self, path_to_exe):

        creation_flags = DEBUG_PROCESS

        startupinfo = STARTUPINFO()
        process_information = PROCESS_INFORMATION()

        startupinfo.cb = sizeof(startupinfo)
        # 下面的设置让新进程在一个单独窗体中被显示
        startupinfo.dwFlags = 0x1
        startupinfo.wShowWindow = 0x0

        if kernel32.CreateProcessA(path_to_exe,
                                   None,
                                   None,
                                   None,
                                   None,
                                   creation_flags,
                                   None,
                                   None,
                                   byref(startupinfo),
                                   byref(process_information)):
            print '[*] 创建进程成功'
            print '[*] PID : {}'.format(process_information.dwProcessId)

            self.h_process = self.open_process(process_information.dwProcessId)

        else:
            print '[*] 创建进程错误: {}'.format(kernel32.GetLastError())

    # 获取一个进程的句柄
    def open_process(self, pid):
        h_process = kernel32.OpenProcess(PROCESS_ALL_ACCESS, False, pid)
        return h_process

    # 根据pid附加到进程
    def attach(self, pid):
        self.h_process = self.open_process(pid)

        if kernel32.DebugActiveProcess(pid):
            self.debugger_active = True
            self.pid = pid
            print '[*] 附加进程 {} 成功.'.format(pid)
            #self.run()
        else:
            print '[*] 不能够附加到进程 {} 上.'.format(pid)

    # 调试主循环
    def run(self):
        while self.debugger_active == True:
            self.get_debug_event()

    # 获取调试事件
    def get_debug_event(self):
        debug_event = DEBUG_EVENT()
        continue_status = DBG_CONTINUE

        if kernel32.WaitForDebugEvent(byref(debug_event), INFINITE):
            # raw_input('press a key to continue....')
            #self.debugger_active = False

            self.h_thread = self.open_thread(debug_event.dwThreadId)
            self.context = self.get_thread_context(debug_event.dwThreadId)

            print '事件ID：{} 线程ID：{}'.format(debug_event.dwDebugEventCode, debug_event.dwThreadId)

            if  debug_event.dwDebugEventCode == EXCEPTION_DEBUG_EVENT:
                exception_code = debug_event.u.Exception.ExceptionRecord.ExceptionCode
                self.exception_address = debug_event.u.Exception.ExceptionRecord.ExceptionAddress
                continue_status = DBG_CONTINUE

                if exception_code == EXCEPTION_ACCESS_VIOLATION:
                    print '[--] 产生页面访问错误'
                elif exception_code == EXCEPTION_BREAKPOINT:
                    print '[--] 产生软件断点异常'
                    if self.first_breakpoint == True:
                        self.first_breakpoint = False
                        continue_status = DBG_CONTINUE
                        print '[--] 系统断点击中，已放行'
                    else:
                        continue_status = self.exception_handler_breakpoint()
                elif exception_code == EXCEPTION_GUARD_PAGE:
                    print '[--] 产生保护页错误'
                    print '[--] 异常地址：0x%08x' % self.exception_address
                elif exception_code == EXCEPTION_SINGLE_STEP:
                    continue_status = self.exception_handler_single_step()


            kernel32.ContinueDebugEvent(debug_event.dwProcessId, debug_event.dwThreadId, continue_status)

    # 断点异常的处理例程
    def exception_handler_breakpoint(self):
        print '[--] 断点处理例程：'
        print '[--] 异常地址：0x%08x' % self.exception_address

        if  self.breakpoints.has_key(self.exception_address):
            print '[--] 用户设置软件断点触发: 0x%08x' % self.exception_address
            raw_input("wait...")

        return  DBG_CONTINUE

    # 硬件断点的异常处理例程
    def exception_handler_single_step(self):
        print '[--] 硬件断点处理例程：'
        print '[--] 异常地址：0x%08x' % self.exception_address

        if  self.context.Dr6 & 0x1  and self.hardware_breakpoints.has_key(0):
            slot = 0
        elif self.context.Dr6 & 0x2  and self.hardware_breakpoints.has_key(1):
            slot = 1
        elif self.context.Dr6 & 0x3  and self.hardware_breakpoints.has_key(2):
            slot = 2
        elif self.context.Dr6 & 0x4  and self.hardware_breakpoints.has_key(3):
            slot = 3
        else:
            continue_status = DBG_EXCEPTION_HANDLED

        if  self.bp_del_hw(slot):
            continue_status = DBG_CONTINUE
            print '[--] 硬件断点已移除'

        return  continue_status

    # 移除第N号硬件断点
    def bp_del_hw(self,slot):
        for thread_id in self.enumerate_threads():
            context = self.get_thread_context(thread_id)

            context.Dr7 &= ~(1 << (slot * 2))
            if  slot == 0:
                context.Dr0 = 0x00000000
            elif slot == 1:
                context.Dr1 = 0x00000000
            elif slot == 2:
                context.Dr2 = 0x00000000
            elif slot == 3:
                context.Dr3 = 0x00000000

            context.Dr7 &= ~(3 << ((slot * 4) + 16))
            context.Dr7 &= ~(3 << ((slot * 4) + 18))
            h_thread = self.open_thread(thread_id)
            kernel32.SetThreadContext(h_thread, byref(context))
        del self.hardware_breakpoints[slot]
        return True

    # 取消附加
    def detach(self):
        if kernel32.DebugActiveProcessStop(self.pid):
            print '[*] 结束附加，退出中。。。'
            return True
        else:
            print '[*] 结束附加时产生错误 : {}'.format(kernel32.GetLastError())
            return False

    # 打开线程，返回线程句柄
    def open_thread(self, thread_id):
        h_thread = kernel32.OpenThread(THREAD_ALL_ACCESS, False, thread_id)

        if h_thread is not None:
            return h_thread
        else:
            print '[*] 打开线程 {} 失败.'.format(thread_id)
            return False

    # 遍历被附加进程的所有线程，返回一个列表
    def enumerate_threads(self):
        thread_entry = THREADENTRY32()
        thread_list = []
        snapshot = kernel32.CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, self.pid)

        if snapshot is not None:
            thread_entry.dwSize = sizeof(THREADENTRY32)
            success = kernel32.Thread32First(snapshot, byref(thread_entry))

            while success:
                if thread_entry.th32OwnerProcessID == self.pid:
                    thread_list.append(thread_entry.th32ThreadID)
                success = kernel32.Thread32Next(snapshot, byref(thread_entry))

            kernel32.CloseHandle(snapshot)
            return thread_list
        else:
            return False

    # 获取线程的线程环境块
    def get_thread_context(self, thread_id=None, h_thread=None):
        context = CONTEXT()
        context.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS

        # 获取线程句柄
        if  h_thread is None:
            h_thread = self.open_thread(thread_id)
        if kernel32.GetThreadContext(h_thread, byref(context)):
            kernel32.CloseHandle(h_thread)
            return context
        else:
            return False

    # 读取进程内存
    def read_process_mem(self, address, length):
        data = ""
        read_buf = create_string_buffer(length)
        count = c_ulong(0)

        if not kernel32.ReadProcessMemory(self.h_process, address, read_buf, length, byref(count)):
            return False
        else:
            data += read_buf.raw
            return data

    # 写入进程内存
    def write_process_mem(self, address, data):
        count = c_long(0)
        length = len(data)

        c_data = c_char_p(data[0:])

        if  not kernel32.WriteProcessMemory(self.h_process, address, c_data, length, byref(count)):
            return False
        else:
            return True

    # 设置断点
    def bp_set(self,address):
        if not self.breakpoints.has_key(address):
            try:
                original_byte = self.read_process_mem(address,1)
                self.write_process_mem(address, '\xcc')
                self.breakpoints[address] = (address, original_byte)

                print '设置软件断点 0x%08x 成功' % address
            except:
                return False
        return True

    # 设置硬件断点
    def bp_set_hw(self,address, length, condition):
        if length not in (1,2,4):
            return False
        else:
            length -= 1

        if  condition not in (HW_WRITE, HW_ACCESS, HW_EXECUTE):
            return False

        if not self.hardware_breakpoints.has_key(0):
            availble = 0
        elif not self.hardware_breakpoints.has_key(1):
            availble = 1
        elif not self.hardware_breakpoints.has_key(2):
            availble = 2
        elif not self.hardware_breakpoints.has_key(3):
            availble = 3
        else:
            return False

        for thread_id in self.enumerate_threads():
            context = self.get_thread_context(thread_id)

            context.Dr7 |= 1 << (availble * 2)
            if  availble == 0:
                context.Dr0 = address
            elif availble == 1:
                context.Dr1 = address
            elif availble == 2:
                context.Dr2 = address
            elif availble == 3:
                context.Dr3 = address

            context.Dr7 |= condition << ((availble * 4) + 16)
            context.Dr7 |= length << ((availble * 4) + 18)

            h_thread = self.open_thread(thread_id)
            kernel32.SetThreadContext(h_thread, byref(context))

        self.hardware_breakpoints[availble] = (address, length, condition)
        return True

    # 设置内存断点：此方式使用的是保护页机制
    def bp_set_mem(self, address, size):
        mbi = MEMORY_BASIC_INFORMATION()

        if  kernel32.VirtualQueryEx(self.h_process, address, byref(mbi), sizeof(mbi)) < sizeof(mbi):
            return False
        current_page = mbi.BaseAddress

        while current_page <= address + size:
            self.guarded_pages.append(current_page)

            old_protection = c_ulong(0)
            if not kernel32.VirtualProtectEx(self.h_process, current_page, size, mbi.Protect | PAGE_GUARD, byref(old_protection)):
                return False
            current_page += self.page_size
        self.memory_breakpoints[address] = (address, size, mbi)
        return True

    # 获取一个模块的导出函数的地址
    def func_resolve(self,dllname,function_name):
        handle = kernel32.GetModuleHandleA(dllname)
        address = kernel32.GetProcAddress(handle, function_name)

        return address


