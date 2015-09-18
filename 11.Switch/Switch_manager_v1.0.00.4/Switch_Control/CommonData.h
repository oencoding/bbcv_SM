#ifndef _COMMONDATA_H_
#define _COMMONDATA_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <map>
#include <vector>
#include <list>
#include <set>
#include <string>


//check status errono
const int Error_No = 0;
const int Error_NoSourceData = -1;
const int Error_NoSendData = -2;
const int Error_NoAll = -3;

const int Error_NO_Sever = -21001;
const int Error_Connect_Sever = -21002;
const int Error_Recv_Sever = -21003;
const int Error_ReSet_Device = -21004;
const int Error_NO_Stream = -21005;





typedef struct _SwitchServerInfo
{
    int  iServerID;
    char strServerIPAddr[256];
    int  iListenPort;
    int  iRunStatus;		 //����״̬�쳣�>0    ����״̬����0
	int	 iStreamStatus;    //��״̬�쳣1   ��״̬����0
    int  iMaxSwitchNumber;
    int  iCurrentSwitchNumber;

}SwitchSeverInfo;


typedef struct _SwitchInfo
{
	int iSRCPort;
	char strSRCIP[128];

	int iDstPort;
	char strDstIP[128];

	int iCheckStatus;
}SwitchInfo;

typedef unsigned char uint8_t;

const int iKEEPALIVETIME = 2;
const int MaxAliveTickInterval = 1000*1000; // 2 sec
const int MinAliveTickInterval = 1000; // 2 sec

//int iAliveTickInterval = 5*1000; // 2 sec
const int iMSIServerPort = 20919;

const int iBeginPort = 14000;

#define MCAST_ADDR "225.0.0.1"     /*һ���ֲ����Ӷಥ��ַ��·����������ת��*/



#endif
