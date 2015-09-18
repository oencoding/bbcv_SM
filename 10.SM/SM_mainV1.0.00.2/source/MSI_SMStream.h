#ifndef _MSI_SMSTREAM_H_
#define _MSI_SMSTREAM_H_

#include "Stream.h"
#include <stdio.h>
#include <stdlib.h>
#include "SM_Manager.h"


class MSI_SM_Stream : public Stream
{
public:
	//��������֮ǰ�鲥�����Ѿ������������úù̶��ĵ�ַ
	MSI_SM_Stream(SM_Manager *pManager);
	~MSI_SM_Stream();


	bool CleanMSIStream(char *strUserId,char *strSerialno);
	
	bool Connect_MSIServer();
	bool TellMI(int isstreamid);
	SM_Manager *m_cSM_Manag;
	static void *Parse_recv_MSI_thread(void *arg);

	Sockfd  connsocket;
	//bool StartMulticast();
	//bool DeleteMulticasst();
	//bool GetTaskStatus();
private:
	
	SM_Manager *m_pManager;

};

#endif

