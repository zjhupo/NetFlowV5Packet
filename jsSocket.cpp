//---------------------------------------------------------------------------


#pragma hdrstop

#include "jsSocket.h"
#include <time.h>

//---------------------------------------------------------------------------

#pragma package(smart_init)

//将数字型的IP地址转换成字符形式
void IntToIp(INT4 ulIp,char*buf)
{
    INT4 tt= htonl(ulIp);
    struct in_addr in;
    in.S_un.S_addr=tt;
    char tmp[20];
    char *p=tmp;
    p=inet_ntoa(in);
    strcpy(tmp,p);
    strcpy(buf,tmp);
    //return buf;
}

//将字符形式的IP地址转换成数字形式
void IpToInt(const CHAR*Ip, INT4*ulIp)
{
    *ulIp=ntohl(inet_addr(Ip));
}

//----------------------------------------
void BufToINT1(INT1**buf,INT1 &v)
{
    v = *buf[0];
    (*buf)++;
}

void BufToINT2(INT1**buf,INT2 &v)
{
    TINT2 lcValue;
    lcValue.ichar[0] = (*buf)[1];
    lcValue.ichar[1] = (*buf)[0];
    v = lcValue.iint;
    (*buf) += 2;
}

void BufToINT4(INT1**buf,INT4 &v)
{
//将传输的值转换成内存的值,如传输的内容为ab cd ,内存的值为cd ab
// buf=       ab
//         00 ab
//   00 00 00 ab
//
    TINT4 lcValue;
    lcValue.ichar[0] = (*buf)[3];
    lcValue.ichar[1] = (*buf)[2];
    lcValue.ichar[2] = (*buf)[1];
    lcValue.ichar[3] = (*buf)[0];
    v = lcValue.ilong;
    (*buf)+=4;
}

//------------------------------------------
CHAR* INT4ToDateTimeStr(INT4 v,CHAR*out,INT0 StartYear,INT0 HourC)
{//将数字形式的秒数转换成日期
//v:网络传输的数字
//out:最终以字符形式出现
//StartYear:起始年份
//HourC:时差修正
    time_t timer;
    struct tm *tblock;
    /* gets time of day */
    timer = v;//StrToInt64("$"+sgData->Cells[sgData->Col][sgData->Row]);//0x49256956;//0x49256860;

    /* converts date/time to a structure */
    tblock = localtime(&timer);

    if(tblock==NULL)
        sprintf(out,"%s","");
    else
        sprintf(out,"%04d-%02d-%02d %02d:%02d:%02d",
            tblock->tm_year+StartYear/*1900*/,
            tblock->tm_mon+1,
            tblock->tm_mday,
            tblock->tm_hour+HourC,
            tblock->tm_min,
            tblock->tm_sec);
    return out;

}
