# CHI-Test 测试框架结构

CHI-Test 测试框架的大致结构如下图所示：
<img src="./images/CHI_test%E6%95%B4%E4%BD%93%E7%BB%93%E6%9E%84.svg" width="80%">

(1) Access Sequence
* Access Sequence 是访问序列，模拟 CPU 对 Cache 的读写请求序列。
* Access Sequence 支持 Read，Write-Allocate，Write-NoAllocacte 和 Flush 操作。

(2) Snoop Sequence
* Snoop Sequence 是 Snoop 访问序列
* Snoop Sequence 支持 Snoop-Read，Snoop-Write 和 Only-Snoop 操作。

(3) RN-F
* RN-F 内部有 Cache
* RN-F 可以接收和响应访问序列 Access Sequence。

(4) ICN
* 此处的 ICN 是 CHI 协议中的 ICN 和 SN 的结合体。
* ICN 内部有 Main Memory
* ICN 可以接收和响应 Snoop 访问序列 Snoop Sequence 
* ICN 可以接收和响应 RN-F 发来的 CHI 请求

总结：

通过设置访问序列 Access Sequence 间接的控制 RN-F 产生 CHI REQ 请求；

通过设置 Snoop 访问序列 Snoop Sequence 间接地控制 ICN 产生 CHI SNP 请求；


***

# CHI-Test 测试举例

(1) 访问序列举例

假如访问序列是读操作 Read，Read 的目的是读取数据。
Read 输入到 RN-F 中后，RN-F 需要检查 CacheLine 的状态。
如果 CacheLine 的状态为 Invalid，发生 Cache Miss，那么 RN-F 会向 ICN 发送 CHI 请求，如 ReadClean 请求，从 ICN 中读取数据。
如果 CacheLine 的状态为 UC，UD，SC, (RN-F 目前不支持 SD 状态)，发生 Cache Hit，那么就不需要发送 CHI 请求。
发生 Cache Hit 后，读取 RN-F 中 CacheLine 的数据，然后通知访问序列 Read 操作完成。

(2) Snoop 访问序列举例

假如 Snoop 访问序列是 Snoop-Write 操作，Snoop-Write 的目的是将数据写入到 ICN Main Memory 中。
Snoop-Write 输入到 ICN 中后，ICN 会通过 CHI 总线发送 Snoop 请求，如 SnpMakeInvalid 和 SnpCleanInvalid 请求，从 RN-F 中取回数据。然后再将 Snoop-Write 操作写的数据写入到 ICN Main Memory 中。

(3) 测试举例
* 创建访问序列 Access Sequence，一系列的读写请求，输入到 CHI-Test 测试框架 RN-F 中。
当访问序列都执行完成后，再创建 Flush 操作序列，将 RN-F 中的数据写回 ICN Main Memory 中。这是由于 RN-F 中可能含有 Dirty 数据，因此需要使用 Flush 操作将 Dirty 数据写回 ICN Main Memory 中。
* 另外，将上诉创建的访问序列直接输入到参考模型 Golden Memory 中执行。
* 由于访问序列是相同的，因此访问序列执行完后，ICN Main Memory 和 Golden Memory 的状态应该相同。故只需要比较两者最终状态是否相同就可以判断测试框架是否正确。

