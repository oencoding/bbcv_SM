
#include "Switch_Manager.h"
#include <string>
#include "Log.h"


Switch_Manager::Switch_Manager()
{
	m_iCurrentPort = iBeginPort;
	m_iCheckStreamTime = 20*1000;
	pthread_mutex_init(&m_lockerttTaskInfo, NULL);
	
}
Switch_Manager::~Switch_Manager()
{

	pthread_mutex_destroy(&m_lockerttTaskInfo);
}

bool Switch_Manager::Init(char *SW_ip) //��Ҫ������ �˿�
{

	memset(m_SW_ip,0,sizeof(m_SW_ip));

	strcpy(m_SW_ip,SW_ip);
	printf("----used SW_IP is %s \n",m_SW_ip);
	fflush(stdout);
	//redis config
	memset(m_redis_ip,0,sizeof(m_redis_ip));
	m_iredis_port = 0;
	memset(m_redis_password,0,sizeof(m_redis_password));
	m_iredis_pool_num = 0;

	//config copy
	strcpy(m_redis_ip,"127.0.0.1");	
	m_iredis_port = 6379;
	strcpy(m_redis_password,"123");	
	m_iredis_pool_num = 2;
	
	
	//LOG_INFO_FORMAT("----used SW_IP is %s \n",m_SW_ip);
	//init redis
	bool ret = m_redis_clients.init(m_redis_ip, 
								m_iredis_port, 
								m_redis_password,
								m_iredis_pool_num);

	//����һ��check��״̬��Ϣ�߳�
	pthread_t check_stream_thread;
	pthread_create(&check_stream_thread, NULL, check_stream_thread_func, this);
	pthread_detach(check_stream_thread);
	
	return true;
}

bool Switch_Manager::Init(const char *SW_ip,const char* redis_ip,int iredis_port,const char* redis_passwd,int iredis_poolnum) //��Ҫ������ �˿�
{

	memset(m_SW_ip,0,sizeof(m_SW_ip));

	strcpy(m_SW_ip,SW_ip);
	printf("----used SW_IP is %s \n",m_SW_ip);
	fflush(stdout);
	//redis config
	memset(m_redis_ip,0,sizeof(m_redis_ip));
	m_iredis_port = 0;
	memset(m_redis_password,0,sizeof(m_redis_password));
	m_iredis_pool_num = 0;
		

	//config copy
	strcpy(m_redis_ip,redis_ip);	
	m_iredis_port = iredis_port;
	strcpy(m_redis_password,redis_passwd);	
	m_iredis_pool_num = iredis_poolnum;
	
	
	//LOG_INFO_FORMAT("----used SW_IP is %s \n",m_SW_ip);
	//init redis
	bool ret = m_redis_clients.init(m_redis_ip, 
								m_iredis_port, 
								m_redis_password,
								m_iredis_pool_num);


		//����һ��check��״̬��Ϣ�߳�
	pthread_t check_stream_thread;
	pthread_create(&check_stream_thread, NULL, check_stream_thread_func, this);
	pthread_detach(check_stream_thread);
	
	return true;
}

int Switch_Manager::ClearAllRedisData()
{

		int ret= -1;
		int istreamstatus = 0;
		string key = "SWM:switch_info:*";
		
		char buf_key[1024];
		std::vector<std::string> TmpVectorValue;
		
		//SWM:switch_info:sessionid  serverid input_url output_url successstatus
		ret = m_redis_clients.getkeysvalue(key,TmpVectorValue);
		if(ret == 0)
		{
			LOG_TRACE_FORMAT("TRACE :get switchinfo mdb success key = [%s] valuenum = [%d] \n", key.c_str(),TmpVectorValue.size());
			//return true;	
		}
		else
		{
			LOG_TRACE_FORMAT("TRACE :get switchinfo mdb error:%d, key = [%s]\n", ret, key.c_str());
			//return false;
		}	
		
		for(int i = 0;i < TmpVectorValue.size();i++)
		{
			
			const char * strsessionid = TmpVectorValue[i].c_str() + strlen("SWM:switch_info:");
			//ɾ����Ӧ��key
			int ret= -1;
			string key;
			string value;
			char buf_key[1024];
			
			//SWM:switch_info:sessionid  serverid input_url output_url successstatus
			snprintf(buf_key,sizeof(buf_key), "SWM:switch_info:%s",strsessionid);

			key = buf_key;

			ret = ManagerFactory::instance()->m_redis_clients.delvalue(key);
			if(ret == 0)
			{
				LOG_TRACE_FORMAT("TRACE :del serverlist mdb success key = [%s]\n", key.c_str());
				//return true;	
			}
			else
			{
				LOG_TRACE_FORMAT("TRACE :del serverlist mdb error:%d, key = [%s]\n", ret, key.c_str());
				//return false;
			}
			
		}

}


//����ӳ����� 

