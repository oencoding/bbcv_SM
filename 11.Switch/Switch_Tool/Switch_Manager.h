#ifndef SWITCH_MANAGER_H_
#define SWITCH_MANAGER_H_

#include "Switch_Stream.h"
#include <pthread.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>


typedef struct _SwitchStream
{
	SwitchInfo *pSwitchInfo;
	Switch_Stream *pSwitchStream;

}SwitchStream;

/*

������ĺ����ɼ��˿��Ƿ�ʹ����
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

typedef std::map<std::string,SwitchStream*> MapSwitchInfo;

class Switch_Manager
{
public:
	Switch_Manager();
	~Switch_Manager();

	bool Init();  //��Ҫ������ �˿�
	//����ӳ����� 

	//���ص��ǹ����ĻỰ
	int AddOneSwitch(char* inputUrl,char* outPutUrl,char* strSessionID);

	int DeleteOneSwitch(char* strSessionID);

	int DeleteAllWsitch();

	int CheckOneSwitch(char* strSessionID);

	pthread_mutex_t m_lockerttTaskInfo;
	MapSwitchInfo m_mapSwitchTaskInfo;

	int m_iCurrentPort;
private:	


};



#endif

