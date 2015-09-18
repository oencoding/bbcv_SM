#ifndef _STREAM_H_
#define _STREAM_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "cJSON.h"
#include <map>


class Stream
{
public:
	Stream();
	virtual ~Stream();
/*
	virtual bool CleanStream(char *strSeesionId,char *strSID,char* strReSID,
					char *strAuthiName,char *strAuthcode,char *strSerialno,char *strMsg);
	virtual  bool CleanTask(){};

	virtual  bool ConnectServer();
	virtual  bool StartOneStream();
	virtual bool FreeOneStream();
	virtual bool GetTaskStatus();
*/
	virtual bool Send_Jsoon_str();

	virtual bool Parse_Json_str(char *strJson);

	virtual bool Requst_Json_str(int iType,const char* strRequstType,const char* strsecRequstContxt);

	//dstd::map<int,

	//输出端口ip
	char m_strdstIP[128];
	int m_idstport;
	int m_clientSocket;
	char m_Url[512];

	//报文回复
	cJSON *pRet_root;

};

#endif

