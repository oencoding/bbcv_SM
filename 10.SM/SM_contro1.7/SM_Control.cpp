#include "SM_Control.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>

#include "CommonFun.h"
#include "cJSON.h"
#include "Stream.h"

SM_Control::SM_Control()
{
	m_iNavPort = 33333;
	m_iAdverPort = 33335;
	m_iVodPort = 33337;
	m_iCleanPort = 20910;
	m_iUseractport = 20915;
	m_iMsiPort = iMSIServerPort;

	m_cSM_Manager = NULL;
//���������߳�
//	pthread_t tcp_recv_thread1;
//	pthread_create(&tcp_recv_thread1, NULL, ts_recv_Nav_thread, this);
//	pthread_detach(tcp_recv_thread1);

	//���������߳�
/*	pthread_t tcp_recv_thread2;
	pthread_create(&tcp_recv_thread2, NULL, ts_recv_Adver_thread, this);
	pthread_detach(tcp_recv_thread2);
*/
	//���������߳�
//	pthread_t tcp_recv_thread3;
//	pthread_create(&tcp_recv_thread3, NULL, ts_recv_Vod_thread, this);
//	pthread_detach(tcp_recv_thread3);

	//���������߳�
	pthread_t tcp_recv_thread4;
	pthread_create(&tcp_recv_thread4, NULL, ts_recv_MSI_thread, this);
	pthread_detach(tcp_recv_thread4);
	//���������߳�
	pthread_t tcp_recv_thread5;
	pthread_create(&tcp_recv_thread5, NULL, ts_recv_Clear_thread, this);
	pthread_detach(tcp_recv_thread5);
	//����
	pthread_t tcp_recv_thread6;
	pthread_create(&tcp_recv_thread6, NULL, ts_recv_useract_thread, this);
	pthread_detach(tcp_recv_thread6);
	m_cSM_Manager = new SM_Manager;
	
	//m_cSM_Manager->m_Advertiser->StartOneStream("1","udp://225.0.0.1:12000","udp://192.168.100.106:50442","e123214");
//	m_cSM_Manager->m_Navigation->StartOneStream("12321","1001","1001","udp://192.111.1l.1:10000","192.168.100.106","1000","120.1.1.11",
//						"11111","1","3dfaf12132213","12312qw1qw");
	pthread_t tcp_recv_thread7;
	pthread_create(&tcp_recv_thread7, NULL, ts_checksession_thread,this);
	pthread_detach(tcp_recv_thread7);

//װ������
//	m_cSM_Manager->LoadStreamStatus();
	m_cSM_Manager->InitStream();
	
	//����
//	m_cSM_Manager->CleanAllStream();

//�������
//���������������������ͬΪһ��
//���Ȳ������������ţ���Ӧ�ٲ��Ҹñ���������ID ��
//����Ŵ����set���У��ٴ�set����ÿ�������Ϊmap first Value ���뵽��Ӧ�����ݵ�map�С�


}
SM_Control::~SM_Control()
{
	if(m_cSM_Manager)
		delete m_cSM_Manager;
	m_cSM_Manager = NULL;


}

//���������ӿ�
void *SM_Control::Parse_recv_Nav_thread(void *arg)
{
	SM_Control *this0 = (SM_Control*)arg;

	int len;
	char Rcv_buf[4096];

	cJSON *pcmd = NULL;

	char cJsonBuff[1024 * 2];
	int iRecvLen = 0;
	int accept_fd = this0->m_NavacceptSocket;
	while(1)
	{
			memset(Rcv_buf,0,sizeof(Rcv_buf));
			int length = 0;
			int i_rc = 0, i_count = 0;
			do
			{
				i_rc = recv(accept_fd, Rcv_buf + i_count, 2000 - i_count, 0);
				if (i_rc <= 0)break;//�쳣�ر�
				i_count += i_rc;
			} while (strstr(Rcv_buf, "XXEE") == NULL);
			iRecvLen = i_count;
			if (iRecvLen <= 0) break;
	
			printf("recv:%s \n",Rcv_buf);
			fflush(stdout);
	
					//������������
			replace(Rcv_buf, "XXEE", "");

			cJSON* pRoot = cJSON_Parse(Rcv_buf);	
			pcmd = cJSON_GetObjectItem(pRoot, "cmd");
			if (pcmd)
			{
				//�ж���������
				if (strcmp(pcmd->valuestring, "login") == 0)
				{
					
				}
				else if(strcmp(pcmd->valuestring, "logout") == 0)
				{
					
				}
			}
			else
			{
			
			}
			
	}

	if(this0->m_NavacceptSocket != -1)
		close(this0->m_NavacceptSocket);
	this0->m_NavacceptSocket = -1;
}


//�������ݶ˿�
void *SM_Control::ts_recv_Nav_thread(void *arg)
{
	SM_Control *this0 = (SM_Control*)arg;

	int BUFFER_SIZE = 4096;
	struct sockaddr_in s_addr;
	int sock;
	socklen_t addr_len;


	if ( (sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))  == -1) {
		perror("socket");
		return NULL;
	} else
		printf("create socket.\n\r");
		fflush(stdout);
	memset(&s_addr, 0, sizeof(struct sockaddr_in));
	s_addr.sin_family = AF_INET;

	s_addr.sin_port = htons(this0->m_iNavPort);


	s_addr.sin_addr.s_addr = INADDR_ANY;

	int optval = 1;
	if ((setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(int))) == -1)
	{
		close(sock);
		return NULL;
	}

	if ( (bind(sock, (struct sockaddr*)&s_addr, sizeof(s_addr))) == -1 ) {
		perror("bind");
		return NULL;
	}else
		printf("bind address to socket.\n\r");
		fflush(stdout);

	if(listen(sock,10)<0)
	{
		perror("listen");
		return NULL;
	}

	int accept_fd = -1;
	struct sockaddr_in remote_addr;
	memset(&remote_addr,0,sizeof(remote_addr));

	this0->m_NavacceptSocket = -1;
	
	while( 1 )
	{  
		
	    socklen_t sin_size = sizeof(struct sockaddr_in); 
		if(( accept_fd = accept(sock,(struct sockaddr*) &remote_addr,&sin_size)) == -1 )  
         {  
             	printf( "Accept error!\n"); 
				fflush(stdout);
                //continue;  
	     }  
         printf("Received a connection from %s\n",(char*) inet_ntoa(remote_addr.sin_addr));  
		 fflush(stdout);
		if(this0->m_NavacceptSocket != -1)
		{
			//��Ҫ�ͷ�֮ǰ������
		}
		this0->m_NavacceptSocket = accept_fd;
		pthread_t tcp_recv_thread1;
		pthread_create(&tcp_recv_thread1, NULL, Parse_recv_Nav_thread, this0);
		pthread_detach(tcp_recv_thread1);
		

	}

	//���սӿڻظ�������
	

}