int Switch_Manager::CheckOneSwitch( char* strSessionID)
{
	//��װcheck����
	//���Ļظ�
	printf("---in checkone switch\n");
	Stream ptmpRequest;
	//cJSON *pRet_root;
	ptmpRequest.pRet_root = cJSON_CreateObject();
	ptmpRequest.Requst_Json_str(2,"cmd","check_session");
	ptmpRequest.Requst_Json_str(2,"sessionid",strSessionID);
	ptmpRequest.Requst_Json_str(2,"serialno","b974e9d8f424");

	char cJsonBuff[1024 * 2]={0};
	char * m_tmp;
	m_tmp = cJSON_Print(ptmpRequest.pRet_root);
	memset(cJsonBuff, 0, sizeof(cJsonBuff));
	sprintf(cJsonBuff, "%sXXEE", m_tmp);
	free(m_tmp);
	printf("send json: %s \n",cJsonBuff);
	fflush(stdout);
	cJSON_Delete(ptmpRequest.pRet_root);
	//copy to output 
	//
	char Replay_buf[1024*2]={0};
	CheckOneSwitch(cJsonBuff,Replay_buf,strSessionID);
	//��������
	cJSON *pcmd = NULL;
	//������������
	replace(Replay_buf, "XXEE", "");
	cJSON* pRoot = cJSON_Parse(Replay_buf);

	int iRetCode = -1;
	if (pRoot)
	{
		pcmd = cJSON_GetObjectItem(pRoot, "cmd");
		if (pcmd)
		{
	
			//�ж���������
			if (strcmp(pcmd->valuestring, "check_session") == 0)
			{
				printf("--check_session return \n");
				//ͨ��
				cJSON* pSeesid = cJSON_GetObjectItem(pRoot, "sessionid");
				char strSeesionID[128] ={0};
				
				//int iSeessionID = 0;
				if(pSeesid){
					//iSeessionID = atoi(pSeesid->valuestring);
					memcpy(strSeesionID,pSeesid->valuestring,strlen(pSeesid->valuestring)+1);
				}
				cJSON* pRetCOde = cJSON_GetObjectItem(pRoot,"ret_code");
				
				if(pRetCOde) iRetCode = atoi(pRetCOde->valuestring);
				cJSON* pSerialNo = cJSON_GetObjectItem(pRoot,"serialno");
				char strSerialNo[128] ={0};
				if(pSerialNo)
					memcpy(strSerialNo,pSerialNo->valuestring,strlen(pSerialNo->valuestring)+1);
				if(iRetCode >= 0)
				{
					//����

				}
				//��ѯ��ǰ��״̬ ��������ڲ�ͬ���޸ġ�
				
				
						int ret= -1;
						int istreamstatus = 0;
						string key;
						string value;
						char buf_key[1024];
						
						//SWM:switch_info:sessionid  serverid input_url output_url successstatus
						snprintf(buf_key,sizeof(buf_key), "SWM:switch_info:%s",strSessionID);
			
						key = buf_key;
						ret = m_redis_clients.getvalue(key,value);
						if(ret == 0)
						{
							LOG_TRACE_FORMAT("TRACE :get switchinfo mdb success key = [%s] value = [%s] \n", key.c_str(),value.c_str());
							//return true;	
						}
						else
						{
							LOG_TRACE_FORMAT("TRACE :get switchinfo mdb error:%d, key = [%s]\n", ret, key.c_str());
							//return false;
						}

						int iswitchserver_id =0;
						char input_url[512] = {0};
						char output_url[512] = {0};
						int iswitchstatus = 0;
						//printf("--get ");
						if(4 != sscanf(value.c_str(),"%d|%[^|]|%[^|]|%d",
							&iswitchserver_id,input_url,output_url,&iswitchstatus))
						{
							LOG_ERROR_FORMAT("ERROR-[SWM]:invald resource id [%s] ,info :[%s]\n", value.c_str(),key.c_str());
							return false;
						}
						if(iswitchstatus != iRetCode)
						{
							//�޸����ݿ�
							istreamstatus = iRetCode;
							int ret= -1;
							string value;
							char buf_value[4096];
							snprintf(buf_value,sizeof(buf_value), "%d|%s|%s|%d",
								iswitchserver_id,input_url,output_url,istreamstatus);
							
							value = buf_value;					
							ret = ManagerFactory::instance()->m_redis_clients.setvalue(key,value);
							if(ret == 0)
							{
								LOG_TRACE_FORMAT("TRACE :add serverlist mdb success key = [%s],value = [%s]\n", key.c_str(), value.c_str());
								//return true;	
							}
							else
							{
								LOG_TRACE_FORMAT("TRACE :add serverlist mdb error:%d, key = [%s],value = [%s]\n", ret, key.c_str(), value.c_str());
								//return false;
							}


						}
						
			}
		}
	}

}


int Switch_Manager::GetOneSwitchServer(SwitchSeverInfo** pSeverSwitch)
{
	//���ҿ��õ�sever  ��ʹ������С��
	//����������
	printf("---get one Switch server fun \n");
	LOG_DEBUG("DEBUG - [SWM]: - get one Switch server fun \n");
	int mindbUseRate = 65565; 
	pthread_mutex_lock(&m_mutexServerList);
	MapSwitchSeverInfo::iterator iterLook = m_mapSwitchServerInfo.begin();
	while(iterLook != m_mapSwitchServerInfo.end())
	{
		//
		SwitchSeverInfo *pTmpSeverInfo = iterLook->second;
		if(pTmpSeverInfo && pTmpSeverInfo->iRunStatus == 0 && pTmpSeverInfo->iStreamStatus == 0)  //���߿��õ�������
		{
			//����״̬��������·��������
			if(mindbUseRate >= pTmpSeverInfo->iCurrentSwitchNumber)
			{	
				mindbUseRate = pTmpSeverInfo->iCurrentSwitchNumber;
			
				*pSeverSwitch = pTmpSeverInfo;
				printf("get one sever \n");
				LOG_INFO_FORMAT("INFO  - [SWM]: get one sever ip %s\n",pTmpSeverInfo->strServerIPAddr);
			}
		}
		
		
		++iterLook;
	}
	
	pthread_mutex_unlock(&ManagerFactory::instance()->m_mutexServerList);
	

	return 0;
}

