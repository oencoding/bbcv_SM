#include "Switch_Control.h"
#include "Stream.h"
#include "Log.h"



Switch_Control::Switch_Control()
{
	m_pSwitchManager = NULL;

	m_iMsiPort = iMSIServerPort;


	pthread_mutex_init(&ManagerFactory::instance()->m_mutexServerList, NULL);

}
Switch_Control::~Switch_Control()
{
	if(m_pSwitchManager)
		delete m_pSwitchManager;
	m_pSwitchManager = NULL;

}

bool Switch_Control::Init()
{

	return true;
}

int Switch_Control::ConnectToSever(const char* phostIp,int iPort,int *pCurrent_num,int *pMax_num)
{
	struct sockaddr_in s_addr;
	int sockid;
	socklen_t addr_len;

    sockid=socket(AF_INET,SOCK_STREAM,0);
	    
    s_addr.sin_family = AF_INET ;

   	s_addr.sin_addr.s_addr = inet_addr(phostIp) ;
    s_addr.sin_port=htons((unsigned short)iPort);
    fcntl(sockid,F_SETFD, FD_CLOEXEC);
 
/*	 if (-1 == fcntl(sockid, F_SETFL, O_NONBLOCK))
	 {
		 printf("fcntl socket error!\n");
		 fflush(stdout);
		 return -1;
	 }	 
*/
	unsigned long ul = 1;
	ioctl(sockid, FIONBIO, &ul); //����Ϊ������ģʽ
	struct timeval timeout={10,1000*500}; //1��
	int len = sizeof(timeout);
	setsockopt(sockid,SOL_SOCKET,SO_SNDTIMEO, &timeout,len);
	setsockopt(sockid,SOL_SOCKET,SO_RCVTIMEO,&timeout,len);

	fd_set set;
 	printf("---begin connect \n");
    //�ظ�����3�Σ���ֹ�жϵ�ԭ���µ��쳣
    for(int i=0;i<3;i++)
    {
    	bool ret = false;
		int iret = -1;
		iret = connect(sockid,(struct sockaddr *)&s_addr,(int)(sizeof(s_addr)));
  		if(iret == -1)
   		{
   			struct timeval tm;
     		tm.tv_sec  = 5;
     		tm.tv_usec = 1000*500;
     		FD_ZERO(&set);
     		FD_SET(sockid, &set);
			int error=-1;
     		if( select(sockid+1, NULL, &set, NULL, &tm) > 0)
     		{
       			getsockopt(sockid, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len);
       			if(error == 0) 
				{
					ret = true;
					printf("---error no \n");
					//break;
   				}
 				else ret = false;
    		} else ret = false;
  		}
 		else if(iret==0)
		{	
			
			break;
		}
    	 if(i==2) 
    	 {
    	 	  //cout<<"connect Error "<<p_host<<":"<<p_Port<<endl;
    	 	  //perror("::connect");
    	 	  printf("--connect error \n");
			  LOG_ERROR("ERROR  - [SWM]: connect error  \n");
    	 	  ::close(sockid);
    	 	  return -1;
    	 }
		// printf("---sleep \n");
    	 usleep(1000);
    	 //cout<<"connect again: "<<p_host<<":"<<p_Port<<endl;
    }
	ul = 0;
	ioctl(sockid, FIONBIO, &ul); //����Ϊ����ģʽ
	printf("---connect success \n");
    int     optval = 1;
   // setsockopt(sockid,SOL_SOCKET,SO_KEEPALIVE,(char *)(&optval),sizeof(optval));

	char alivetick[1024]={0};
	int iData=0;
	char strdata[32]={0};
	sprintf(strdata,"%d",iData);
  	//��������
	Stream ptmpRequest;

	ptmpRequest.m_clientSocket = sockid;
	//cJSON *pRet_root;
	ptmpRequest.pRet_root = cJSON_CreateObject();
	ptmpRequest.Requst_Json_str(2,"cmd","alivetick");
	ptmpRequest.Requst_Json_str(2,"dataid",strdata);
	ptmpRequest.Send_Jsoon_str();

	//LOG_INFO("INFO  - [SWM]: send alive \n");
		
	memset(alivetick,0,sizeof(alivetick));
	int length = 0;
	int i_rc = 0, i_count = 0;
	int iRecvLen = 0;
	do
	{
		i_rc = recv(sockid, alivetick + i_count, 2000 - i_count, 0);
		if (i_rc <= 0)break;//�쳣�ر�
		i_count += i_rc;
	} while (strstr(alivetick, "XXEE") == NULL);
	iRecvLen = i_count;
	
	if (iRecvLen <= 0) { 
		::close(sockid);
		LOG_ERROR("ERROR  - [SWM]: Recv error  \n");
		return -1;
	}
	alivetick[iRecvLen]='\0';

	LOG_INFO_FORMAT("INFO  - [SWM]: tcp recved %d bytes :[%s] \n",iRecvLen,alivetick);
		
	printf("recv:%s \n",alivetick);
	fflush(stdout);

	cJSON *pcmd = NULL;

			//������������
	replace(alivetick, "XXEE", "");
	cJSON* pRoot = cJSON_Parse(alivetick);

	if (pRoot)
	{
		pcmd = cJSON_GetObjectItem(pRoot, "cmd");
		if (pcmd)
		{
			//�ж���������
			if (strcmp(pcmd->valuestring, "alivetick") == 0)
			{
				//ͨ��
				printf("--alive tick repay \n");
				cJSON* pDataid = cJSON_GetObjectItem(pRoot, "dataid");
				char strDataid[128] ={0};
				if(pDataid)
					memcpy(strDataid,pDataid->valuestring,strlen(pDataid->valuestring)+1);
				int iRecvData = atoi(strDataid);
				cJSON* pcurrent = cJSON_GetObjectItem(pRoot, "current_num");
				char strcurrent[128] ={0};
				if(pcurrent)
					memcpy(strcurrent,pcurrent->valuestring,strlen(pcurrent->valuestring)+1);
				int icurrentnum = atoi(strcurrent);
				cJSON* pmaxnum = cJSON_GetObjectItem(pRoot, "max_num");
				char strmax[128] ={0};
				if(pmaxnum)
					memcpy(strmax,pmaxnum->valuestring,strlen(pmaxnum->valuestring)+1);
				int imaxtnum = atoi(strmax);

				*pCurrent_num = icurrentnum;
				*pMax_num = imaxtnum;
				if(iRecvData == (iData +1))
				{	
					//��������
					::close(sockid);
					//printf("---- alive keep \n");
					return 0;
				}
			}
		}

	}

	::close(sockid);

	return -1;
}