//���������ӿ�
void *SM_Control::Parse_recv_Adver_thread(void * arg)
{
	SM_Control *this0 = (SM_Control*)arg;

	int len;
	char Rcv_buf[4096];

	cJSON *pcmd = NULL;

	char cJsonBuff[1024 * 2];
	int iRecvLen = 0;
	int accept_fd = this0->m_AdvacceptSocket;
	while(1)
	{
			memset(Rcv_buf,0,sizeof(Rcv_buf));
			int length = 0;
			int i_rc = 0, i_count = 0;
			do
			{
				i_rc = recv(accept_fd, Rcv_buf + i_count, 2000 - i_count, 0);
				if (i_rc <= 0)break;//�쳣�ر�
				i_count += i_rc;
			} while (strstr(Rcv_buf, "XXEE") == NULL);
			iRecvLen = i_count;
			if (iRecvLen <= 0) break;
	
			printf("recv:%s \n",Rcv_buf);
			fflush(stdout);
	
					//������������
			replace(Rcv_buf, "XXEE", "");
			cJSON* pRoot = cJSON_Parse(Rcv_buf);	

			if (pRoot)
			{
				pcmd = cJSON_GetObjectItem(pRoot, "cmd");
				if (pcmd)
				{
					//�ж���������
					if (strcmp(pcmd->valuestring, "add_ads_stream") == 0)
					{
						printf("--add_ads_stream return \n");
						fflush(stdout);
						//ͨ��
						cJSON* pSeesid = cJSON_GetObjectItem(pRoot, "sessionid");
						//char strSeesionID[128] ={0};
					//	memcpy(strSeesionID,pSeesid->valuestring,strlen(pSeesid->valuestring));
						int iSeessionID = 0;
						if(pSeesid) iSeessionID = atoi(pSeesid->valuestring);
						cJSON* pTaskID = cJSON_GetObjectItem(pRoot, "task_id");
						int iTaskID = -1;
						if(pTaskID) iTaskID = pTaskID->valueint;
						cJSON* pRetCOde = cJSON_GetObjectItem(pRoot,"ret_code");
						int iRetCode = -1;
						if(pRetCOde) iRetCode = atoi(pRetCOde->valuestring);
						cJSON* pSerialNo = cJSON_GetObjectItem(pRoot,"serialno");
						char strSerialNo[128] ={0};
						if(pSerialNo)
							memcpy(strSerialNo,pSerialNo->valuestring,strlen(pSerialNo->valuestring)+1);
										
						
						//��������Ϣ״̬
						//�ҵ�����Ӧ���ݣ�
						MapStreamStatus::iterator itFind = this0->m_cSM_Manager->m_mapStreamStatus.find(iSeessionID);
						if(itFind != this0->m_cSM_Manager->m_mapStreamStatus.end())
						{
							//�޸��ڴ�����
							StreamStatus *ptmp = itFind->second;
							if(iRetCode >= 0)
							{
								strcpy(ptmp->strStreamType,Status_Adver);
								if(strcmp(ptmp->strBind_userID,"") == 0)
								{
									//δ��									
								}
								else
								{
									
								}

							}
							else
							{
								// retcode ʧ����Ҫ����
							}					
							
							//�޸Ķ�Ӧ���ݿ�
						}
						
					}
					else if(strcmp(pcmd->valuestring, "del_ads_stream") == 0)
					{
						printf("--del_ads_stream return \n");
						fflush(stdout);
						//ͨ��
						cJSON* pSeesid = cJSON_GetObjectItem(pRoot, "sessionid");
						//char strSeesionID[128] ={0};
					//	memcpy(strSeesionID,pSeesid->valuestring,strlen(pSeesid->valuestring));
						int iSeessionID = 0;
						if(pSeesid) iSeessionID = atoi(pSeesid->valuestring);
						cJSON* pRetCOde = cJSON_GetObjectItem(pRoot,"ret_code");
						int iRetCode = -1;
						if(pRetCOde) iRetCode = atoi(pRetCOde->valuestring);
						cJSON* pSerialNo = cJSON_GetObjectItem(pRoot,"serialno");
						char strSerialNo[128] ={0};
						if(pSerialNo)
						memcpy(strSerialNo,pSerialNo->valuestring,strlen(pSerialNo->valuestring)+1);
						
						//��������Ϣ״̬
						//�ҵ�����Ӧ���ݣ�
						MapStreamStatus::iterator itFind = this0->m_cSM_Manager->m_mapStreamStatus.find(iSeessionID);
						if(itFind != this0->m_cSM_Manager->m_mapStreamStatus.end())
						{
							//�޸��ڴ�����
							StreamStatus *ptmp = itFind->second;
							if(iRetCode >= 0)
							{
								strcpy(ptmp->strStreamType,Status_Adver);
								if(strcmp(ptmp->strBind_userID,"") == 0)
								{
									//δ��									
								}
								else
								{
									
								}
							}
							else
							{
								// retcode ʧ����Ҫ����
							}
							//�޸Ķ�Ӧ���ݿ�
						}						
					}
					else if(strcmp(pcmd->valuestring, "check_session") == 0)
					{
						printf("--check_session return \n");
						fflush(stdout);
						//ͨ��
						cJSON* pSeesid = cJSON_GetObjectItem(pRoot, "sessionid");
						//char strSeesionID[128] ={0};
					//	memcpy(strSeesionID,pSeesid->valuestring,strlen(pSeesid->valuestring));

						int iSeessionID = 0;
						if(pSeesid) iSeessionID = atoi(pSeesid->valuestring);
						cJSON* pRetCOde = cJSON_GetObjectItem(pRoot,"ret_code");
						int iRetCode = -1;
						if(pRetCOde) iRetCode = atoi(pRetCOde->valuestring);
						cJSON* pSerialNo = cJSON_GetObjectItem(pRoot,"serialno");
						char strSerialNo[128] ={0};
						if(pSerialNo)
							memcpy(strSerialNo,pSerialNo->valuestring,strlen(pSerialNo->valuestring)+1);
	
					}
					
				}
			}
			
	}

	if(this0->m_AdvacceptSocket != -1)
		close(this0->m_AdvacceptSocket);
	this0->m_AdvacceptSocket = -1;
}

