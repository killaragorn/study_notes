### 为什么在 DISPATCH_LEVEL 级别上不能访问分页内存以及进行 timeout != 0的等待？

#### 前置知识

1. IRQ：中断请求，当中断发生后，发生中断的设备通过它使用的中断请求信号线向中断请求控制器发出中断信号，cpu可以通过IRQ号来识别中断。
2. Interrupt Vector: 当cpu检测到一个中断请求后，
3. IRQL:

#### 为什么不能访问分页内存？

- 首先，windows 缺页异常处理程序是运行在 APC_LEVEL 级别上的，如果在 DISPATCH_LEVEL 级别上访问分页内存，就有可能触发缺页异常处理程序，

#### 单处理器同步

- 单处理器上只需要提升 irql 即可实现关键数据的原子性访问，因为线程调度器运行在 DISPATCH_LEVEL 级别上，提升了 irql 之后，线程就不会被调度了

#### 多处理器同步

- 使用 Spin Lock 
  - 当一个核心申请到自旋锁之后，其他核心如果也想获取自旋锁，其他核心会一直空转，尝试获取，直到第一个核心释放自旋锁。
  - 注意，一个已拥有当前自旋锁的核心，不能再次获取这个自旋锁，否则当前cpu会死锁
- 

#### 参考链接

[0_Understanding Irql](http://ext2fsd.sourceforge.net/documents/irql.htm)

[1_What is Irql and Why is important?](https://techcommunity.microsoft.com/t5/ask-the-performance-team/what-is-irql-and-why-is-it-important/ba-p/372666)

[2_从irq到irql](https://blog.csdn.net/x313695373/article/details/7009698)

[3_分析驱动程序在IRQL>=DISPATCH_LEVEL时和DPC过程中不能用KeWaitForSingleObject等待对象的原因](https://blog.csdn.net/lixiangminghate/article/details/51063394)

[4_分页内存在DISPATCH_LEVEL 上蓝屏问题](https://blog.csdn.net/weixin_33929309/article/details/86193857)

https://arstechnica.com/civis/viewtopic.php?f=17&t=337381

https://blog.csdn.net/lixiangminghate/article/details/51012983

https://blog.csdn.net/lixiangminghate/article/details/51062889

https://community.osr.com/discussion/127882/irq-irql-intel-docs-and-windows

https://titanwolf.org/Network/Articles/Article?AID=b3d78a2f-953e-47be-8e64-ce9042f8197a#gsc.tab=0

