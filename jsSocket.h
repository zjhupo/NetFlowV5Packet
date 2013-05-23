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


//网络传输中的IP地址转换
//将数字型的IP地址转换成字符形式
void IntToIp(INT4 ulIp,char*buf);
//将字符形式的IP地址转换成数字形式
void IpToInt(const CHAR*Ip, INT4*ulIp);

//网络传输中的字节转换
//如一个unsigned int的4个字节的数字0xabcdef01
//这个数字在内存中的存储形式为：01 EF CD AB
//但在协议中的标准网络传输为：AB CD EF 01
typedef union {
    INT1 ichar[4];
    INT4 ilong;
} TINT4;

typedef union {
    INT1 ichar[2];
    INT2 iint;
} TINT2;

//下面的函数将网络传输的数字转换成操作系统的内存格式
void BufToINT1(INT1**buf,INT1 &v);
void BufToINT2(INT1**buf,INT2 &v);
void BufToINT4(INT1**buf,INT4 &v);

//将网络传输的日期/时间格式进行转换
CHAR* INT4ToDateTimeStr(INT4 v,CHAR*out,INT0 StartYear,INT0 HourC);

#endif