void *SM_Control::ts_recv_Adver_thread(void *arg)
{
	SM_Control *this0 = (SM_Control*)arg;

	int BUFFER_SIZE = 4096;
	struct sockaddr_in s_addr;
	int sock;
	socklen_t addr_len;


	if ( (sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))  == -1) {
		perror("socket");
		return NULL;
	} else
		printf("create socket.\n\r");
		fflush(stdout);

	memset(&s_addr, 0, sizeof(struct sockaddr_in));
	s_addr.sin_family = AF_INET;

	s_addr.sin_port = htons(this0->m_iAdverPort);


	s_addr.sin_addr.s_addr = INADDR_ANY;

	int optval = 1;
	if ((setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(int))) == -1)
	{
		close(sock);
		return NULL;
	}

	if ( (bind(sock, (struct sockaddr*)&s_addr, sizeof(s_addr))) == -1 ) {
		perror("bind");
		return NULL;
	}else
		printf("bind address to socket.\n\r");
		fflush(stdout);

	if(listen(sock,10)<0)
	{
		perror("listen");
		return NULL;
	}

	int accept_fd = -1;
	struct sockaddr_in remote_addr;
	memset(&remote_addr,0,sizeof(remote_addr));

	this0->m_AdvacceptSocket = -1;
	
	while( 1 )
	{  
		
	    socklen_t sin_size = sizeof(struct sockaddr_in); 
		if(( accept_fd = accept(sock,(struct sockaddr*) &remote_addr,&sin_size)) == -1 )  
         {  
             	printf( "Accept error!\n");  
				fflush(stdout);
                //continue;  
	     }  
         printf("Received a connection from %s\n",(char*) inet_ntoa(remote_addr.sin_addr));  
		 fflush(stdout);
		if(this0->m_AdvacceptSocket != -1)
		{
			//��Ҫ�ͷ�֮ǰ������
		}
		this0->m_AdvacceptSocket = accept_fd;
		pthread_t tcp_recv_thread1;
		pthread_create(&tcp_recv_thread1, NULL, Parse_recv_Adver_thread, this0);
		pthread_detach(tcp_recv_thread1);
		

	}

}

//����VOD�ӿ�
void *SM_Control::Parse_recv_Vod_thread(void * arg)
{
	SM_Control *this0 = (SM_Control*)arg;

	int len;
	char Rcv_buf[4096];

	cJSON *pcmd = NULL;

	char cJsonBuff[1024 * 2];
	int iRecvLen = 0;
	int accept_fd = this0->m_VodacceptSocket;
	while(1)
	{
			memset(Rcv_buf,0,sizeof(Rcv_buf));
			int length = 0;
			int i_rc = 0, i_count = 0;
			do
			{
				i_rc = recv(accept_fd, Rcv_buf + i_count, 2000 - i_count, 0);
				if (i_rc <= 0)break;//�쳣�ر�
				i_count += i_rc;
			} while (strstr(Rcv_buf, "XXEE") == NULL);
			iRecvLen = i_count;
			if (iRecvLen <= 0) break;
	
			printf("recv:%s \n",Rcv_buf);
			fflush(stdout);
	
					//������������
			replace(Rcv_buf, "XXEE", "");
			cJSON* pRoot = cJSON_Parse(Rcv_buf);	

			if (pRoot)
			{
					
				pcmd = cJSON_GetObjectItem(pRoot, "cmd");
				if (pcmd)
				{
					//�ж���������
	/*				if (strcmp(pcmd->valuestring, "add_ads_stream") == 0)
					{
						printf("--add_ads_stream return \n");
					}
					else if(strcmp(pcmd->valuestring, "del_ads_stream") == 0)
					{
						printf("--del_ads_stream return \n");
					}
					else if(strcmp(pcmd->valuestring, "check_session") == 0)
					{
						printf("--check_session return \n");
					}
	*/				
				}
			}
			
	}

	if(this0->m_VodacceptSocket != -1)
		close(this0->m_VodacceptSocket);
	this0->m_VodacceptSocket = -1;
}

void *SM_Control::ts_recv_Vod_thread(void *arg)
{
	SM_Control *this0 = (SM_Control*)arg;

	int BUFFER_SIZE = 4096;
	struct sockaddr_in s_addr;
	int sock;
	socklen_t addr_len;


	if ( (sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))  == -1) {
		perror("socket");
		return NULL;
	} else
		printf("create socket.\n\r");
		fflush(stdout);
	memset(&s_addr, 0, sizeof(struct sockaddr_in));
	s_addr.sin_family = AF_INET;

	s_addr.sin_port = htons(this0->m_iVodPort);


	s_addr.sin_addr.s_addr = INADDR_ANY;

	int optval = 1;
	if ((setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(int))) == -1)
	{
		close(sock);
		return NULL;
	}

	if ( (bind(sock, (struct sockaddr*)&s_addr, sizeof(s_addr))) == -1 ) {
		perror("bind");
		return NULL;
	}else
		printf("bind address to socket.\n\r");
		fflush(stdout);
		
	if(listen(sock,10)<0)
	{
		perror("listen");
		return NULL;
	}

	int accept_fd = -1;
	struct sockaddr_in remote_addr;
	memset(&remote_addr,0,sizeof(remote_addr));

	this0->m_VodacceptSocket = -1;
	
	while( 1 )
	{  
		
	    socklen_t sin_size = sizeof(struct sockaddr_in); 
		if(( accept_fd = accept(sock,(struct sockaddr*) &remote_addr,&sin_size)) == -1 )  
         {  
             	printf( "Accept error!\n");  
				fflush(stdout);
                //continue;  
	     }  
         printf("Received a connection from %s\n",(char*) inet_ntoa(remote_addr.sin_addr));  
		 fflush(stdout);
		if(this0->m_VodacceptSocket != -1)
		{
			//��Ҫ�ͷ�֮ǰ������
		}
		this0->m_VodacceptSocket = accept_fd;
		pthread_t tcp_recv_thread1;
		pthread_create(&tcp_recv_thread1, NULL, Parse_recv_Vod_thread, this0);
		pthread_detach(tcp_recv_thread1);
		

	}


}


