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


typedef struct _SwitchInfo
{
	int iSRCPort;
	char strSRCIP[128];
	
	int iDstPort;
	char strDstIP[128];

}SwitchInfo;

typedef unsigned char uint8_t;


const int iMSIServerPort = 20919;

const int iBeginPort = 14000;

#define MCAST_ADDR "225.0.0.1"     /*һ���ֲ����Ӷಥ��ַ��·����������ת��*/



#endif