void *Switch_Control::ts_recv_Control_thread(void *arg)
{

	Switch_Control *this0 = (Switch_Control*)arg;

	struct timeval tv1,tv2;
	long long time1,time2;
	long long nsendbytes = 0;
	long long totaltime = 0;
	long now_bit_rate = 0;
	gettimeofday(&tv1, NULL);
	time1 = tv1.tv_sec*1000 + tv1.tv_usec / 1000;
	bool iFirstTime = true;

	//long bit_rate = this0->m_iSendRate;
	while(1)
	{
		if(!iFirstTime)
		{
			gettimeofday(&tv2, NULL);
			time2 = tv2.tv_sec*1000 + tv2.tv_usec / 1000;
			if(time2 - time1 < this0->iAliveTickInterval)
			{
				usleep(500*1000);	
				//printf("now_bit_rate is too large %d %d\n",now_bit_rate,this0->m_iSendRate);
				continue;
			}
			printf("------%ld \n",time2);
			time1 = time2;
		}
		iFirstTime = false;

		printf("----switch sever size =%d \n",ManagerFactory::instance()->m_mapSwitchServerInfo.size());
		//����������

		MapSwitchSeverInfo::iterator iterLook = ManagerFactory::instance()->m_mapSwitchServerInfo.begin();
		while(iterLook != ManagerFactory::instance()->m_mapSwitchServerInfo.end())
		{
			printf("---send alive msg\n");
			//
			SwitchSeverInfo *pTmpSeverInfo = iterLook->second;
			if(pTmpSeverInfo)
			{
				int iCurrent_num = 0;
				int iMax_num = 0;
				int iRet = this0->ConnectToSever(pTmpSeverInfo->strServerIPAddr,pTmpSeverInfo->iListenPort,&iCurrent_num,&iMax_num);
				printf("---send %s alive end current=%d max =%d \n",pTmpSeverInfo->strServerIPAddr,iCurrent_num,iMax_num);

				pthread_mutex_lock(&ManagerFactory::instance()->m_mutexServerList);
				printf("---in lock \n");
				if(iRet == 0)
				{
					if(pTmpSeverInfo->iRunStatus > iKEEPALIVETIME)
					{
						//�ɵ���תΪ���� ��Ҫ����������
						int iret = -1;
						printf("---need reset message \n");
						
						iret = this0->SendResetMessage(pTmpSeverInfo->strServerIPAddr,pTmpSeverInfo->iListenPort);
						if(iret == 0)
						{
							//���óɹ�
							printf("---success reset message \n");
							LOG_INFO_FORMAT("INFO  - [SWM]:������[%s] �������ɹ� \n",pTmpSeverInfo->strServerIPAddr);
							pTmpSeverInfo->iRunStatus = 0;
							pTmpSeverInfo->iStreamStatus = 0;
							if(iCurrent_num != 0 || iMax_num != 0)
							{
							//	pTmpSeverInfo->iCurrentSwitchNumber = iCurrent_num;
							//	pTmpSeverInfo->iMaxSwitchNumber = iMax_num;
							}
							
						}
						else
						{
							pTmpSeverInfo->iStreamStatus = 1;
							LOG_INFO_FORMAT("INFO  - [SWM]:������[%s] ������ʧ�� \n",pTmpSeverInfo->strServerIPAddr);
							if(iCurrent_num != 0 || iMax_num != 0)
							{
							//	pTmpSeverInfo->iCurrentSwitchNumber = iCurrent_num;
							//	pTmpSeverInfo->iMaxSwitchNumber = iMax_num;
							}

						}
					}
					else
					{
						pTmpSeverInfo->iRunStatus = 0;
						if(iCurrent_num != 0 || iMax_num != 0)
						{
						//	pTmpSeverInfo->iCurrentSwitchNumber = iCurrent_num;
						//	pTmpSeverInfo->iMaxSwitchNumber = iMax_num;
						}

					}
				}
				else
				{
					pTmpSeverInfo->iRunStatus++;
					if(pTmpSeverInfo->iRunStatus <= iKEEPALIVETIME)
						LOG_WARN_FORMAT("WARN  - [SWM]: %s alive lost   \n",pTmpSeverInfo->strServerIPAddr);
					printf("----no alive %s \n",pTmpSeverInfo->strServerIPAddr);
				}
				
				if(pTmpSeverInfo->iRunStatus > iKEEPALIVETIME)
				{
					
					LOG_INFO_FORMAT("INFO  - [SWM]:������[%s] ���� \n",pTmpSeverInfo->strServerIPAddr);
					if(pTmpSeverInfo->iRunStatus > 200000) 
						pTmpSeverInfo->iRunStatus = 6;
				}
				
				pthread_mutex_unlock(&ManagerFactory::instance()->m_mutexServerList);
				printf("---out lock \n");
			}
			
			++iterLook;
		}

	}


}

