//---------------------------------------------------------------------------

/**
    һ��V5��ʽ�İ�Ϊ  ����ͷ+�����塣
    ����ͷ����=24
    �����峤��=48
    V5�涨���Ϊ30�������壬�����󳤶�Ϊ48*30+24=1464
*/
#ifndef NetFlowV5H
#define NetFlowV5H
//---------------------------------------------------------------------------
#include <vcl.h>
#include <winsock.h>
#include "jsSocket.h"

#define NETFLOW_LEN_HEADER 24
#define NETFLOW_LEN_RECORD 48


//����ͷ������Ϊ24
typedef struct tagNetFlow5_Header{
    INT2 _Version;     //Type of record format (5=NetFlow-5)
    INT2 _Count;       //Number of flow records contained in the UDP packet (maximum of 30)
    INT4 _SysUpTime;   //Value of SysUpTime ) when the UDP packet was sent (measured in milliseconds); SysUpTime measures how long the system has been up and running
                        //��·����������ʼ,����ǰ���Ժ���Ƶ�ʱ��
    INT4 _EpochSeconds;//Number of seconds between the epoch (January 1, 1970) and the time when the UDP packet was sent. (Epoch is a date used as the "beginning of time" for timestamps. Time values in Unix systems are represented as the number of seconds since the epoch.)
                        //��0000UTC1970��ʼ����ǰ������
    INT4 _Nanoseconds; //Residual nanoseconds (billionth of a second) after the epoch second
                        //��0000UTC1970��ʼ����ǰ����ĺ�����
    INT4 _FlowsSeen;   //Number of flows seen since PacketWise began emitting flow detail records
                       //��Ϣ����˳���
                       //������кŵ���ǰһ�����кż��ϸոչ�ȥ�����ݱ�������Ϣ���ĸ���
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
                             //��������� ���ӿڵ�SNMP����
    INT2 _OutboundSnmpIFindex;//ͬ��
    //��ǰ��Ϊͳ���˶��ٸ�����
    INT4 _PacketCount;        //Number of packets in the flow
                             //��Ϣ�������ݰ�
    //��ǰ��Ϊͳ���˶�����ĵ�������
    INT4 _ByteCount;         //Total number of bytes in the flow
                             //����Ϣ���������е�3���ֽڵ�����
    INT4 _StartTime;         //Value of SysUpTime when the first packet in the flow was seen (measured in milliseconds)
    INT4 _EndTime;           //Value of SysUpTime when the last packet in the flow was seen (measured in milliseconds)
    INT2 _SourcePort;        //Port number of the device that the flow went out of
    INT2 _DestPort;          //Port number of the device that the flow went to
    INT1 _OnePadByte;        //  	n/a
    INT1 _TCPFlags;         //Protocol state (URG=32, ACK=16, PSH=8, RST=4, SYN=2, FIN=1).
                            //For example, a value of 27 indicates the flow had a SYN, ACK, PUSH, and FIN (2+16+8+1=27).
                            //http://www.wangchao.net.cn/bbsdetail_772174.html
                            //����6����־��URG��ACK��PSH��RST��SYN��FIN
                            //ACK����ʾȷ�Ϻ��Ƿ�Ϸ���1��ʾ�Ϸ���0��ʾȷ�Ϻ���Ч��ȷ�Ϻ�ָ����ϣ���յ�����һ���ֽڶ�����ǰ���Ѿ��յ����ֽڡ��ɴ˿ɼ�����������������İ�������һ��������ACKΪ������0�ģ����İ���������1��
                            //����SYN�������������ӡ������������У�SYN=1����������ȷ���У�SYN=1����ˣ�SYN��ʾCONNECTION REQUEST��CONNECTION ACCEPTED��������TCP���У�SYN���Ǳ��ó�0��
                            //����FIN�����ڳ������ӡ�
                           // �������Ͽ������±��������ܽ᣺
                            //����ACK��־λ SYN��־λ ��TCP���ĺ���
                            //����0 1 ��������
                            //����1 1 ��������ȷ�ϣ�������������
                            //����1 0 ȷ����������ȷ��
                            //����1 0 ���ݰ�
    INT1 _Layer4Protocol;   //Type of layer 4 protocol. For example, ICMP=1, TCP=6, Telnet=14, UDP=17
    INT1 _IpType;           //Value that designates special handling of traffic (precedence, delay, throughput, and reliability)
                            //IP ��������
    INT2 _SourceSysID;               // n/a
    INT2 _DestSysID;                 // n/a
                                      //Դ��Ŀ�ĵ�AS
    INT1 _SourceMaskBitsCount;       // n/a
    INT1 _DestMaskBitsCount;         // n/a
                                    //Դ��Ŀ�ĵĵ�ַǰ׺������λ
    INT2 _TwoPadBytes;               // n/a
}NETFLOW5_RECORD,*PNETFLOW5_RECORD;

typedef struct tagNetFlow5
{
    CHAR PeerIp[20];//��ǰNF�������Ǹ�������
    NETFLOW5_HEADER Header;
    NETFLOW5_RECORD Record[30];
}NETFLOW5,*PNETFLOW5;

//�������ݻ�����ɱ���
PNETFLOW5 Encode_NetFlow5(const CHAR*PeerIp,const INT1*buf,INT4 bufLen);
//���ݱ������������
INT1* Decode_NetFlow5(PNETFLOW5 nf5, INT1*buf,INT4* bufLen);

CHAR* GetProtocol(PNETFLOW5 nf5,CHAR*out);
#endif