void *Switch_Manager::check_stream_thread_func(void *param)
{
	Switch_Manager* this0 = (Switch_Manager*)param;

	struct timeval tv1,tv2;
	long long time1,time2;
	long long nsendbytes = 0;
	long long totaltime = 0;
	long now_bit_rate = 0;
	gettimeofday(&tv1, NULL);
	time1 = tv1.tv_sec*1000 + tv1.tv_usec / 1000;
	bool iFirstTime = true;

	printf("---check time=%d \n",this0->m_iCheckStreamTime);
	//long bit_rate = this0->m_iSendRate;
	while(1)
	{
		if(!iFirstTime)
		{
			gettimeofday(&tv2, NULL);
			time2 = tv2.tv_sec*1000 + tv2.tv_usec / 1000;
			if(time2 - time1 < this0->m_iCheckStreamTime)
			{
				usleep(500*1000);	
				//printf("now_bit_rate is too large %d %d\n",now_bit_rate,this0->m_iSendRate);
				continue;
			}
			printf("------%ld \n",time2);
			time1 = time2;
		}
		//�����״̬
		//����map
	//	pthread_mutex_lock(&this0->m_lockerttTaskInfo);
		MapSeverInfo::iterator itlook = this0->m_mapSwitchTaskInfo.begin();
		while(itlook != this0->m_mapSwitchTaskInfo.end())
		{
			//�����״̬
			char strSessionId[128]={0};
			strcpy(strSessionId,itlook->first.c_str());

//			pthread_mutex_unlock(&this0->m_lockerttTaskInfo);
			this0->CheckOneSwitch(strSessionId);
			
			itlook++;
		}
		


		iFirstTime = false;
	}
}


int Switch_Manager::FindOneSwitchServer(char* strSessionID,SwitchSeverInfo** ppSeverSwitch)
{
	if(ppSeverSwitch == NULL)
		return -2;
	printf("---find one SwitchServer \n");
	LOG_DEBUG("DEBUG SWT - find one SwitchServer \n");
	int ifindServerID = -1;
	pthread_mutex_lock(&m_lockerttTaskInfo);
	MapSeverInfo::iterator itfind = m_mapSwitchTaskInfo.find(strSessionID);
	if(itfind != m_mapSwitchTaskInfo.end())
	{
		ifindServerID = itfind->second;
	}
	pthread_mutex_unlock(&m_lockerttTaskInfo);
	if(ifindServerID != -1)
	{
		//�ҵ�severinfo
		SwitchSeverInfo *pTmpSeverInfo = NULL;
		pthread_mutex_lock(&m_mutexServerList);
		
		MapSwitchSeverInfo::iterator itlook = m_mapSwitchServerInfo.find(ifindServerID);
		if(itlook != m_mapSwitchServerInfo.end())
		{
			pTmpSeverInfo = itlook->second;
			if(pTmpSeverInfo ==NULL || pTmpSeverInfo->iRunStatus!= 0 || pTmpSeverInfo->iStreamStatus != 0)
			{
				if(pTmpSeverInfo != NULL)
					printf("----find sever but sever status ip %s error \n",pTmpSeverInfo->strServerIPAddr);
				LOG_INFO("INFO  - [SWM]: find sever but sever status error \n");
				pthread_mutex_unlock(&m_mutexServerList);
				return -1;
			}

			pthread_mutex_unlock(&m_mutexServerList);
		}
		else
		{
			pthread_mutex_unlock(&m_mutexServerList);
			printf("---not find severID =%d map size=%d \n",ifindServerID,m_mapSwitchServerInfo.size());
			LOG_INFO_FORMAT("INFO  - [SWM]: not find severID =%d  map size=%d \n",ifindServerID,m_mapSwitchServerInfo.size());
			return -1;
		}
		
		*ppSeverSwitch = pTmpSeverInfo;
		
	}
	else
	{
		//ɾ�����߼��״̬��û����Ҳ��Ҫ�ظ���ֱ��ѡ���һ�����õ�sever
		printf("---not find sessionid %s map size=%d \n",strSessionID,m_mapSwitchTaskInfo.size());
		LOG_INFO_FORMAT("INFO  - [SWM]: not find sessionid %s map size=%d \n",strSessionID,m_mapSwitchTaskInfo.size());
		return -1;
	}


	return 0;
}

int Switch_Manager::FindSwitchServerFromIP(char* strIP,SwitchSeverInfo** ppSeverSwitch)
{
	if(ppSeverSwitch == NULL)
		return -2;
	printf("---find one SwitchServer from ip \n");
	LOG_DEBUG("DEBUG - [SWM]: - find one SwitchServer from ip \n");
//	if(ifindServerID != -1)
	{
		//�ҵ�severinfo
		SwitchSeverInfo *pTmpSeverInfo = NULL;
		pthread_mutex_lock(&m_mutexServerList);
		
		MapSwitchSeverInfo::iterator itlook = m_mapSwitchServerInfo.begin();
		while(itlook != m_mapSwitchServerInfo.end())
		{
			pTmpSeverInfo = itlook->second;
			if(strcmp(pTmpSeverInfo->strServerIPAddr,strIP)==0)
			{
				
				if(pTmpSeverInfo ==NULL || pTmpSeverInfo->iRunStatus!= 0 || pTmpSeverInfo->iStreamStatus != 0)
				{
					if(pTmpSeverInfo != NULL)
						printf("----find from ip sever but sever status ip %s error \n",pTmpSeverInfo->strServerIPAddr);
					LOG_INFO("INFO  - [SWM]: find from ip sever but sever status error \n");
					pthread_mutex_unlock(&m_mutexServerList);
					return -1;
				}
				else
				{
					printf("---find sever Ip =%s map size=%d \n",strIP,m_mapSwitchServerInfo.size());	
					break;
				}
			}
			else
			{
				pTmpSeverInfo = NULL;
			}

			++itlook;
			
		}
		pthread_mutex_unlock(&m_mutexServerList);
		if(pTmpSeverInfo ==NULL)
		{
			printf("---not find sever Ip =%s map size=%d \n",strIP,m_mapSwitchServerInfo.size());
			LOG_INFO_FORMAT("INFO  - [SWM]: not find severID =%s  map size=%d \n",strIP,m_mapSwitchServerInfo.size());
			return -1;
		}
		
		*ppSeverSwitch = pTmpSeverInfo;
		
	}


	return 0;
}