void *Switch_Control::Parse_recv_MSI_thread(void * arg)
{
	Switch_Control *this0 = (Switch_Control*)arg;

	int len;
	char Rcv_buf[4096];

	cJSON *pcmd = NULL;

	char cJsonBuff[1024 * 2];
	int iRecvLen = 0;
	int accept_fd = this0->m_MsiacceptSocket;
	//while(1)
	{
			if(accept_fd == -1)
			{
				printf("error accept socket in msi\n");
				fflush(stdout);
				//break;
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
			if (iRecvLen <= 0) return NULL;

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
						printf("--Add_stream request \n");
						fflush(stdout);
						//ͨ��
						cJSON* psessionid = cJSON_GetObjectItem(pRoot, "sessionid");
						char strsessionid[128] ={0};
						if(psessionid)
							memcpy(strsessionid,psessionid->valuestring,strlen(psessionid->valuestring)+1);
						cJSON* pToken = cJSON_GetObjectItem(pRoot, "input_url");
						char input_url[128] ={0};
						if(pToken) memcpy(input_url,pToken->valuestring,strlen(pToken->valuestring)+1);

						char output_url[128] ={0};
						cJSON* pStreamID = cJSON_GetObjectItem(pRoot, "output_url");
						if(pStreamID) memcpy(output_url,pStreamID->valuestring,strlen(pStreamID->valuestring)+1);

						cJSON* pSerialNo = cJSON_GetObjectItem(pRoot,"serialno");

						char strSerialNo[128] ={0};
						if(pSerialNo)
							memcpy(strSerialNo,pSerialNo->valuestring,strlen(pSerialNo->valuestring)+1);

						//printf("istreamid=%d usename=%s token=%s \n",iStreamID,strUserName,strToken);
					//	printf("---request parse \n");
						int ret = this0->m_pSwitchManager->AddOneSwitch(input_url,output_url,strsessionid,input_url);
						//�ظ�
						/*
							{"cmd":"stream_bind","ret_code":"0","serialno":"c0e1758d697841fa8dad428c23b867a7"}XXEE
						*/
						//���Ļظ�
						Stream ptmpRequest;

						ptmpRequest.m_clientSocket = accept_fd;
						//cJSON *pRet_root;
						ptmpRequest.pRet_root = cJSON_CreateObject();
						ptmpRequest.Requst_Json_str(2,"cmd","add_ads_stream");
						ptmpRequest.Requst_Json_str(2,"sessionid",strsessionid);

						ptmpRequest.Requst_Json_str(1,"task_id",strsessionid);
						char txt[32] ={0};
						sprintf(txt,"%d",ret);
						ptmpRequest.Requst_Json_str(2,"ret_code",txt);

						ptmpRequest.Requst_Json_str(2,"serialno",strSerialNo);
						ptmpRequest.Send_Jsoon_str();

					}
					else if(strcmp(pcmd->valuestring, "del_ads_stream") == 0)
					{
						printf("--Del_stream request \n");
						fflush(stdout);
						//ͨ��
						cJSON* psessionid = cJSON_GetObjectItem(pRoot, "sessionid");
						char strsessionid[128] ={0};
						if(psessionid)
							memcpy(strsessionid,psessionid->valuestring,strlen(psessionid->valuestring)+1);

						cJSON* pSerialNo = cJSON_GetObjectItem(pRoot,"serialno");

						char strSerialNo[128] ={0};
						if(pSerialNo)
							memcpy(strSerialNo,pSerialNo->valuestring,strlen(pSerialNo->valuestring)+1);

						int ret =0;// this0->m_pSwitchManager->DeleteOneSwitch(strsessionid);
						//�ظ�
						/*
							{"cmd":"stream_bind","ret_code":"0","serialno":"c0e1758d697841fa8dad428c23b867a7"}XXEE
						*/
						//���Ļظ�
						Stream ptmpRequest;

						ptmpRequest.m_clientSocket = accept_fd;
						//cJSON *pRet_root;
						ptmpRequest.pRet_root = cJSON_CreateObject();
						ptmpRequest.Requst_Json_str(2,"cmd","del_ads_stream");

						ptmpRequest.Requst_Json_str(2,"sessionid",strsessionid);



						char txt[32] ={0};
						sprintf(txt,"%d",ret);
						ptmpRequest.Requst_Json_str(2,"ret_code",txt);

						ptmpRequest.Requst_Json_str(2,"serialno",strSerialNo);
						ptmpRequest.Send_Jsoon_str();
					}
					else if(strcmp(pcmd->valuestring, "reset_device") == 0)
					{
						printf("--Del_stream request \n");

						fflush(stdout);
						//{"cmd":"reset_device","returnCode":"0","serialno":"131117ac2d6843b9bf4774e258ab2248"}XXEE
						//ͨ��
						cJSON* pSerialNo = cJSON_GetObjectItem(pRoot,"serialno");

						char strSerialNo[128] ={0};
						if(pSerialNo)
							memcpy(strSerialNo,pSerialNo->valuestring,strlen(pSerialNo->valuestring)+1);

						int ret =0;// this0->m_pSwitchManager->DeleteAllWsitch();
						//�ظ�
						/*
							{"cmd":"stream_bind","ret_code":"0","serialno":"c0e1758d697841fa8dad428c23b867a7"}XXEE
						*/
						//���Ļظ�
						Stream ptmpRequest;

						ptmpRequest.m_clientSocket = accept_fd;
						//cJSON *pRet_root;
						ptmpRequest.pRet_root = cJSON_CreateObject();
						ptmpRequest.Requst_Json_str(2,"cmd","reset_device");

						char txt[32] ={0};
						sprintf(txt,"%d",ret);
						ptmpRequest.Requst_Json_str(2,"ret_code",txt);

						ptmpRequest.Requst_Json_str(2,"serialno",strSerialNo);
						ptmpRequest.Send_Jsoon_str();
					}
					else if (strcmp(pcmd->valuestring, "change_stream") == 0)
					{
						printf("--Change_stream request \n");
						fflush(stdout);
						cJSON* old_psessionid = cJSON_GetObjectItem(pRoot, "old_sessionid");
						char old_strsessionid[128] ={0};
						if(old_psessionid)
							memcpy(old_strsessionid,old_psessionid->valuestring,strlen(old_psessionid->valuestring)+1);
						cJSON* psessionid = cJSON_GetObjectItem(pRoot, "sessionid");
						char strsessionid[128] ={0};
						if(psessionid)
							memcpy(strsessionid,psessionid->valuestring,strlen(psessionid->valuestring)+1);
						cJSON* pInterval = cJSON_GetObjectItem(pRoot, "interval");
						char strInterval[128] ={0};
						if(pInterval)
							memcpy(strInterval,pInterval->valuestring,strlen(pInterval->valuestring)+1);


						cJSON* pToken = cJSON_GetObjectItem(pRoot, "input_url");
						char input_url[128] ={0};
						if(pToken) memcpy(input_url,pToken->valuestring,strlen(pToken->valuestring)+1);

						char output_url[128] ={0};
						cJSON* pStreamID = cJSON_GetObjectItem(pRoot, "output_url");
						if(pStreamID) memcpy(output_url,pStreamID->valuestring,strlen(pStreamID->valuestring)+1);

						cJSON* pSerialNo = cJSON_GetObjectItem(pRoot,"serialno");

						char strSerialNo[128] ={0};
						if(pSerialNo)
							memcpy(strSerialNo,pSerialNo->valuestring,strlen(pSerialNo->valuestring)+1);

						int ret = 0;//this0->m_pSwitchManager->DeleteOneSwitch(old_strsessionid);
						//usleep(1000*500);
						int iInterval = atoi(strInterval);
						iInterval = iInterval*1000*1000;
						usleep(iInterval);
						ret = this0->m_pSwitchManager->AddOneSwitch(input_url,output_url,strsessionid,input_url);

						//���Ļظ�
						Stream ptmpRequest;

						ptmpRequest.m_clientSocket = accept_fd;
						//cJSON *pRet_root;
						ptmpRequest.pRet_root = cJSON_CreateObject();
						ptmpRequest.Requst_Json_str(2,"cmd","change_stream");

						char txt[32] ={0};
						sprintf(txt,"%d",ret);
						ptmpRequest.Requst_Json_str(2,"ret_code",txt);

						ptmpRequest.Requst_Json_str(2,"serialno",strSerialNo);
						ptmpRequest.Send_Jsoon_str();


					}
					else if(strcmp(pcmd->valuestring, "check_session") == 0)
					{
						printf("--check_session \n");
						//ͨ��
						cJSON* psessionid = cJSON_GetObjectItem(pRoot, "sessionid");
						char strsessionid[128] ={0};
						int iSeessionID = 0;
						if(psessionid)
						{
							memcpy(strsessionid,psessionid->valuestring,strlen(psessionid->valuestring)+1);
							iSeessionID = atoi(strsessionid);
						}



						cJSON* pSerialNo = cJSON_GetObjectItem(pRoot,"serialno");
						char strSerialNo[128] ={0};
						if(pSerialNo)
							memcpy(strSerialNo,pSerialNo->valuestring,strlen(pSerialNo->valuestring)+1);

						int ret = 0;//this0->m_pSwitchManager->CheckOneSwitch(strsessionid);

						//���Ļظ�
						Stream ptmpRequest;

						ptmpRequest.m_clientSocket = accept_fd;
						//cJSON *pRet_root;
						ptmpRequest.pRet_root = cJSON_CreateObject();
						ptmpRequest.Requst_Json_str(2,"cmd","check_session");

						ptmpRequest.Requst_Json_str(2,"sessionid",strsessionid);

						ptmpRequest.Requst_Json_str(1,"task_id",strsessionid);
						char txt[32] ={0};
						sprintf(txt,"%d",ret);
						ptmpRequest.Requst_Json_str(2,"ret_code",txt);

						ptmpRequest.Requst_Json_str(2,"serialno",strSerialNo);
						ptmpRequest.Send_Jsoon_str();
					}


				}
			}

	}

		close(accept_fd);

}

