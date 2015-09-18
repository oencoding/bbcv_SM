#ifndef _VGW_VOD_STREAM_H_
#define _VGW_VOD_STREAM_H_

#include "Stream.h"
#include <stdio.h>
#include <stdlib.h>
#include "SM_Manager.h"
#include "Base64.h"


typedef std::map<int,void*> MapSem_Stream;
typedef std::map<int,Stream*>MapVodPlay;  //��ظ�socket����


class VGW_Vod_Stream : public Stream
{
 public:
	//��������֮ǰ�鲥�����Ѿ������������úù̶��ĵ�ַ
	VGW_Vod_Stream(SM_Manager *pManager);
	~VGW_Vod_Stream();


	bool CleanVODStream(char *strSeesionId,char *strTaskID,char *strSerialno);
	
	bool Connect_VODServer();

	static void *Parse_recv_Vod_thread(void *arg);


	bool AddSemToStream(int iStreamID,void *psem);
	bool StartOneStream(int iStreamID,char *strUrl,char *strRegionID,char* strUserID,char *vodname,char *posterurl,int fd=0,char *outputid = NULL,char *outputport = NULL);

	bool CheckStatus(int strSeesionId,char *strTaskID,char *strSerialno);

	bool Checktype();

	MapVodPlay m_mapVod_player;
private:

	//ipqam��iP��Port
	char m_transmitIP[128];
	int m_iport;

	SM_Manager *m_pManager;

	MapSem_Stream m_mapSem_Stream;



};

#endif