int Switch_Manager::DeleteAllWsitch(char* strUrl,char* outPutUrl,char* strSessionID)
{
	LOG_DEBUG("DEBUG - [SWM]: - DeleteAllWsitch fun \n");
	printf("---DeleteAllWsitch fun \n");
	bool bIsReplyflag = false;
		//���������� ʱ�������������
	pthread_mutex_lock(&m_mutexServerList);
	MapSwitchSeverInfo::iterator iterLook = m_mapSwitchServerInfo.begin();
	while(iterLook != m_mapSwitchServerInfo.end())
	{
		//
		SwitchSeverInfo *pFindSeverInfo = iterLook->second;
		if(pFindSeverInfo && pFindSeverInfo->iRunStatus == 0)  //���ߵ�������
		{
				//����server 
			//����ת��
			Stream ptmpRequest;
			int iret = ptmpRequest.ConnectServer(pFindSeverInfo->strServerIPAddr,pFindSeverInfo->iListenPort);
			if(iret != 0)
			{
				//����ʧ��		
				continue;
			}
			iret = ptmpRequest.Send_str(strUrl);
			if(iret != 0)
			{
				continue;
			}
			char Rcv_buf[1024]={0};
			int iLen = sizeof(Rcv_buf);
			iret = ptmpRequest.Recv_str(Rcv_buf,&iLen);
			if(iret !=0)
				continue;

			memcpy(outPutUrl,Rcv_buf,iLen);
			bIsReplyflag = true;
			outPutUrl[iLen] ='\0';

				cJSON *pcmd = NULL;

					//������������
			replace(Rcv_buf, "XXEE", "");
			cJSON* pRoot = cJSON_Parse(Rcv_buf);

			int iRet = -1;
			if (pRoot)
			{
				pcmd = cJSON_GetObjectItem(pRoot, "cmd");
				if (pcmd)
				{
					//�ж���������
					if(strcmp(pcmd->valuestring, "reset_device") == 0)
					{
						iRet = 0;
					}
				}

			}
			if(iRet == 0)
			{
				LOG_INFO_FORMAT("INFO  - [SWM]:������[%s] �������ɹ� \n",pFindSeverInfo->strServerIPAddr);
				pFindSeverInfo->iStreamStatus = 0;
				pFindSeverInfo->iCurrentSwitchNumber = 0;


			}
			else
			{
				
				LOG_INFO_FORMAT("INFO  - [SWM]:������[%s] ������ʧ�� \n",pFindSeverInfo->strServerIPAddr);
				pFindSeverInfo->iStreamStatus = 1;  //��״̬�쳣
			}
		}
		else
			printf("---������ ������ %s \n",pFindSeverInfo->strServerIPAddr);
		
		++iterLook;
	}

	pthread_mutex_unlock(&ManagerFactory::instance()->m_mutexServerList);

	if(!bIsReplyflag)
	{
		ReplyException(strUrl,outPutUrl,Error_ReSet_Device);
		LOG_INFO("INFO  - [SWM]: Delete ALL stream exception \n");
	}
	printf("----clear map and redis\n");
	//�����map
	pthread_mutex_lock(&m_lockerttTaskInfo);
	MapSeverInfo::iterator itlook = m_mapSwitchTaskInfo.begin();
	while(itlook != m_mapSwitchTaskInfo.end())
	{
		//��������һ���ỰID ��ɾ�� yyd
		//����redis
		int ret= -1;
		int istreamstatus = 0;
		string key;
		string value;
		char buf_key[1024];
		
		//SWM:switch_info:sessionid  serverid input_url output_url successstatus
		snprintf(buf_key,sizeof(buf_key), "SWM:switch_info:%s",itlook->first.c_str());

		printf("---%s \n",buf_key);
		key = buf_key;

		ret = ManagerFactory::instance()->m_redis_clients.delvalue(key);
		if(ret == 0)
		{
			LOG_TRACE_FORMAT("TRACE :del serverlist mdb success key = [%s]\n", key.c_str());
			//return true;	
		}
		else
		{
			LOG_TRACE_FORMAT("TRACE :del serverlist mdb error:%d, key = [%s]\n", ret, key.c_str());
			//return false;
		}
		//ɾ��map
		m_mapSwitchTaskInfo.erase(itlook++);
	}
	//���sever��map����������������ɾ����
	printf("----erase to map size=%d \n",m_mapSwitchTaskInfo.size());
	pthread_mutex_unlock(&m_lockerttTaskInfo);
	LOG_INFO_FORMAT("----erase to map size=%d \n",m_mapSwitchTaskInfo.size());

	return 0;

}

