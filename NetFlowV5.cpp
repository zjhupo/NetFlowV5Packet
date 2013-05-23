//---------------------------------------------------------------------------


#pragma hdrstop

#include "NetFlowV5.h"
#include <stdio.h>
#include "ProtocolUtils.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

static INT1 _Sys_Buf[24+48*30+1];

//�����紫������ݽ�����NETFLOW��׼��
PNETFLOW5 Encode_NetFlow5(const CHAR*PeerIp,const INT1*buf,INT4 bufLen)
{
    INT4 ulLen = bufLen - NETFLOW_LEN_HEADER;
    if(ulLen % NETFLOW_LEN_RECORD !=0)
    {//���ĳ��Ȳ����ϱ�׼
        return NULL;
    }
    memset(_Sys_Buf,0,sizeof(_Sys_Buf));
    memcpy(_Sys_Buf,buf,bufLen);
    INT1*p = _Sys_Buf;

    PNETFLOW5 vFlow = new NETFLOW5;

    BufToINT2(&p,vFlow->Header._Version);
    if(vFlow->Header._Version!=5)
    {   //�������V5��ʽ���򷵻�,��Ϊ"��Ч����"����
        delete vFlow;
        return NULL;
    }
    strcpy(vFlow->PeerIp,PeerIp);
    BufToINT2(&p,vFlow->Header._Count);
    BufToINT4(&p,vFlow->Header._SysUpTime);
    BufToINT4(&p,vFlow->Header._EpochSeconds);
    BufToINT4(&p,vFlow->Header._Nanoseconds);
    BufToINT4(&p,vFlow->Header._FlowsSeen);
    BufToINT1(&p,vFlow->Header._EngineType);
    BufToINT1(&p,vFlow->Header._EngineID);
    BufToINT2(&p,vFlow->Header._SamplingInfo);

    for(INT2 i =0;i< vFlow->Header._Count ;i++)
    {
        BufToINT4(&p,vFlow->Record[i]._SourceIp);
        BufToINT4(&p,vFlow->Record[i]._DestIp);
        BufToINT4(&p,vFlow->Record[i]._NextRouterIp);
        BufToINT2(&p,vFlow->Record[i]._InboundSnmpIFindex);
        BufToINT2(&p,vFlow->Record[i]._OutboundSnmpIFindex);
        BufToINT4(&p,vFlow->Record[i]._PacketCount);
        BufToINT4(&p,vFlow->Record[i]._ByteCount);
        BufToINT4(&p,vFlow->Record[i]._StartTime);
        BufToINT4(&p,vFlow->Record[i]._EndTime);
        BufToINT2(&p,vFlow->Record[i]._SourcePort);
        BufToINT2(&p,vFlow->Record[i]._DestPort);
        BufToINT1(&p,vFlow->Record[i]._OnePadByte);
        BufToINT1(&p,vFlow->Record[i]._TCPFlags);
        BufToINT1(&p,vFlow->Record[i]._Layer4Protocol);
        BufToINT1(&p,vFlow->Record[i]._IpType);
        BufToINT2(&p,vFlow->Record[i]._SourceSysID);
        BufToINT2(&p,vFlow->Record[i]._DestSysID);
        BufToINT1(&p,vFlow->Record[i]._SourceMaskBitsCount);
        BufToINT1(&p,vFlow->Record[i]._DestMaskBitsCount);
        BufToINT2(&p,vFlow->Record[i]._TwoPadBytes);
    }
    return vFlow;
}

CHAR* GetProtocol(PNETFLOW5 nf5,CHAR*out)
{
    //TODO:������Ҫ���ݶ˿�������ɸ���ϸ��Э��

    //����V5�ĸ�ʽ,���õ���ϢΪ  "Э����"��"�˿ڱ��"


    if(G_nfProtocal->Find(nf5->Record[0]._Layer4Protocol,nf5->Record[0]._DestPort,out)==NULL)
    {
        strcpy(out,"");
        switch(nf5->Record[0]._Layer4Protocol)
        {
            case 1:
                strcpy(out,"ICMP");
                break;
            case 6:
                strcpy(out,"TCP");
                break;
            case 14:
                strcpy(out,"Telnet");
                break;
            case 17:
                strcpy(out,"UDP");
                break;
        }
    }
    return out;
}

