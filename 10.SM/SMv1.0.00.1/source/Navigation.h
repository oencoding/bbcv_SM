#ifndef _NAVIGATION_H_
#define _NAVIGATION_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <iconv.h>
#include <locale.h>

#include "cJSON.h"
#include <map>
#include "Stream.h"
#include "SM_Manager.h"

class Navigation_Stream : public Stream
{
public:
	Navigation_Stream(SM_Manager *pManager);
	~Navigation_Stream();

	bool CleanNavStream(char *strSeesionId,char *strSID,char* strReSID,
					char *strAuthiName,char *strAuthcode,char *strSerialno,char *strMsg);
	bool CleanNavTask();

	static void *ts_recv_Nav_thread(void *arg);

	bool Connect_NavServer(int streamid,int mtemp=0);
	bool StartOneStream(char *strSeesionId,char *strSID,char* strReSID,
					char *strUrl,char *strIIP,char* strIPort,char* strISIP,char* strSPort,
					char* strAreaID,char *strSerialno,char *strMsg,int temp);
	bool FreeOneStream();
	bool GetTaskStatus(int isstreamid,int temp=0);
	bool Utfurl(char *pin,char *pout);
	static void *Parse_recv_Nav_thread(void *arg);

/*
	bool Send_Jsoon_str();

	bool Parse_Json_str(char *strJson);

	bool Requst_Json_str(int iType,char* strRequstType,char* strsecRequstContxt);
*/
	//dstd::map<int,
private:
	//����˿�ip
	char m_NavUrl[512];

	//����ģ��ظ��Ķ˿�
	int m_iNavPort;
	
	//���
	char m_strSerialNo[128];
	//��ֵ�˿�
	int m_keyPort;
	int m_s2qPort;

	SM_Manager *m_pManager;
	SetUsedRegionID m_whereHD;
};


#endif