int Switch_Manager::GetOneSwitchCheckStatus(char* strUrl,char* outPutUrl,char* strSessionID)
{
		LOG_DEBUG("DEBUG - [SWM]: - getOneSwitchstatus\n");
		printf("--getOneSwitchstatus session %s \n",strSessionID);

		//��ѯ��ǰ��״̬ ��������ڲ�ͬ���޸ġ�


		int ret= -1;
		int istreamstatus = 0;
		string key;
		string value;
		char buf_key[1024];
		
		//SWM:switch_info:sessionid  serverid input_url output_url successstatus
		snprintf(buf_key,sizeof(buf_key), "SWM:switch_info:%s",strSessionID);

		key = buf_key;
		ret = m_redis_clients.getvalue(key,value);
		if(ret == 0)
		{
			LOG_TRACE_FORMAT("TRACE :get switchinfo mdb success key = [%s] value = [%s] \n", key.c_str(),value.c_str());
			//return true;	
		}
		else
		{
			LOG_TRACE_FORMAT("TRACE :get switchinfo mdb error:%d, key = [%s]\n", ret, key.c_str());
			//return false;
		}

		int iswitchserver_id =0;
		char input_url[512] = {0};
		char output_url[512] = {0};
		int iswitchstatus = -1;
		//printf("--get ");
		if(4 != sscanf(value.c_str(),"%d|%[^|]|%[^|]|%d",
			&iswitchserver_id,input_url,output_url,&iswitchstatus))
		{
			LOG_ERROR_FORMAT("ERROR-[SWM]:invald resource id [%s] ,info :[%s]\n", value.c_str(),key.c_str());
			//return false;
		}	

		{
			printf("---set getoneswitchcheckstatu value \n");
			ReplyException(strUrl,outPutUrl,iswitchstatus);
		}	
	
		return 0;
}


int Switch_Manager::CheckOneSwitch(char* strUrl,char* outPutUrl,char* strSessionID)
{
	LOG_DEBUG("DEBUG - [SWM]: - CheckOneSwitch\n");
	printf("--CheckOneSwitch session %s \n",strSessionID);
	int ret = -1;
//���һỰ
//	printf("---find sessionid %s map size=%d \n",strSessionID,m_mapSwitchTaskInfo.size());

	SwitchSeverInfo** ppSeverSwitch = (SwitchSeverInfo**)malloc(sizeof(SwitchSeverInfo*));
	memset(ppSeverSwitch,0,sizeof(ppSeverSwitch));

	int iRet = FindOneSwitchServer(strSessionID,ppSeverSwitch);
	//�ҵ���Ӧ������
	
	SwitchSeverInfo *pFindSeverInfo = *ppSeverSwitch;
	if(pFindSeverInfo ==NULL)
	{
		printf("---no find sever info or no sessiond \n");
		ReplyException(strUrl,outPutUrl,Error_NO_Stream);
		return -2;
	}
	printf("----find server id =%d ip=%s port=%d \n",pFindSeverInfo->iServerID,
			pFindSeverInfo->strServerIPAddr,pFindSeverInfo->iListenPort);
	//����server 
	//����ת��
	Stream ptmpRequest;
	int iret = ptmpRequest.ConnectServer(pFindSeverInfo->strServerIPAddr,pFindSeverInfo->iListenPort);
	if(iret != 0)
	{
		//����ʧ��
		printf("--- connect error  \n");
		ReplyException(strUrl,outPutUrl,Error_Connect_Sever);
		return -1;
	}
	iret = ptmpRequest.Send_str(strUrl);
	if(iret != 0)
	{
		printf("--- send error  \n");
		ReplyException(strUrl,outPutUrl,Error_Connect_Sever);
		return -1;
	}
	char Rcv_buf[1024]={0};
	int iLen = sizeof(Rcv_buf);
	iret = ptmpRequest.Recv_str(Rcv_buf,&iLen);
	if(iret !=0)
	{
		ReplyException(strUrl,outPutUrl,Error_Recv_Sever);
		return -1;
	}


	memcpy(outPutUrl,Rcv_buf,iLen);
	//LOG_INFO_FORMAT("check one switch status :%s");
	//cJSON *pcmd = NULL;
	//������������
	//replace(Rcv_buf, "XXEE", "");
	//cJSON* pRoot = cJSON_Parse(Rcv_buf);
	

	return 0;
}


int Switch_Manager::ReqOneSwitchForPort(char* strUrl,char* outPutUrl,char* strSessionID)
{
	//���ҿ��õ�sever  ��ʹ������С��
				//����������
	printf("-----ReqOneSwitchForPort \n");
	LOG_DEBUG("DEBUG - [SWM]: - ReqOneSwitchForPort \n");
	printf("-----add switch %s \n",strSessionID);
	SwitchSeverInfo **ppFindSeverInfo = (SwitchSeverInfo **)malloc(sizeof(SwitchSeverInfo *));
	memset(ppFindSeverInfo,0,sizeof(ppFindSeverInfo));

	int iRet = FindOneSwitchServer(strSessionID,ppFindSeverInfo);
	//�ҵ���Ӧ������
	SwitchSeverInfo *pFindSeverInfo = NULL;
	
	if(iRet == 0 && *ppFindSeverInfo!= NULL)
	{
		//�ҵ�����ת������
		pFindSeverInfo = *ppFindSeverInfo;
	}
	else
	{
	
		iRet = GetOneSwitchServer(ppFindSeverInfo);
		if(*ppFindSeverInfo ==NULL)
		{
			printf("---no can used sever -----\n");
			
			ReplyException(strUrl,outPutUrl,Error_NO_Sever);
			
			return -1; //û�п����õ�Switch_Sever;
		}
		pFindSeverInfo = *ppFindSeverInfo;
	}
	 
	//����server 
	//����ת��
	Stream ptmpRequest;
	int iret = ptmpRequest.ConnectServer(pFindSeverInfo->strServerIPAddr,pFindSeverInfo->iListenPort);
	if(iret != 0)
	{
		//����ʧ��
		ReplyException(strUrl,outPutUrl,Error_Connect_Sever);
		return -1;
	}
	iret = ptmpRequest.Send_str(strUrl);
	if(iret != 0)
	{
		ReplyException(strUrl,outPutUrl,Error_Connect_Sever);
		return -1;
	}
	char Rcv_buf[1024]={0};
	int iLen = sizeof(Rcv_buf);
	iret = ptmpRequest.Recv_str(Rcv_buf,&iLen);
	if(iret !=0)
	{
		ReplyException(strUrl,outPutUrl,Error_Recv_Sever);
		return -1;
	}

	memcpy(outPutUrl,Rcv_buf,iLen);

	
	return 0;
}