//����MSI�ӿ�
void *SM_Control::Parse_recv_MSI_thread(void * arg)
{
	SM_Control *this0 = (SM_Control*)arg;
	int len;
	char Rcv_buf[4096];
	cJSON *pcmd = NULL;
	char cJsonBuff[1024 * 2];
	int iRecvLen = 0;
	int accept_fd = 0;
	Sockfd::iterator iterList = this0->m_MsiacceptSocket.begin();
	if(iterList != this0->m_MsiacceptSocket.end())
	{
		accept_fd = *iterList;
		this0->m_MsiacceptSocket.erase(iterList);
		printf("111222---accept_fd = %d\n",accept_fd);
		fflush(stdout);
	}
	while(1)
	{
			if(accept_fd == -1)
			{
				printf("error accept socket in msi\n");
				fflush(stdout);
				break;
			}
			memset(Rcv_buf,0,sizeof(Rcv_buf));
			int length = 0;
			int i_rc = 0, i_count = 0;
			do
			{
				i_rc = recv(accept_fd, Rcv_buf + i_count, 2000 - i_count, 0);
				if (i_rc <= 0)break;//�쳣�ر�
				i_count += i_rc;
			} while (strstr(Rcv_buf, "XXEE") == NULL);
			iRecvLen = i_count;
			if (iRecvLen <= 0) break;
	
			printf("recv:%s \n",Rcv_buf);
			fflush(stdout);
	
					//������������
			replace(Rcv_buf, "XXEE", "");
			cJSON* pRoot = cJSON_Parse(Rcv_buf);	

			if (pRoot)
			{
				pcmd = cJSON_GetObjectItem(pRoot, "cmd");
				if (pcmd)
				{
					
					//�ж���������
					if (strcmp(pcmd->valuestring, "stream_bind") == 0)
					{
						printf("--bind_stream request \n");
						fflush(stdout);
						//ͨ��
						cJSON* pUserName = cJSON_GetObjectItem(pRoot, "username");
						char strUserName[128] ={0};
						if(pUserName)
							memcpy(strUserName,pUserName->valuestring,strlen(pUserName->valuestring)+1);
						cJSON* pToken = cJSON_GetObjectItem(pRoot, "token");
						char strToken[128] ={0};
						if(pToken) memcpy(strToken,pToken->valuestring,strlen(pToken->valuestring)+1);
						cJSON* pStreamID = cJSON_GetObjectItem(pRoot, "stream_id");
						int iStreamID = -1;
						if(pStreamID) iStreamID = atoi(pStreamID->valuestring);

						cJSON* pRecallAddr = cJSON_GetObjectItem(pRoot, "recall_addr");
						char strRecallAddr[128] ={0};
						if(pRecallAddr) memcpy(strRecallAddr,pRecallAddr->valuestring,strlen(pRecallAddr->valuestring)+1);

						cJSON* pSerialNo = cJSON_GetObjectItem(pRoot,"serialno");
						
						char strSerialNo[128] ={0};
						if(pSerialNo)
							memcpy(strSerialNo,pSerialNo->valuestring,strlen(pSerialNo->valuestring)+1);

						printf("istreamid=%d usename=%s token=%s \n",iStreamID,strUserName,strToken);
						this0->m_cSM_Manager->Bindagain(strUserName,iStreamID);
						fflush(stdout);
					//	printf("---request parse \n");
						int iChannelInfo=0;
						//��manager �а�
						int ret = this0->m_cSM_Manager->Bind_OneStream(iStreamID,strUserName,strToken,&iChannelInfo);
						
						//�ظ�
						/*
							{"cmd":"stream_bind","ret_code":"0","serialno":"c0e1758d697841fa8dad428c23b867a7"}XXEE
						*/

						//��ѯ�߱���
/*
						StreamResource pTmpResource;
						memset(&pTmpResource,0,sizeof(pTmpResource));
						char strkey_value[64] = {0};
						sprintf(strkey_value,"%d",iStreamID);
						int iret = DBInterfacer::GetInstance()->FindOneStream(4,"iStreamID",strkey_value,&pTmpResource);
						int iwhetherhd;
						if(iret)
						{
							iwhetherhd = pTmpResource.iWherether_HD;
							printf("iWherether_HD = %d\n",iwhetherhd);
							fflush(stdout);
						}

*/
						//���Ļظ�
						Stream ptmpRequest;
						
						ptmpRequest.m_clientSocket = accept_fd;
						//cJSON *pRet_root;
						ptmpRequest.pRet_root = cJSON_CreateObject();
						ptmpRequest.Requst_Json_str(2,"cmd","stream_bind");
					
						char txt[32] ={0};
						sprintf(txt,"%d",ret);
						ptmpRequest.Requst_Json_str(2,"ret_code",txt);

						char strChannelInfo[32]={0};
						sprintf(strChannelInfo,"%d",iChannelInfo);
						ptmpRequest.Requst_Json_str(2,"ChannelInfo",strChannelInfo);
						ptmpRequest.Requst_Json_str(2,"serialno",strSerialNo);
						char csip[128] = {0};
						char csport[64] = {0};
						this0->m_cSM_Manager->Findcsaddr(csip,csport,iStreamID);
				
						ptmpRequest.Requst_Json_str(2,"strNavIP",csip);
						ptmpRequest.Requst_Json_str(2,"iNavPort",csport);
						
	/*				
						char channel_type[12] = {0};
						sprintf(channel_type,"%d",iwhetherhd);
						ptmpRequest.Requst_Json_str(2,"channel_type",channel_type);
	*/				
						ptmpRequest.Send_Jsoon_str();
						if(ret>=0)
						{
							this0->m_cSM_Manager->Userbehav(iStreamID,"B",strUserName,"stream_bind sucess",ret,NULL);
						}
						else
							this0->m_cSM_Manager->Userbehav(iStreamID,"B",strUserName,"stream_bind faild",ret,NULL);
						//����һ·�µ�����
						
						if(ret >= 0)
							this0->m_cSM_Manager->AddOneStream(Navigation,iStreamID);			
						
					}
					else if(strcmp(pcmd->valuestring, "vod_play") == 0)
					{
						printf("--Vod_call_stream request \n");
						fflush(stdout);
						//ͨ��
						cJSON* pUserName = cJSON_GetObjectItem(pRoot, "username");
						char strUserName[128] ={0};
						if(pUserName)
							memcpy(strUserName,pUserName->valuestring,strlen(pUserName->valuestring)+1);
						cJSON* pStreamID = cJSON_GetObjectItem(pRoot, "stream_id");
						int iStreamID = -1;
						if(pStreamID) iStreamID = atoi(pStreamID->valuestring);

						cJSON* pUrlAddr = cJSON_GetObjectItem(pRoot, "url");
						char strUrlAddr[128] ={0};
						if(pUrlAddr) memcpy(strUrlAddr,pUrlAddr->valuestring,strlen(pUrlAddr->valuestring)+1);

						cJSON* pSerialNo = cJSON_GetObjectItem(pRoot,"serialno");
						
						char strSerialNo[128] ={0};
						if(pSerialNo)
							memcpy(strSerialNo,pSerialNo->valuestring,strlen(pSerialNo->valuestring)+1);
						
						//vodname
						cJSON* pVodename = cJSON_GetObjectItem(pRoot,"vodname");
						
						char vodname[128] ={0};
						if(pVodename)
						memcpy(vodname,pVodename->valuestring,strlen(pVodename->valuestring)+1);
						printf("vodname = %s\n",vodname);
						fflush(stdout);
						
						//posterurl

						cJSON* pPosterurl = cJSON_GetObjectItem(pRoot,"posterurl");
						char posterurl[128] ={0};
						if(pPosterurl)
						memcpy(posterurl,pPosterurl->valuestring,strlen(pPosterurl->valuestring)+1);
						printf("posterurl = %s\n",posterurl);
						fflush(stdout);
						
						int ret=0;
						printf("======msi connect fd=%d \n",accept_fd);
						fflush(stdout);
						//��manager �е㲥			
						 ret = this0->m_cSM_Manager->AddOneVodStream(iStreamID,strUrlAddr,accept_fd,strUserName,vodname,posterurl,strSerialNo);//��Ҫ����
						
						if(ret>=0)
						{
							this0->m_cSM_Manager->Userbehav(iStreamID,"D",strUserName,"vod_play success",ret,NULL);
						}
						if(ret<0)
							this0->m_cSM_Manager->Userbehav(iStreamID,"D",strUserName,"vod_play faild",ret,NULL);
						
					}
					else if (strcmp(pcmd->valuestring, "goback") == 0)
					{
						printf("--bind_stream request \n");
						fflush(stdout);
						//ͨ��
						cJSON* pSeesid = cJSON_GetObjectItem(pRoot, "sessionid");

						int iSeessionID = 0;
						if(pSeesid) iSeessionID = atoi(pSeesid->valuestring);
	
						cJSON* pSerialNo = cJSON_GetObjectItem(pRoot,"serialno");
						char strSerialNo[128] ={0};
						//printf("parse--%s\n",pSerialNo->valuestring);
						if(pSerialNo)
							memcpy(strSerialNo,pSerialNo->valuestring,strlen(pSerialNo->valuestring)+1);

						cJSON* pMsg = cJSON_GetObjectItem(pRoot,"msg");
						char strMsg[128] ={0};
						//printf("parse--%s\n",pSerialNo->valuestring);
						if(pMsg)
							memcpy(strMsg,pMsg->valuestring,strlen(pMsg->valuestring)+1);
						cJSON* pUsername = cJSON_GetObjectItem(pRoot,"username");
						char iUsername[64] = {0};
						if(pUsername)
							memcpy(iUsername,pUsername->valuestring,strlen(pUsername->valuestring)+1);
						//iSeessionID = atoi(strSerialNo);
						
						//vod�����˳�����
						this0->m_cSM_Manager->VodStreamOver(iSeessionID,iUsername);

						this0->m_cSM_Manager->Tellcums(iUsername);

						usleep(1000*8000);
						
						pthread_mutex_lock(&this0->m_cSM_Manager->m_lockerRegion);
						SetUsedRegionID::iterator itused = this0->m_cSM_Manager->m_SetUsedRegionID.find(iSeessionID);
						if(itused != this0->m_cSM_Manager->m_SetUsedRegionID.end())
						{
							//������ʹ�õĵȻ��ͷ�
							
							char txt[128]={0};
									sprintf(txt,"%d",itused->second);
									printf("-----erase region %s success \n",txt);
									fflush(stdout);
							this0->m_cSM_Manager->m_SetUsedRegionID.erase(itused);
						}
							pthread_mutex_unlock(&this0->m_cSM_Manager->m_lockerRegion);
						this0->m_cSM_Manager->Userbehav(iSeessionID,"E",iUsername,"vod_play_over",0,NULL);
						
					}
					//�󶨳�ʱ
					else if (strcmp(pcmd->valuestring, "overtime") == 0)
					{	
						printf("bind time------");
						fflush(stdout);
						cJSON* pStreamID = cJSON_GetObjectItem(pRoot, "streamid");
						if(pStreamID->valuestring!=NULL)
						{	
							int iStreamID = -1;
							if(pStreamID) iStreamID = atoi(pStreamID->valuestring);
							this0->m_cSM_Manager->BindOverTime(iStreamID);
						}
					}
					//VOD��ͣ
					else if(strcmp(pcmd->valuestring,"vod_pause")==0)
					{
						cJSON* pStreamID = cJSON_GetObjectItem(pRoot, "stream_id");
						cJSON* pUsername = cJSON_GetObjectItem(pRoot, "username");
						//vodname
						cJSON* pVodename = cJSON_GetObjectItem(pRoot,"vodname");
						
						char vodname[128] ={0};
						if(pVodename)
						memcpy(vodname,pVodename->valuestring,strlen(pVodename->valuestring)+1);
						printf("vodname = %s\n",vodname);
						fflush(stdout);
						//posterurl

						cJSON* pPosterurl = cJSON_GetObjectItem(pRoot,"posterurl");
						char posterurl[128] ={0};
						if(pPosterurl)
						memcpy(posterurl,pPosterurl->valuestring,strlen(pPosterurl->valuestring)+1);
						printf("posterurl = %s\n",posterurl);
						fflush(stdout);
						
						if(pStreamID->valuestring!=NULL)
						{
							int iStreamID = -1;
							if(pStreamID) iStreamID = atoi(pStreamID->valuestring);
							char iUsername[32] = {0};
							strcpy(iUsername,pUsername->valuestring);
							printf("----1234iUsername = %s\n",iUsername);
							fflush(stdout);
							this0->m_cSM_Manager->PauseVOD(iStreamID,iUsername,vodname,posterurl);
						}
						
						Stream ptmpRequest;

						ptmpRequest.m_clientSocket = accept_fd;
						//cJSON *pRet_root;
						ptmpRequest.pRet_root = cJSON_CreateObject();
						ptmpRequest.Requst_Json_str(2,"cmd","vod_pause");
					
						char txt[32] ={0};
						sprintf(txt,"%d",0);
						ptmpRequest.Requst_Json_str(2,"ret_code",txt);

						ptmpRequest.Requst_Json_str(2,"serialno","ade8976d88d76");
						ptmpRequest.Send_Jsoon_str();
						
				   }
					//��ͣ��ʱ
				  else if(strcmp(pcmd->valuestring,"pause")==0)
				  {
						char iUsername[32] = {0};
						cJSON* pStreamID = cJSON_GetObjectItem(pRoot, "streamid");
						cJSON* pSUserID = cJSON_GetObjectItem(pRoot, "username");
						if(pStreamID->valuestring!=NULL)
						{	
							int iStreamID = -1;
							if(pStreamID) iStreamID = atoi(pStreamID->valuestring);
							strcpy(iUsername,pSUserID->valuestring);
							this0->m_cSM_Manager->BindOverTime(iStreamID);
							this0->m_cSM_Manager->Tellcums(iUsername);
						}
				  }
				  else if(strcmp(pcmd->valuestring,"vod_resume")==0)
				  {
						
						cJSON* pStreamID = cJSON_GetObjectItem(pRoot, "stream_id");
						if(pStreamID->valuestring!=NULL)
						{	
							int iStreamID = -1;
							printf("1111111111111112222streamid = %s",pStreamID->valuestring);
							fflush(stdout);
							if(pStreamID) iStreamID = atoi(pStreamID->valuestring);
							this0->m_cSM_Manager->RecoverVodPlay(iStreamID,NULL,NULL,NULL);
						}
						Stream ptmpRequest;

						ptmpRequest.m_clientSocket = accept_fd;
						//cJSON *pRet_root;
						ptmpRequest.pRet_root = cJSON_CreateObject();
						ptmpRequest.Requst_Json_str(2,"cmd","vod_resume");
						
						char txt[32] ={0};
						sprintf(txt,"%d",0);
						ptmpRequest.Requst_Json_str(2,"ret_code",txt);
						ptmpRequest.Requst_Json_str(2,"serialno","ade8976d88d76");
						ptmpRequest.Send_Jsoon_str();
				  }
				 else if (strcmp(pcmd->valuestring,"stream_unbind") == 0)
				 {	
					printf("unbind time------");
					fflush(stdout);
					char iusernam[128] = {0};
					char ustreamid[32] = {0};
					int istreamid = 0;
					cJSON* pStreamID = cJSON_GetObjectItem(pRoot, "stream_id");
					if(pStreamID)
					{
						if(pStreamID->valuestring)
						{
							strcpy(ustreamid,pStreamID->valuestring);
							istreamid = atoi(ustreamid);
						}
					}
					cJSON* pStreamuser = cJSON_GetObjectItem(pRoot, "username");
					if(pStreamuser)
					{
						if(pStreamuser->valuestring)
						strcpy(iusernam,pStreamuser->valuestring);
					}
					char iserialno[64]={0};
					cJSON* pSerialno = cJSON_GetObjectItem(pRoot, "iserialno");
					if(pSerialno)
					{
						if(pSerialno->valuestring)
						strcpy(iserialno,pSerialno->valuestring);
					}
		
					this0->m_cSM_Manager->Unbind(iusernam,ustreamid);
					this0->m_cSM_Manager->CleanStream(istreamid);
					usleep(1000*1800);
					this0->m_cSM_Manager->AddOneAdvStream(istreamid);

					Stream ptmpRequest;
					ptmpRequest.m_clientSocket = accept_fd;
						//cJSON *pRet_root;
					ptmpRequest.pRet_root = cJSON_CreateObject();
					ptmpRequest.Requst_Json_str(2,"cmd","stream_unbind");
					
					int ret = 0;
					char txt[32] ={0};
					sprintf(txt,"%d",ret);
					ptmpRequest.Requst_Json_str(2,"ret_code",txt);
					ptmpRequest.Requst_Json_str(2,"serialno",iserialno);
					ptmpRequest.Send_Jsoon_str();
					
				}		 
				else if(strcmp(pcmd->valuestring,"quit")==0)
				{
					cJSON* pStreamID = cJSON_GetObjectItem(pRoot, "stream_id");
					cJSON* pUsername = cJSON_GetObjectItem(pRoot, "username");
					if(pStreamID->valuestring!=NULL)
					{
						int iStreamID = -1;
						if(pStreamID) iStreamID = atoi(pStreamID->valuestring);
						char iUsername[32] = {0};
						strcpy(iUsername,pUsername->valuestring);
						this0->m_cSM_Manager->PauseVOD(iStreamID,iUsername,NULL,NULL,1);
					}
					
					Stream ptmpRequest;
					
					ptmpRequest.m_clientSocket = accept_fd;
					//cJSON *pRet_root;
					ptmpRequest.pRet_root = cJSON_CreateObject();
					ptmpRequest.Requst_Json_str(2,"cmd","quit");
				
					char txt[32] ={0};
					sprintf(txt,"%d",0);
					ptmpRequest.Requst_Json_str(2,"ret_code",txt);

					ptmpRequest.Requst_Json_str(2,"serialno","ade8976d88d76");
					ptmpRequest.Send_Jsoon_str();
						
				}
				
				else if(strcmp(pcmd->valuestring,"recovery")==0)
				 {
						
					cJSON* pStreamID = cJSON_GetObjectItem(pRoot, "stream_id");
					int iStreamID = -1;
					if(pStreamID->valuestring)
					{
						printf("----this is Recovery streamid = %s\n",pStreamID->valuestring);
						fflush(stdout);
						if(pStreamID) iStreamID = atoi(pStreamID->valuestring);
					}
					//username
					char iusername[64]={0};
					cJSON* pUsername = cJSON_GetObjectItem(pRoot, "username");
					if(pUsername)
					{
						memcpy(iusername,pUsername->valuestring,strlen(pUsername->valuestring)+1);
					}	
					printf("----4567iUsername = %s\n",iusername);
					fflush(stdout);
					//vodname
					char ivodname[128] = {0};
					cJSON* pVodname = cJSON_GetObjectItem(pRoot, "vodname");
					if(pVodname)
					{
						memcpy(ivodname,pVodname->valuestring,strlen(pVodname->valuestring)+1);
					}

					//posterurl
					char iposterurl[128] = {0};
					cJSON* pPosterurl = cJSON_GetObjectItem(pRoot, "posterurl");
					if(pPosterurl)
					{
						memcpy(iposterurl,pPosterurl->valuestring,strlen(pPosterurl->valuestring)+1);
					}
					
					this0->m_cSM_Manager->RecoverVodPlay(iStreamID,iusername,ivodname,iposterurl);
					
					Stream ptmpRequest;

					ptmpRequest.m_clientSocket = accept_fd;
					//cJSON *pRet_root;
					ptmpRequest.pRet_root = cJSON_CreateObject();
					ptmpRequest.Requst_Json_str(2,"cmd","recovery");
						
					char txt[32] ={0};
					sprintf(txt,"%d",0);
					ptmpRequest.Requst_Json_str(2,"ret_code",txt);
					ptmpRequest.Requst_Json_str(2,"serialno","ade8976d88d76");
					ptmpRequest.Send_Jsoon_str();
				 }
			}
		}
			
	}

		close(accept_fd);

}