int Switch_Control::GetHostIp(const char *name,char *addr)
{
	int ret =-1;
	int inet_sock;
	struct ifreq ifr;
	inet_sock = socket(AF_INET, SOCK_DGRAM, 0);
	strcpy(ifr.ifr_name, name);
	if (ioctl(inet_sock, SIOCGIFADDR, &ifr) < 0)
	{
		perror("ioctl");
		return ret;
	}
	sprintf(addr,"%s", inet_ntoa(((struct sockaddr_in*)&(ifr.ifr_addr))->sin_addr));
	return 0;
}

int Switch_Control::FindSeverInfo(const char *strSeverInfo)
{
	  char strSw_ServerList[1024] = {0};
	  strcpy(strSw_ServerList,strSeverInfo);
	  int iParselen = 0;
	  int Totallen = strlen(strSw_ServerList);
	
	  int iServerID = 1;
	  while(iParselen < Totallen)
	  {
		   int iport = 0;
		   char pfindip[256] = {0};
		   char pfindport[32] = {0};
		   char* pCur = strSw_ServerList+iParselen;
		   printf("--%s \n",pCur);
		   char* pTmpfind = strstr(pCur,":");
		   if(NULL != pTmpfind)
		   {
	
			  int ilen = pTmpfind - pCur;
			  iParselen += (ilen);
			  printf("----parse len =%d \n",iParselen);
			  if(ilen < 5)
			  {
				  //��������������
					printf("continue 1---\n");
				  continue;
			  }
			  strncpy(pfindip,pCur,ilen);
			  pfindip[ilen] = '\0';
			  //����port
			  if(strlen(pTmpfind)<=1)
			  {
				  //û�ҵ��˿�
				  printf("endd 1---\n");
				  break;
			  }
			  char *pTmpdot = strstr(pTmpfind,",");
			  if(pTmpdot == NULL)
			  {
				  //���һ��
				  iParselen += strlen(pTmpfind);
				  iport = 0;
				  iport = atoi(pTmpfind+1);

				  if(iport <= 0)
			  	{
			  		
					break;
			  	}
				  //��������Ϊ���ҵ���һ����ȷ��server �����б�
				 SwitchSeverInfo *pNewServer = new SwitchSeverInfo;
				 memset(pNewServer,0,sizeof(SwitchSeverInfo));
				 strcpy(pNewServer->strServerIPAddr,pfindip);
				 pNewServer->iListenPort = iport;
				 pNewServer->iServerID = iServerID++;
	
				 printf("--find ip=%s port=%d ----\n",pNewServer->strServerIPAddr,pNewServer->iListenPort);
	
				 ManagerFactory::instance()->m_mapSwitchServerInfo.insert(MapSwitchSeverInfo::value_type(pNewServer->iServerID,pNewServer));
				
				 break;
			  }
			  else
			  {
				  ilen = pTmpdot - pTmpfind;
				  iParselen += (ilen + 1);
				  printf("----parse len =%d \n",iParselen);
				  if(ilen<=1)
				  {
					  //û�ҵ��˿�
					  continue ;
				  }
				  strncpy(pfindport,pTmpfind+1,ilen-1);
				  pfindport[ilen] = '\0';
				  iport =0;
				  iport = atoi(pfindport);

			 	if(iport <= 0)
					continue;
				  //��������Ϊ���ҵ���һ����ȷ��server �����б�
				 SwitchSeverInfo *pNewServer = new SwitchSeverInfo;
				 memset(pNewServer,0,sizeof(SwitchSeverInfo));
				 strcpy(pNewServer->strServerIPAddr,pfindip);
				 pNewServer->iListenPort = iport;
				 pNewServer->iServerID = iServerID++;
	
				 printf("--find ip=%s port=%d ----\n",pNewServer->strServerIPAddr,pNewServer->iListenPort);
	
				 ManagerFactory::instance()->m_mapSwitchServerInfo.insert(MapSwitchSeverInfo::value_type(pNewServer->iServerID,pNewServer));
		  	 }
		   }
		   else
	   		{	
				break;
		   	}
	  }
	return 0;
}

