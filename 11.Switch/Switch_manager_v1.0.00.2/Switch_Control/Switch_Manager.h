#ifndef SWITCH_MANAGER_H_
#define SWITCH_MANAGER_H_

#include "Switch_Stream.h"
#include <pthread.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include "Singleton.h"
#include "Stream.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <net/if.h>
#include <sys/time.h>
#include <sys/select.h>

#include "cJSON.h"



typedef struct _SwitchStream
{
	SwitchInfo *pSwitchInfo;
	Switch_Stream *pSwitchStream;

}SwitchStream;

/*

用下面的函数可检测端口是否被使用中
         const static BOOL PortUsedUDP(ULONG uPort)
{
MIB_UDPTABLE UdpTable[100];
DWORD nSize = sizeof(UdpTable);
if(NO_ERROR == GetUdpTable(&UdpTable[0],&nSize,TRUE))
{
DWORD nCount = UdpTable[0].dwNumEntries;
if (nCount > 0)
{
for(DWORD i=0;i<nCount;i++)
{
MIB_UDPROW UdpRow = UdpTable[0].table[i];
DWORD temp1 = UdpRow.dwLocalPort;
int temp2 = temp1 / 256 + (temp1 % 256) * 256;
if(temp2 == uPort)
{
return TRUE;
}
}
}
return FALSE;
}
return FALSE;
}


*/
/*
typedef std::map<std::string,SwitchStream*> MapSwitchInfo;

typedef std::map<std::string,SwitchInfo*> MapCheckSwitchInfo;
*/

typedef std::map<std::string,int>  MapSeverInfo;

typedef std::map<int,SwitchSeverInfo*> MapSwitchSeverInfo;



class Switch_Manager
{
public:
	Switch_Manager();
	~Switch_Manager();

	bool Init(char *SW_ip);  //需要管理发送 端口
	//创建映射关联 

	//返回的是关联的会话
	int AddOneSwitch(char* strUrl,char* outPutUrl,char* strSessionID,char* inputUrl);

	int DeleteOneSwitch(char* strUrl,char* outPutUrl,char* strSessionID);

	int DeleteAllWsitch(char* strUrl,char* outPutUrl,char* strSessionID);

	int CheckOneSwitch(char* strUrl,char* outPutUrl,char* strSessionID);

	int GetOneSwitchServer(SwitchSeverInfo** pSeverSwitch);

	int ReqOneSwitchForPort(char* inputUrl,char* outPutUrl,char* strSessionID);

	int FindSwitchServerFromIP(char* strIP,SwitchSeverInfo** ppSeverSwitch);
	int FindOneSwitchServer(char* iStreamID,SwitchSeverInfo** pSeverSwitch);

	int ReplyException(char* strurl,char* strOutUrl,int iRetcode);

	pthread_mutex_t m_lockerttTaskInfo;
	MapSeverInfo m_mapSwitchTaskInfo;

	//MapCheckSwitchInfo m_mapCheckSwitchInfo;


	pthread_mutex_t m_mutexServerList;
	MapSwitchSeverInfo m_mapSwitchServerInfo;

	int m_iCurrentPort;

	char m_SW_ip[256];
private:	



};


typedef Singleton<Switch_Manager>  ManagerFactory;



#endif