void *SM_Control::ts_recv_MSI_thread(void *arg)
{

	SM_Control *this0 = (SM_Control*)arg;

	int BUFFER_SIZE = 4096;
	struct sockaddr_in s_addr;
	int sock;
	socklen_t addr_len;


	if ( (sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))  == -1) {
		perror("socket");
		return NULL;
	} else
		printf("create socket.\n\r");
		fflush(stdout);
	memset(&s_addr, 0, sizeof(struct sockaddr_in));
	s_addr.sin_family = AF_INET;

	s_addr.sin_port = htons(this0->m_iMsiPort);


	s_addr.sin_addr.s_addr = INADDR_ANY;

	int optval = 1;
	if ((setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(int))) == -1)
	{
		close(sock);
		return NULL;
	}

	if ( (bind(sock, (struct sockaddr*)&s_addr, sizeof(s_addr))) == -1 ) {
		perror("bind");
		return NULL;
	}else
		printf("bind address to socket.\n\r");
		fflush(stdout);
	if(listen(sock,10)<0)
	{
		perror("listen");
		return NULL;
	}

	int accept_fd = -1;
	struct sockaddr_in remote_addr;
	memset(&remote_addr,0,sizeof(remote_addr));

	this0->m_MsiacceptSocket.clear();
	
	while( 1 )
	{  
		
	    socklen_t sin_size = sizeof(struct sockaddr_in); 
		if(( accept_fd = accept(sock,(struct sockaddr*) &remote_addr,&sin_size)) == -1 )  
         {  
             	printf( "Accept error!\n");  
				fflush(stdout);
                //continue;  
	     }  
        printf("Received a connection from %s\n",(char*) inet_ntoa(remote_addr.sin_addr));  
		fflush(stdout);
		//m_ispause.push_back(iStreamID);
		this0->m_MsiacceptSocket.push_back(accept_fd);
		pthread_t tcp_recv_thread1;
		pthread_create(&tcp_recv_thread1, NULL, Parse_recv_MSI_thread, this0);
		pthread_detach(tcp_recv_thread1);
		

	}
}
void *SM_Control::Parse_recv_clear_thread(void * arg)
{
	SM_Control *this0 = (SM_Control*)arg;
	
	int len;
	char Rcv_buf[4096];
	
	cJSON *pcmd = NULL;
	
	char cJsonBuff[1024 * 2];
	int iRecvLen = 0;
	int accept_fd = this0->m_ClearstremSocket;
	while(1)
	{
		memset(Rcv_buf,0,sizeof(Rcv_buf));
		int length = 0;
		int i_rc = 0, i_count = 0;
		do
		{
			i_rc = recv(accept_fd, Rcv_buf + i_count, 2000 - i_count, 0);
			if (i_rc <= 0)break;//�쳣�ر�
			i_count += i_rc;
		} while (strstr(Rcv_buf, "XXEE") == NULL);
		iRecvLen = i_count;
		if (iRecvLen <= 0) break;	
		printf("recv:%s \n",Rcv_buf);
		fflush(stdout);
		//������������
		replace(Rcv_buf, "XXEE", "");
		cJSON* pRoot = cJSON_Parse(Rcv_buf);	
		if(pRoot)
		{
			pcmd = cJSON_GetObjectItem(pRoot, "cmd");
			if (strcmp(pcmd->valuestring, "clean") == 0)
			{
							
				printf("bind time------");
				fflush(stdout);
				cJSON* pStreamID = cJSON_GetObjectItem(pRoot, "streamid");
				cJSON* pStreamtype = cJSON_GetObjectItem(pRoot,"streamtype");
				if(pStreamID->valuestring!=NULL)
				{	
					int iStreamID = -1;
					int type = -1;
					if(pStreamID) 
					iStreamID = atoi(pStreamID->valuestring);
					if(pStreamtype) 
					type = atoi(pStreamtype->valuestring);
					this0->m_cSM_Manager->ClearoneStream(iStreamID,type);	
				}
			}
					
		}
		
		if(this0->m_ClearstremSocket != -1)
			close(this0->m_ClearstremSocket);
			this0->m_ClearstremSocket = -1;
	}
}