int Switch_Control::SendResetMessage(const char* phostIp,int iPort)
{
	struct sockaddr_in s_addr;
	int sockid;
	socklen_t addr_len;

    sockid=socket(AF_INET,SOCK_STREAM,0);
	    
    s_addr.sin_family = AF_INET ;

   	s_addr.sin_addr.s_addr = inet_addr(phostIp) ;
    s_addr.sin_port=htons((unsigned short)iPort);

   
/*	 if (-1 == fcntl(sockid, F_SETFL, O_NONBLOCK))
	 {
		 printf("fcntl socket error!\n");
		 fflush(stdout);
		 return -1;
	 }	 
*/
	 
	 unsigned long ul = 1;
	 ioctl(sockid, FIONBIO, &ul); //����Ϊ������ģʽ
	 struct timeval timeout={3,1000*500}; //1��
	 int len = sizeof(timeout);
	 setsockopt(sockid,SOL_SOCKET,SO_SNDTIMEO, &timeout,len);
	 setsockopt(sockid,SOL_SOCKET,SO_RCVTIMEO,&timeout,len);

	fd_set set;
 	printf("---begin connect \n");
    //�ظ�����3�Σ���ֹ�жϵ�ԭ���µ��쳣
    for(int i=0;i<3;i++)
    {
    	bool ret = false;
		int iret = -1;
		iret = connect(sockid,(struct sockaddr *)&s_addr,(int)(sizeof(s_addr)));
  		if(iret == -1)
   		{
   			struct timeval tm;
     		tm.tv_sec  = 0;
     		tm.tv_usec = 1000*500;
     		FD_ZERO(&set);
     		FD_SET(sockid, &set);
			int error=-1;
     		if( select(sockid+1, NULL, &set, NULL, &tm) > 0)
     		{
       			getsockopt(sockid, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len);
       			if(error == 0) 
				{
					ret = true;
					printf("---error no \n");
					//break;
   				}
 				else ret = false;
    		} else ret = false;
  		}
 		else if(iret==0)
		{	
			
			break;
		}
		if(i==2) 
    	 {
    	 	  //cout<<"connect Error "<<p_host<<":"<<p_Port<<endl;
    	 	  //perror("::connect");
    	 	  ::close(sockid);
    	 	  return -1;
    	 }
		 printf("---sleeep\n");
    	 usleep(1000);
    	 //cout<<"connect again: "<<p_host<<":"<<p_Port<<endl;
    }
    int     optval = 1;

	 ul = 0;
	 ioctl(sockid, FIONBIO, &ul); //����Ϊ����ģʽ
	printf("---connect success \n");	

	char alivetick[1024]={0};
	int iData=0;
	char strdata[32]={0};
	sprintf(strdata,"%d",iData);
  	//��������
	Stream ptmpRequest;

	ptmpRequest.m_clientSocket = sockid;
	//cJSON *pRet_root;
	ptmpRequest.pRet_root = cJSON_CreateObject();
	ptmpRequest.Requst_Json_str(2,"cmd","reset_device");
	ptmpRequest.Requst_Json_str(2,"returnCode","0");
	ptmpRequest.Requst_Json_str(2,"serialno","11111111");
	ptmpRequest.Send_Jsoon_str();

	memset(alivetick,0,sizeof(alivetick));
	int length = 0;
	int i_rc = 0, i_count = 0;
	int iRecvLen = 0;
	do
	{
		i_rc = recv(sockid, alivetick + i_count, 2000 - i_count, 0);
		if (i_rc <= 0)break;//�쳣�ر�
		i_count += i_rc;
	} while (strstr(alivetick, "XXEE") == NULL);
	iRecvLen = i_count;
	if (iRecvLen <= 0){
		::close(sockid);
		return -1;
	}
	printf("recv:%s \n",alivetick);

	cJSON *pcmd = NULL;

			//������������
	replace(alivetick, "XXEE", "");
	cJSON* pRoot = cJSON_Parse(alivetick);

	if (pRoot)
	{
		pcmd = cJSON_GetObjectItem(pRoot, "cmd");
		if (pcmd)
		{
			//�ж���������
			if(strcmp(pcmd->valuestring, "reset_device") == 0)
			{
				::close(sockid);
				return 0;
			}
		}

	}
	::close(sockid);
	return -1;
}


