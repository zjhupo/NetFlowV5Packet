//---------------------------------------------------------------------------

#ifndef jsSocketH
#define jsSocketH
#include <vcl.h>
#include <winsock.h>
#include <stdio.h>
//---------------------------------------------------------------------------
//8-bit
typedef unsigned char INT1;
//16-bit
typedef Word INT2;
//32-bit
typedef unsigned long INT4;
typedef char CHAR;
//32-bit
typedef int INT0;
typedef unsigned __int64 INT8B;
//INT8    d;


//���紫���е�IP��ַת��
//�������͵�IP��ַת�����ַ���ʽ
void IntToIp(INT4 ulIp,char*buf);
//���ַ���ʽ��IP��ַת����������ʽ
void IpToInt(const CHAR*Ip, INT4*ulIp);

//���紫���е��ֽ�ת��
//��һ��unsigned int��4���ֽڵ�����0xabcdef01
//����������ڴ��еĴ洢��ʽΪ��01 EF CD AB
//����Э���еı�׼���紫��Ϊ��AB CD EF 01
typedef union {
    INT1 ichar[4];
    INT4 ilong;
} TINT4;

typedef union {
    INT1 ichar[2];
    INT2 iint;
} TINT2;

//����ĺ��������紫�������ת���ɲ���ϵͳ���ڴ��ʽ
void BufToINT1(INT1**buf,INT1 &v);
void BufToINT2(INT1**buf,INT2 &v);
void BufToINT4(INT1**buf,INT4 &v);

//�����紫�������/ʱ���ʽ����ת��
CHAR* INT4ToDateTimeStr(INT4 v,CHAR*out,INT0 StartYear,INT0 HourC);

#endif