void *SM_Control::ts_recv_Clear_thread(void *arg)
{
	SM_Control *this0 = (SM_Control*)arg;
	
	int BUFFER_SIZE = 4096;
	struct sockaddr_in s_addr;
	int sock;
	socklen_t addr_len;


	if ( (sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))  == -1) {
		perror("socket");
		return NULL;
	} else
		printf("create socket.\n\r");
		fflush(stdout);
	memset(&s_addr, 0, sizeof(struct sockaddr_in));
	s_addr.sin_family = AF_INET;

	s_addr.sin_port = htons(this0->m_iCleanPort);

	s_addr.sin_addr.s_addr = INADDR_ANY;

	int optval = 1;
	if ((setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(int))) == -1)
	{
		close(sock);
		return NULL;
	}

	if ( (bind(sock, (struct sockaddr*)&s_addr, sizeof(s_addr))) == -1 ) {
		perror("bind");
		return NULL;
	}else
		printf("bind address to socket.\n\r");
		fflush(stdout);
	if(listen(sock,10)<0)
	{
		perror("listen");
		return NULL;
	}

	int accept_fd = -1;
	struct sockaddr_in remote_addr;
	memset(&remote_addr,0,sizeof(remote_addr));

	this0->m_ClearstremSocket = -1;
	
	while( 1 )
	{  
		
	    socklen_t sin_size = sizeof(struct sockaddr_in); 
		if(( accept_fd = accept(sock,(struct sockaddr*) &remote_addr,&sin_size)) == -1 )  
         {  
             	printf( "Accept error!\n");  
				fflush(stdout);
                //continue;  
	     }  
         printf("Received a connection from %s\n",(char*) inet_ntoa(remote_addr.sin_addr));  
		fflush(stdout);
		if(this0->m_ClearstremSocket != -1)
		{
			//��Ҫ�ͷ�֮ǰ������
		}
		this0->m_ClearstremSocket = accept_fd;
		pthread_t tcp_recv_thread1;
		pthread_create(&tcp_recv_thread1, NULL, Parse_recv_clear_thread, this0);
		pthread_detach(tcp_recv_thread1);
		

	}
}


