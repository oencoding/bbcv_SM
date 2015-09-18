#ifndef _ADVERTISEMENT_H_
#define _ADVERTISEMENT_H_

#include "Stream.h"
#include <stdio.h>
#include <stdlib.h>
#include "SM_Manager.h"


class Advertisement_Stream : public Stream
{
public:
	//��������֮ǰ�鲥�����Ѿ������������úù̶��ĵ�ַ
	Advertisement_Stream(SM_Manager *pManager);
	~Advertisement_Stream();

	bool CleanALLAdverStream();
	


	bool CleanAdverStream(char *strSeesionId,char *strTaskID,char *strSerialno);
	
	bool Connect_AdvServer();

	bool StartOneStream(char *strSeesionId,char  *strInputUrl,char* strOutputUrl,
					char *strSerialno,int temp=0);
	
	bool CheckStatus(int strSeesionId,char *strSerialno);

	static void *Parse_recv_Adver_thread(void *arg);

	bool Getaddr(char* strSeesionId,char *strSerialno);
private:
	//�鲥���߷����鲥�ĵ�ַ��udp://:239.1.1.1:12345
	char m_MulticastIP[10][128];//10·�鲥��
	//ipqam��iP��Port
	char m_transmitIP[128];
	int m_iport;
	
	Streatype m_isvod;
		
	SM_Manager *m_pManager;

	Sockfd  connsocket;

};

#endif

