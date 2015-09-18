/*
	author yyd

*/


#ifndef _SM_CONTROL_H_
#define _SM_CONTROL_H_

//���ո���ģ���ָ������� �ɶ�Ӧ�Ĺ���ʵ��

#include "SM_Manager.h"
//#include "Advertisement.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>



class SM_Control
{
public:

	SM_Control();
	~SM_Control();
	
	//�������ݶ˿�
	static void *Parse_recv_Nav_thread(void *arg);
	static void *ts_recv_Nav_thread(void *arg);

	static void *Parse_recv_Adver_thread(void *arg);
	static void *ts_recv_Adver_thread(void *arg);

	static void *Parse_recv_Vod_thread(void *arg);
	static void *ts_recv_Vod_thread(void *arg);

	static void *Parse_recv_MSI_thread(void *arg);
	static void *ts_recv_MSI_thread(void *arg);
	


	SM_Manager *m_cSM_Manager;

	int m_NavacceptSocket;
	int m_AdvacceptSocket;
	int m_MsiacceptSocket;
	int m_VodacceptSocket;


	private:

	int m_iNavPort;
	int m_iAdverPort;
	int m_iVodPort;
	int m_iMsiPort;

	
	//����˿�ip
	char m_strdstIP[128];
	int m_iport;
	char m_NavUrl[512];

	//���
	char m_strSerialNo[128];
	char m_strSeesionID[128];
	//��ֵ�˿�
	int m_keyPort;
	int m_s2qPort;

};

#endif

