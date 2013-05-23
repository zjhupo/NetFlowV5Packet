//---------------------------------------------------------------------------

/**
    一个V5格式的包为  报文头+报文体。
    报文头长度=24
    报文体长度=48
    V5规定最多为30个报文体，因此最大长度为48*30+24=1464
*/
#ifndef NetFlowV5H
#define NetFlowV5H
//---------------------------------------------------------------------------
#include <vcl.h>
#include <winsock.h>
#include "jsSocket.h"

#define NETFLOW_LEN_HEADER 24
#define NETFLOW_LEN_RECORD 48


//报文头部长度为24
typedef struct tagNetFlow5_Header{
    INT2 _Version;     //Type of record format (5=NetFlow-5)
    INT2 _Count;       //Number of flow records contained in the UDP packet (maximum of 30)
    INT4 _SysUpTime;   //Value of SysUpTime ) when the UDP packet was sent (measured in milliseconds); SysUpTime measures how long the system has been up and running
                        //从路由器启动开始,到当前的以毫秒计的时间
    INT4 _EpochSeconds;//Number of seconds between the epoch (January 1, 1970) and the time when the UDP packet was sent. (Epoch is a date used as the "beginning of time" for timestamps. Time values in Unix systems are represented as the number of seconds since the epoch.)
                        //从0000UTC1970开始到当前的秒数
    INT4 _Nanoseconds; //Residual nanoseconds (billionth of a second) after the epoch second
                        //从0000UTC1970开始到当前残余的毫秒数
    INT4 _FlowsSeen;   //Number of flows seen since PacketWise began emitting flow detail records
                       //信息流的顺序号
                       //这个序列号等于前一个序列号加上刚刚过去的数据报文中信息流的个数
    INT1 _EngineType;  //User-configurable value (0-255) assigned to the Packeteer unit that is emitting flow detail records. Use the setup flowrecords engineType command to assign a type to the unit.
    INT1 _EngineID;    //User-configurable value (0-255) assigned to the Packeteer unit that is emitting flow detail records. Use the setup flowrecords engineID command to assign an ID to the unit.
    INT2 _SamplingInfo;//n/a
}NETFLOW5_HEADER,*PNETFLOW5_HEADER;

typedef struct tagNetFlow5_Record{
    INT4 _SourceIp;
    INT4 _DestIp;
    INT4 _NextRouterIp; //n/a
    INT2 _InboundSnmpIFindex;//SNMP index number that identifies the Inbound interface on the Packeteer unit:
                             //1 Inside (built-in)
                             //2 Outside (built-in)
                             //3 Upper_Inside (upper LEM)
                             //4 Upper_Outside (upper LEM)
                             //5 Lower_Inside (lower LEM)
                             //6 Lower_Outside (lower LEM)
                             //输入和输入 出接口的SNMP索引
    INT2 _OutboundSnmpIFindex;//同上
    //当前包为统计了多少个报文
    INT4 _PacketCount;        //Number of packets in the flow
                             //信息流的数据包
    //当前包为统计了多个报文的总流量
    INT4 _ByteCount;         //Total number of bytes in the flow
                             //在信息流的数据中第3层字节的总数
    INT4 _StartTime;         //Value of SysUpTime when the first packet in the flow was seen (measured in milliseconds)
    INT4 _EndTime;           //Value of SysUpTime when the last packet in the flow was seen (measured in milliseconds)
    INT2 _SourcePort;        //Port number of the device that the flow went out of
    INT2 _DestPort;          //Port number of the device that the flow went to
    INT1 _OnePadByte;        //  	n/a
    INT1 _TCPFlags;         //Protocol state (URG=32, ACK=16, PSH=8, RST=4, SYN=2, FIN=1).
                            //For example, a value of 27 indicates the flow had a SYN, ACK, PUSH, and FIN (2+16+8+1=27).
                            //http://www.wangchao.net.cn/bbsdetail_772174.html
                            //共有6个标志：URG、ACK、PSH、RST、SYN和FIN
                            //ACK：表示确认号是否合法，1表示合法，0表示确认号无效。确认号指的是希望收到的下一个字节而不是前面已经收到的字节。由此可见，发起建立连接请求的包（即第一个保）的ACK为总是置0的，其后的包则总是置1。
                            //　　SYN：用来建立连接。在连接请求中，SYN=1，连接请求确认中，SYN=1。因此，SYN表示CONNECTION REQUEST和CONNECTION ACCEPTED。在其后的TCP包中，SYN总是被置成0。
                            //　　FIN：用于撤销连接。
                           // 　　以上可以用下表来加以总结：
                            //　　ACK标志位 SYN标志位 该TCP包的含义
                            //　　0 1 连接请求
                            //　　1 1 连接请求确认（接受连接请求）
                            //　　1 0 确认连接请求确认
                            //　　1 0 数据包
    INT1 _Layer4Protocol;   //Type of layer 4 protocol. For example, ICMP=1, TCP=6, Telnet=14, UDP=17
    INT1 _IpType;           //Value that designates special handling of traffic (precedence, delay, throughput, and reliability)
                            //IP 服务类型
    INT2 _SourceSysID;               // n/a
    INT2 _DestSysID;                 // n/a
                                      //源和目的的AS
    INT1 _SourceMaskBitsCount;       // n/a
    INT1 _DestMaskBitsCount;         // n/a
                                    //源我目的的地址前缀的屏蔽位
    INT2 _TwoPadBytes;               // n/a
}NETFLOW5_RECORD,*PNETFLOW5_RECORD;

typedef struct tagNetFlow5
{
    CHAR PeerIp[20];//当前NF包来自那个交换机
    NETFLOW5_HEADER Header;
    NETFLOW5_RECORD Record[30];
}NETFLOW5,*PNETFLOW5;

//根据数据缓冲组成报文
PNETFLOW5 Encode_NetFlow5(const CHAR*PeerIp,const INT1*buf,INT4 bufLen);
//根据报文输出到缓冲
INT1* Decode_NetFlow5(PNETFLOW5 nf5, INT1*buf,INT4* bufLen);

CHAR* GetProtocol(PNETFLOW5 nf5,CHAR*out);
#endif