//���ص��ǹ����ĻỰ
int Switch_Manager::AddOneSwitch(char* strUrl,char* outPutUrl,char* strSessionID,char* inputUrl,int *pServerID)
{
	printf("-----AddOneSwitch\n");
	LOG_DEBUG("DEBUG - [SWM]: - AddOneSwitch\n");
	
	//���ҿ��õ�sever  ��ʹ������С��
				//����������
	printf("-----add switch %s \n",strSessionID);
	SwitchSeverInfo **ppFindSeverInfo = (SwitchSeverInfo **)malloc(sizeof(SwitchSeverInfo *));
	memset(ppFindSeverInfo,0,sizeof(ppFindSeverInfo));

	int iRet = FindOneSwitchServer(strSessionID,ppFindSeverInfo);
	//�ҵ���Ӧ������
	SwitchSeverInfo *pFindSeverInfo = NULL;
	
	if(iRet == 0 && *ppFindSeverInfo!= NULL)
	{
		//�ҵ�����ת������
		pFindSeverInfo = *ppFindSeverInfo;
	}
	else
	{
		//������ת����
		//��Ҫ����inputip��ȷ��ʹ���ĸ�sever
		//������ip �˿�
		int iSrcPort = 0;
		char strSrcIP[128]={0};

		char strSrcPort[32]={0};
		FindDataFromString(inputUrl,strSrcIP,strSrcPort);
		iSrcPort = atoi(strSrcPort);
		//����IP����sever
		iRet = FindSwitchServerFromIP(strSrcIP,ppFindSeverInfo);
		if(*ppFindSeverInfo ==NULL)
		{
			//���û��ָ��ip���ɵ��ȷ���
		
			iRet = GetOneSwitchServer(ppFindSeverInfo);
			if(*ppFindSeverInfo ==NULL)
			{
				printf("---no can used sever -----\n");
				
				ReplyException(strUrl,outPutUrl,Error_NO_Sever);
				
				return -1; //û�п����õ�Switch_Sever;
			}
		}

		pFindSeverInfo = *ppFindSeverInfo;
	}

	//����server 
	//����ת��
	Stream ptmpRequest;
	int iret = ptmpRequest.ConnectServer(pFindSeverInfo->strServerIPAddr,pFindSeverInfo->iListenPort);
	if(iret != 0)
	{
		//����ʧ��
		ReplyException(strUrl,outPutUrl,Error_Connect_Sever);
		return -1;
	}
	iret = ptmpRequest.Send_str(strUrl);
	if(iret != 0)
	{
		ReplyException(strUrl,outPutUrl,Error_Connect_Sever);
		return -1;
	}
	char Rcv_buf[1024]={0};
	int iLen = sizeof(Rcv_buf);
	iret = ptmpRequest.Recv_str(Rcv_buf,&iLen);
	if(iret !=0)
	{
		ReplyException(strUrl,outPutUrl,Error_Recv_Sever);
		return -1;
	}

	memcpy(outPutUrl,Rcv_buf,iLen);
	cJSON *pcmd = NULL;
	//������������
	replace(Rcv_buf, "XXEE", "");
	cJSON* pRoot = cJSON_Parse(Rcv_buf);

	int iRetCode = -1;
	if (pRoot)
	{
		pcmd = cJSON_GetObjectItem(pRoot, "cmd");
		if (pcmd)
		{
	
			//�ж���������
			if (strcmp(pcmd->valuestring, "add_ads_stream") == 0)
			{
				printf("--add_ads_stream return \n");
				//ͨ��
				cJSON* pSeesid = cJSON_GetObjectItem(pRoot, "sessionid");
				char strSeesionID[128] ={0};
				
				//int iSeessionID = 0;
				if(pSeesid){
					//iSeessionID = atoi(pSeesid->valuestring);
					memcpy(strSeesionID,pSeesid->valuestring,strlen(pSeesid->valuestring)+1);
				}
				cJSON* pTaskID = cJSON_GetObjectItem(pRoot, "task_id");
				int iTaskID = -1;
				if(pTaskID) iTaskID = pTaskID->valueint;
				cJSON* pRetCOde = cJSON_GetObjectItem(pRoot,"ret_code");
				
				if(pRetCOde) iRetCode = atoi(pRetCOde->valuestring);
				cJSON* pSerialNo = cJSON_GetObjectItem(pRoot,"serialno");
				char strSerialNo[128] ={0};
				if(pSerialNo)
					memcpy(strSerialNo,pSerialNo->valuestring,strlen(pSerialNo->valuestring)+1);
				if(iRetCode >= 0)
				{
					//��ӳɹ����ӹ���sessionID ��severID ��ֵ��
					
					pthread_mutex_lock(&m_lockerttTaskInfo);
					MapSeverInfo::iterator itlook = m_mapSwitchTaskInfo.find(strSeesionID);
					if(itlook != m_mapSwitchTaskInfo.end())
					{
						//��������һ���ỰID ��ɾ�� yyd
						m_mapSwitchTaskInfo.erase(itlook);
					}

					pFindSeverInfo->iCurrentSwitchNumber++;
					m_mapSwitchTaskInfo.insert(MapSeverInfo::value_type(strSeesionID,pFindSeverInfo->iServerID));
					printf("----insert to map %s ,size=%d \n",strSeesionID,m_mapSwitchTaskInfo.size());
					pthread_mutex_unlock(&m_lockerttTaskInfo);
					LOG_INFO_FORMAT("----insert to map %s ,size=%d \n",strSeesionID,m_mapSwitchTaskInfo.size());

					if(pServerID)
						*pServerID = pFindSeverInfo->iServerID;

				}
				
			}
		}
	}

	return iRetCode;
}