void *SM_Control::ts_recv_useract_thread(void *arg)
{
	SM_Control *this0 = (SM_Control*)arg;
	printf("1111111111111111this is ts_recv_useract_thread\n");
	fflush(stdout);
	int BUFFER_SIZE = 4096;
	struct sockaddr_in s_addr;
	int sock;
	socklen_t addr_len;
	
	if ((sock = socket(AF_INET,SOCK_DGRAM,0))  == -1){
		perror("socket");
		return NULL;
	} else
		printf("create socket.\n\r");
		fflush(stdout);
	memset(&s_addr, 0, sizeof(struct sockaddr_in));
	s_addr.sin_family = AF_INET;

	s_addr.sin_port = htons(this0->m_iUseractport);

	s_addr.sin_addr.s_addr = INADDR_ANY;

	int optval = 1;
	if ((setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(int))) == -1)
	{
		close(sock);
		printf("11111111111111111111111 setsockopt error\n");
		fflush(stdout);
		return NULL;
	}

	if ((bind(sock, (struct sockaddr*)&s_addr, sizeof(s_addr))) == -1 ) {
		perror("bind");
		return NULL;
	}else
		printf("bind address to socket.\n\r");
	    fflush(stdout);
		char Rcv_buf[4096];
		cJSON *pcmd = NULL;
		socklen_t address_size;	
		char cJsonBuff[1024*2];
		int iRecvLen = 0;
		struct sockaddr_in rin;
		int accept_fd = sock;		
		while(1)
		{
			memset(Rcv_buf,0,sizeof(Rcv_buf));
			int length = 0;
			int i_rc = 0, i_count = 0;
			address_size = sizeof(rin);
			do
			{
				printf("222222222222222222before recv\n");
				fflush(stdout);
				i_rc = recvfrom(accept_fd, Rcv_buf + i_count, 2000 - i_count,NULL,(struct sockaddr *)&rin,&address_size);
				if (i_rc <= 0)break;//�쳣�ر�
				i_count += i_rc;
			} while (strstr(Rcv_buf, "XXEE") == NULL);	
			iRecvLen = i_count;
			if (iRecvLen <= 0) break;
			
			printf("recv:%s \n",Rcv_buf);
			fflush(stdout);
							//������������
			replace(Rcv_buf, "XXEE", "");	
			cJSON* pRoot = cJSON_Parse(Rcv_buf);
			if(pRoot)
			{
				pcmd = cJSON_GetObjectItem(pRoot, "cmd");
				if(strcmp(pcmd->valuestring,"user_action")==0)
				{
					char iUsername[32] = {0};
					char iUsertype[32] = {0};
					char iUserdate[64] = {0};
					char iUsermsg[32] = {0};
					int iStreamID;
					int result;
					cJSON* pUsertype = cJSON_GetObjectItem(pRoot, "action_type");
					cJSON* pStreamID = cJSON_GetObjectItem(pRoot, "stream_id");
					cJSON* pSUserID = cJSON_GetObjectItem(pRoot, "username");
					cJSON* pUserdate = cJSON_GetObjectItem(pRoot, "action_date");
					cJSON* pUserresult = cJSON_GetObjectItem(pRoot, "action_result");
					cJSON* pUsermsg = cJSON_GetObjectItem(pRoot, "msg");
					if(pSUserID)
					strcpy(iUsername,pSUserID->valuestring);
					if(pUsertype)
					strcpy(iUsertype,pUsertype->valuestring);
					if(pUserdate)
					strcpy(iUserdate,pUserdate->valuestring);
					if(pUsermsg)
					strcpy(iUsermsg,pUsermsg->valuestring);
					if(pUserresult) result = pUserresult->valueint;
					if(pStreamID)
					{
						if(pStreamID->valuestring == NULL)
						{
							iStreamID = 0;
						}
						else
						iStreamID = atoi(pStreamID->valuestring);
					}
					else
					{
						iStreamID = 0;
					}
					printf("77777777777777begin to insert action to DB\n");
					fflush(stdout);
					this0->m_cSM_Manager->Userbehav(iStreamID,iUsertype,iUsername,iUsermsg,result,iUserdate);
				}
			}	
		}
}

void *SM_Control::ts_checksession_thread(void *arg)
{
	SM_Control *this0 = (SM_Control*)arg;
	printf("this is ts_checksession_thread\n");
	fflush(stdout);
	while(1)
	{
		usleep(iwaittime);
		printf("begin to check the session\n");
		fflush(stdout);
		this0->m_cSM_Manager->Checksession();
		printf("check session ok");
		fflush(stdout);
	}
}