int Switch_Control::ResetAllSwitch()
{
			//����������
		pthread_mutex_lock(&ManagerFactory::instance()->m_mutexServerList);
		MapSwitchSeverInfo::iterator iterLook = ManagerFactory::instance()->m_mapSwitchServerInfo.begin();
		while(iterLook != ManagerFactory::instance()->m_mapSwitchServerInfo.end())
		{
			//
			SwitchSeverInfo *pTmpSeverInfo = iterLook->second;
			if(pTmpSeverInfo && pTmpSeverInfo->iRunStatus == 0)  //���ߵ�������
			{
				int iRet = SendResetMessage(pTmpSeverInfo->strServerIPAddr,pTmpSeverInfo->iListenPort);
				if(iRet == 0)
				{
					LOG_INFO_FORMAT("INFO  - [SWM]:������[%s] �������ɹ� \n",pTmpSeverInfo->strServerIPAddr);
					pTmpSeverInfo->iStreamStatus = 0;
				}
				else
				{
					
					LOG_INFO_FORMAT("INFO  - [SWM]:������[%s] ������ʧ�� \n",pTmpSeverInfo->strServerIPAddr);
					pTmpSeverInfo->iStreamStatus = 1;  //��״̬�쳣
				}
			}
			else
				printf("---������ ������ %s \n",pTmpSeverInfo->strServerIPAddr);
			
			++iterLook;
		}

		pthread_mutex_unlock(&ManagerFactory::instance()->m_mutexServerList);
	return 0;
}