int Switch_Manager::DeleteOneSwitch(char* strUrl,char* outPutUrl,char* strSessionID)
{
	printf("----DeleteOneSwitch fun \n");
	LOG_DEBUG("DEBUG - [SWM]: - DeleteOneSwitch fun \n");
	//���һỰ
//	printf("---find sessionid %s map size=%d \n",strSessionID,m_mapSwitchTaskInfo.size());

	SwitchSeverInfo** ppSeverSwitch = (SwitchSeverInfo**)malloc(sizeof(SwitchSeverInfo*));
	memset(ppSeverSwitch,0,sizeof(ppSeverSwitch));
	int iRet = FindOneSwitchServer(strSessionID,ppSeverSwitch);
	//�ҵ���Ӧ������
	
	SwitchSeverInfo *pFindSeverInfo = *ppSeverSwitch;
	if(pFindSeverInfo == NULL)
	{
		printf("---no find sever info or no sessiond \n");
		ReplyException(strUrl,outPutUrl,0);
		return -2;
	}
	//����server 
	//����ת��
	Stream ptmpRequest;
	int iret = ptmpRequest.ConnectServer(pFindSeverInfo->strServerIPAddr,pFindSeverInfo->iListenPort);
	if(iret != 0)
	{
		//����ʧ��
		ReplyException(strUrl,outPutUrl,Error_Connect_Sever);
		return -1;
	}
	iret = ptmpRequest.Send_str(strUrl);
	if(iret != 0)
	{
		ReplyException(strUrl,outPutUrl,Error_Connect_Sever);
		return -1;
	}
	char Rcv_buf[1024]={0};
	int iLen = sizeof(Rcv_buf);
	iret = ptmpRequest.Recv_str(Rcv_buf,&iLen);
	if(iret !=0)
	{
		ReplyException(strUrl,outPutUrl,Error_Recv_Sever);
		return -1;
	}

	printf("recv Del strem\n");
	fflush(stdout);
	memcpy(outPutUrl,Rcv_buf,iLen);
	cJSON *pcmd = NULL;
	//������������
	replace(Rcv_buf, "XXEE", "");
	cJSON* pRoot = cJSON_Parse(Rcv_buf);
	int iRetCode = -1;
	if (pRoot)
	{
		pcmd = cJSON_GetObjectItem(pRoot, "cmd");
		if (pcmd)
		{
	
			//�ж���������
			 if(strcmp(pcmd->valuestring, "del_ads_stream") == 0)
			{
				 cJSON* pSeesid = cJSON_GetObjectItem(pRoot, "sessionid");
				 char strSeesionID[128] ={0};
				 
				// int iSeessionID = 0;
				 if(pSeesid){
					// iSeessionID = atoi(pSeesid->valuestring);
					 memcpy(strSeesionID,pSeesid->valuestring,strlen(pSeesid->valuestring)+1);
				 }

				 cJSON* pRetCOde = cJSON_GetObjectItem(pRoot,"ret_code");
				 
				 if(pRetCOde) iRetCode = atoi(pRetCOde->valuestring);
				 //printf("parse--%s\n",pRetCOde->valuestring);
				 cJSON* pSerialNo = cJSON_GetObjectItem(pRoot,"serialno");
				 char strSerialNo[128] ={0};
				 //printf("parse--%s\n",pSerialNo->valuestring);
				 if(pSerialNo)
				 memcpy(strSerialNo,pSerialNo->valuestring,strlen(pSerialNo->valuestring)+1);
			 
				 printf("--del_ads_stream return \n");		 

				if(iRetCode >= 0)
				{
					//��ӳɹ����ӹ���sessionID ��severID ��ֵ��
					
					pthread_mutex_lock(&m_lockerttTaskInfo);
					MapSeverInfo::iterator itlook = m_mapSwitchTaskInfo.find(strSeesionID);
					if(itlook != m_mapSwitchTaskInfo.end())
					{
						//��������һ���ỰID ��ɾ�� yyd
						m_mapSwitchTaskInfo.erase(itlook);
					}
					//���sever��map����������������ɾ����
					pFindSeverInfo->iCurrentSwitchNumber--;
					printf("----erase to map %s ,size=%d \n",strSeesionID,m_mapSwitchTaskInfo.size());
					pthread_mutex_unlock(&m_lockerttTaskInfo);
					LOG_INFO_FORMAT("----erase to map %s ,size=%d \n",strSeesionID,m_mapSwitchTaskInfo.size());
				}
				
			}
		}
	}
			
	return iRetCode;
}


