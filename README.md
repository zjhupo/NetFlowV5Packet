NetFlowV5Packet
===============

NetFlow是一种数据交换方式，其工作原理是：NetFlow利用标准的交换模式处理数据流的第一个IP包数据，生成NetFlow 缓存，随后同样的数据基于缓存信息在同一个数据流中进行传输，不再匹配相关的访问控制等策略，NetFlow缓存同时包含了随后数据流的统计信息。通过分析该报文，我们可以实现用户网络行为审计的功能。