bool Switch_Control::init(const char * ConfigFile)
{
	char ip[64];
	PropConfig cfg;
	if(cfg.init(ConfigFile)==false)  return false;

	SW_version = "V1.0.00.1";
	SW_netname=cfg.getValue("SWM.NETNAME");
	GetHostIp(SW_netname.c_str(),ip);
	SW_ip=ip;
	SW_listentport=cfg.getValue("SWM.LISTENTPORT");
    string SW_SeverList;
    SW_SeverList=cfg.getValue("SWM_SERVER.LIST");

	char SW_Net_ip[256]={0};
	strcpy(SW_Net_ip,SW_ip.c_str());
	
	ManagerFactory::instance()->Init(SW_Net_ip);

    //���������е�erver�б� ��ʽ����ip:port,ip:port"
  	FindSeverInfo(SW_SeverList.c_str());

	swm_process_thread = 4;
	swm_process_thread = atoi(cfg.getValue("SWM.PROCESS_THREADS").c_str());
	log_file_path=cfg.getValue("SWM.LOG_FILE_PATH");
	log_file=cfg.getValue("SWM.LOG_FILE");
	log_level=atoi(cfg.getValue("SWM.LOG_SIZE").c_str());
	log_size=atoi(cfg.getValue("SWM.LOG_NUM").c_str());
	log_num=atoi(cfg.getValue("SWM.LOG_LEVEL").c_str());
	iAliveTickInterval = 0;
	iAliveTickInterval = atoi(cfg.getValue("SWM.ALIVE_INTERVAL").c_str());
	cout<<"ALIVE_INTERVAL ="<<iAliveTickInterval<<endl;
	iAliveTickInterval = iAliveTickInterval*1000;

	if(iAliveTickInterval < MinAliveTickInterval || iAliveTickInterval > MaxAliveTickInterval)
		iAliveTickInterval = MinAliveTickInterval*3;
	
	cout<<endl<<"load config is  ----------------------"<<endl;
	cout<<"SWM_Version ="<<SW_version<<endl;
	cout<<"SWM_ip ="<<SW_ip<<endl;
	cout<<"SWM_listentport ="<<SW_listentport<<endl;
	cout<<"SWM.PROCESS_THREADS ="<<swm_process_thread<<endl;
	cout<<"log_file_path ="<<log_file_path<<endl;
	cout<<"log_file ="<<log_file<<endl;
	cout<<"log_level ="<<log_level<<endl;
	cout<<"ALIVE_INTERVAL ="<<iAliveTickInterval<<endl;
	cout<<"load config over      ----------------------"<<endl;

	LogFactory::instance()->init(log_file_path.c_str(),
   					log_file.c_str(),
   					log_level,
   					LOGOUT_FILE,/*|LOGOUT_SCREEN,*/
   					log_size,//per 256 M
   					log_num);//10 files
	LogFactory::instance()->start();

	
	//init setting 
	LOG_INFO("INFO  - [SWM]:��ʼ����\n");
	LOG_INFO("INFO  - [SWM]:��ȡSWM�����ļ�\n");
	LOG_INFO_FORMAT("INFO  - [SWM]:����汾[SWM.version] = %s\n",SW_version.c_str());
	LOG_INFO_FORMAT("INFO  - [SWM]:����˿�[SWM.tcpserver.port] = %s\n",SW_listentport.c_str());

	m_iMsiPort = atoi(SW_listentport.c_str());


	
	Swm_Worker_Sever sever_Worker;
	sever_Worker.setName("swm_sever");
	if(!sever_Worker.open(m_iMsiPort,swm_process_thread))
		return -1;

	LOG_INFO("INFO  - [SWM]: �����ɹ� ........\n");

		
		//��������б��߳�
	pthread_t tcp_recv_thread4;
	pthread_create(&tcp_recv_thread4, NULL, ts_recv_Control_thread, this);
	pthread_detach(tcp_recv_thread4);

		//Sleep
	sleep(1);
	printf("---send reset msg\n");
	//������������
	ResetAllSwitch();

	sever_Worker.wait();
	LogFactory::instance()->wait();

		printf("---send reset msg1\n");
/*
	m_pSwitchManager = new Switch_Manager;
	char SW_Net_ip[256]={0};
	strcpy(SW_Net_ip,SW_ip.c_str());
	m_pSwitchManager->Init(SW_Net_ip);
*/

	



	 return true;
}