int Switch_Manager::ReplyException(char* strurl,char* strOutUrl,int iRetcode)
{
	printf("-----ReplyException \n");
	LOG_DEBUG("DEBUG - [SWM]: - ReplyException \n");
	if(strOutUrl==NULL)
		return -1;
	char *Rcv_buf=strurl;
		//���Ļظ�
	Stream ptmpRequest;
		cJSON *pcmd = NULL;
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
				printf("--Add_stream request exception \n");
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

				//cJSON *pRet_root;
				ptmpRequest.pRet_root = cJSON_CreateObject();
				ptmpRequest.Requst_Json_str(2,"cmd","add_ads_stream");
				ptmpRequest.Requst_Json_str(2,"sessionid",strsessionid);

				ptmpRequest.Requst_Json_str(1,"task_id",strsessionid);
				char txt[32] ={0};
				sprintf(txt,"%d",iRetcode);
				ptmpRequest.Requst_Json_str(2,"ret_code",txt);

				ptmpRequest.Requst_Json_str(2,"serialno",strSerialNo);
				

			}
			else if(strcmp(pcmd->valuestring, "del_ads_stream") == 0)
			{
				printf("--Del_stream request exception\n");
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
				
				ptmpRequest.pRet_root = cJSON_CreateObject();
				ptmpRequest.Requst_Json_str(2,"cmd","del_ads_stream");

				ptmpRequest.Requst_Json_str(2,"sessionid",strsessionid);

				char txt[32] ={0};
				sprintf(txt,"%d",iRetcode);
				ptmpRequest.Requst_Json_str(2,"ret_code",txt);

				ptmpRequest.Requst_Json_str(2,"serialno",strSerialNo);
				
			}
			else if(strcmp(pcmd->valuestring, "reset_device") == 0)
			{
				printf("--reset_stream request exception\n");

				fflush(stdout);
				cJSON* pSerialNo = cJSON_GetObjectItem(pRoot,"serialno");

				char strSerialNo[128] ={0};
				if(pSerialNo)
					memcpy(strSerialNo,pSerialNo->valuestring,strlen(pSerialNo->valuestring)+1);

				//cJSON *pRet_root;
				ptmpRequest.pRet_root = cJSON_CreateObject();
				ptmpRequest.Requst_Json_str(2,"cmd","reset_device");

				char txt[32] ={0};
				sprintf(txt,"%d",iRetcode);
				ptmpRequest.Requst_Json_str(2,"ret_code",txt);

				ptmpRequest.Requst_Json_str(2,"serialno",strSerialNo);
				
			}
			else if (strcmp(pcmd->valuestring, "change_stream") == 0)
			{
				printf("--Change_stream request exception\n");
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

				//cJSON *pRet_root;
				ptmpRequest.pRet_root = cJSON_CreateObject();
				ptmpRequest.Requst_Json_str(2,"cmd","change_stream");

				char txt[32] ={0};
				sprintf(txt,"%d",iRetcode);
				ptmpRequest.Requst_Json_str(2,"ret_code",txt);

				ptmpRequest.Requst_Json_str(2,"serialno",strSerialNo);
				ptmpRequest.Send_Jsoon_str();


			}
			else if(strcmp(pcmd->valuestring, "check_session") == 0)
			{
				printf("--check_session exception\n");
				//ͨ��
				cJSON* psessionid = cJSON_GetObjectItem(pRoot, "sessionid");
				char strsessionid[128] ={0};
				//int iSeessionID = 0;
				if(psessionid)
				{
					memcpy(strsessionid,psessionid->valuestring,strlen(psessionid->valuestring)+1);
					//iSeessionID = atoi(strsessionid);
				}

				cJSON* pSerialNo = cJSON_GetObjectItem(pRoot,"serialno");
				char strSerialNo[128] ={0};
				if(pSerialNo)
					memcpy(strSerialNo,pSerialNo->valuestring,strlen(pSerialNo->valuestring)+1);

				//cJSON *pRet_root;
				ptmpRequest.pRet_root = cJSON_CreateObject();
				ptmpRequest.Requst_Json_str(2,"cmd","check_session");

				ptmpRequest.Requst_Json_str(2,"sessionid",strsessionid);

				ptmpRequest.Requst_Json_str(1,"task_id",strsessionid);
				char txt[32] ={0};
				sprintf(txt,"%d",iRetcode);
				ptmpRequest.Requst_Json_str(2,"ret_code",txt);

				ptmpRequest.Requst_Json_str(2,"serialno",strSerialNo);
				
			}
			else if(strcmp(pcmd->valuestring, "req_ads_stream") == 0)
			{
				printf("--req stream_session exception\n");
				//ͨ��
				cJSON* psessionid = cJSON_GetObjectItem(pRoot, "sessionid");
				char strsessionid[128] ={0};
				//int iSeessionID = 0;
				if(psessionid)
				{
					memcpy(strsessionid,psessionid->valuestring,strlen(psessionid->valuestring)+1);
					//iSeessionID = atoi(strsessionid);
				}

				cJSON* pSerialNo = cJSON_GetObjectItem(pRoot,"serialno");
				char strSerialNo[128] ={0};
				if(pSerialNo)
					memcpy(strSerialNo,pSerialNo->valuestring,strlen(pSerialNo->valuestring)+1);

				//cJSON *pRet_root;
				ptmpRequest.pRet_root = cJSON_CreateObject();
				ptmpRequest.Requst_Json_str(2,"cmd","req_ads_stream");

				ptmpRequest.Requst_Json_str(2,"sessionid",strsessionid);

				char strnoip[128]={0};
				strcpy(strnoip,"1.1.1.1");
				char strport[32]={0};
				strcpy(strport,"1");
				ptmpRequest.Requst_Json_str(1,"switch_ip",strnoip);
				ptmpRequest.Requst_Json_str(1,"switch_port",strport);
				char txt[32] ={0};
				sprintf(txt,"%d",iRetcode);
				ptmpRequest.Requst_Json_str(2,"ret_code",txt);

				ptmpRequest.Requst_Json_str(2,"serialno",strSerialNo);
				
			}


		}
	}
	
	char cJsonBuff[1024 * 2];
	char * m_tmp;
	m_tmp = cJSON_Print(ptmpRequest.pRet_root);
	memset(cJsonBuff, 0, sizeof(cJsonBuff));
	sprintf(cJsonBuff, "%sXXEE", m_tmp);
	free(m_tmp);
	printf("-----%s \n",cJsonBuff);
	fflush(stdout);
	cJSON_Delete(ptmpRequest.pRet_root);
	//copy to output 

	strcpy(strOutUrl,cJsonBuff);
	
	return 0;
}


