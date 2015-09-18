#include "Switch_Stream.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>



Switch_Stream::Switch_Stream(int iUsedPort)
{

	m_iBeginPort = iUsedPort;
	m_bSwitchFlag = true;

	read_index = 0;
	write_index = 0; 
	pthread_mutex_init(&m_mutex, NULL);
	pthread_mutex_init(&m_mutexlocker, NULL);

	buffer_max_size = 10*1024*1024;
	buffer = NULL;
//	m_iRcvThreadID = -1;
	m_iSendRate = 8800000;

	m_vectSendAddr.clear();
	m_vectSwitchInfo.clear();
}
Switch_Stream::~Switch_Stream()
{
	close(m_BindSocket);
	if(buffer)
		free(buffer);
	pthread_mutex_destroy(&m_mutexlocker);
	pthread_mutex_destroy(&m_mutex);
}

int Switch_Stream::InitRecvStream(SwitchInfo *cSwitchInfo)  //���ն˿� ���Ͷ˿�
{
	//������

	//int iRecvPort = cSwitchInfo.iSRCPort;
//	int iDstPort = cSwitchInfo.iDstPort;

	//�Ȳ��ҵ�ǰ�Ƿ񱻰�
	if(m_vectSendAddr.size() > 0)
	{
		pthread_mutex_lock(&m_mutexlocker);
		//��Ӷ�·��Ϣ
		m_vectSwitchInfo.push_back(cSwitchInfo);

		struct sockaddr_in *send_addr =  (struct sockaddr_in *)malloc(sizeof(struct sockaddr));
		memset(send_addr,0,sizeof(struct sockaddr));
		send_addr->sin_family=AF_INET;
		send_addr->sin_port = htons(cSwitchInfo->iDstPort);
		send_addr->sin_addr.s_addr = inet_addr(cSwitchInfo->strDstIP);

		
		m_vectSendAddr.push_back(send_addr);

		pthread_mutex_unlock(&m_mutexlocker);
		printf("=====push back in send addr size =%d \n",m_vectSendAddr.size());
		fflush(stdout);
		
	}
	else
	{

		memset(&m_cSwitchInfo,0,sizeof(SwitchInfo));

		m_cSwitchInfo.iDstPort = cSwitchInfo->iDstPort;
		strcpy(m_cSwitchInfo.strDstIP,cSwitchInfo->strDstIP);
		m_cSwitchInfo.iSRCPort = cSwitchInfo->iSRCPort;
		strcpy(m_cSwitchInfo.strSRCIP,cSwitchInfo->strSRCIP);

		printf("------stream -----\n");
		printf("src %s %d \n",m_cSwitchInfo.strSRCIP,m_cSwitchInfo.iSRCPort);
		printf("dst %s %d \n",m_cSwitchInfo.strDstIP,m_cSwitchInfo.iDstPort);	
		fflush(stdout);
		int sock = -1;

		struct sockaddr_in s_addr;
		if ( (sock = socket(AF_INET, SOCK_DGRAM, 0))  == -1) {
			perror("socket");
			fflush(stderr);
			//exit(errno);
			return -1;
		} else
			printf("create socket.\n\r");
		fflush(stdout);

		memset(&s_addr, 0, sizeof(struct sockaddr_in));
		s_addr.sin_family = AF_INET;

		s_addr.sin_port = htons(m_cSwitchInfo.iSRCPort);


		s_addr.sin_addr.s_addr = INADDR_ANY;   //����
		//s_addr.sin_addr.s_addr = inet_addr("225.0.0.1");

		//inet_aton("225.0.0.1", &s_addr.sin_addr) 

		int optval = 1;
		if ((setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(int))) == -1)
		{
		//	close(sock);
		//	return -1;
			printf("******reuseaddr %d port failed \n",m_cSwitchInfo.iSRCPort);
			fflush(stdout);
		}


				int nRecvBuf = 0;
		socklen_t iLen = 4;
		getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &nRecvBuf, &iLen);
		nRecvBuf = 1024*1024;//����Ϊ
		setsockopt(sock,SOL_SOCKET,SO_RCVBUF,&nRecvBuf,sizeof(socklen_t));
		int nSize = 0;
		getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &nSize, &iLen);

		printf("---get socket rcvbuf size =%d\n",nSize);

		if ( (bind(sock, (struct sockaddr*)&s_addr, sizeof(s_addr))) == -1 ) {
			perror("bind");
			fflush(stderr);
			printf("******bind %d port failed \n",m_cSwitchInfo.iSRCPort);
			fflush(stdout);
			return -1;
		}else
		{
			printf("bind success address to socket.\n\r");
			
		}

		
		fflush(stdout);
	    if (-1 == fcntl(sock, F_SETFL, O_NONBLOCK))
	    {
	        printf("fcntl socket error!\n");
			fflush(stdout);
	        return -1;
	    }	

                                              /*���ûػ����*/
	    int loop = 1;
	    int err = setsockopt(sock,IPPROTO_IP, IP_MULTICAST_LOOP,&loop, sizeof(loop));
	    if(err < 0)
	    {
	        perror("setsockopt():IP_MULTICAST_LOOP");
			fflush(stderr);
	        return -1;
	    }

		struct ip_mreq mreq;                                    /*����ಥ��*/
	    mreq.imr_multiaddr.s_addr = inet_addr(MCAST_ADDR); /*�ಥ��ַ*/
	    mreq.imr_interface.s_addr = htonl(INADDR_ANY); /*����ӿ�ΪĬ��*/
															/*����������ಥ��*/
	    err = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,&mreq, sizeof(mreq));
	    if (err < 0)
	    {
	        perror("setsockopt():IP_ADD_MEMBERSHIP");
	        return -1;
	    }

		
		
		m_BindSocket = sock;

		pthread_mutex_lock(&m_mutexlocker);
		m_vectSwitchInfo.push_back(cSwitchInfo);
		struct sockaddr_in *send_addr =  (struct sockaddr_in *)malloc(sizeof(struct sockaddr));
		memset(send_addr,0,sizeof(struct sockaddr));
		send_addr->sin_family=AF_INET;
		send_addr->sin_port = htons(cSwitchInfo->iDstPort);
		send_addr->sin_addr.s_addr = inet_addr(cSwitchInfo->strDstIP);

		m_vectSendAddr.push_back(send_addr);
		pthread_mutex_unlock(&m_mutexlocker);
		//���������߳�

//		buffer = (char *)malloc(buffer_max_size);


			pthread_attr_t attr;	
		pthread_attr_init(&attr);	
		pthread_attr_setstacksize(&attr,10*1024*1024);
		//pthread_t tcp_recv_thread4;
		pthread_create(&m_iRcvThreadID, NULL, Parse_recv_thread, this);
/*
		if (pthread_create(&write_thread_id, &attr, &ts_send_thread, this) != 0)		
		{
			fprintf(stderr, "libtssmooth pthread_create failed...\n");
			fflush(stderr);
			pthread_cancel(read_thread_id);
			pthread_mutex_destroy(&m_mutex);
			free(buffer);
			return 0;
		}
		//pthread_detach(tcp_recv_thread4);
*/		
	}
	return 0;
}

bool Switch_Stream::PutInBuff(char* UDP_buf,int recv_len)
{
	pthread_mutex_lock(&m_mutex);
	if(write_index - read_index + recv_len > buffer_max_size)
	{
		pthread_mutex_unlock(&m_mutex);
		printf("%s %d Waring,buffer is too small,or send is slow,it will reset!\n",__FUNCTION__,__LINE__);

		fflush(stdout);
		//reset
		write_index = 0;
		read_index = 0;
		//
		//continue;
	}
	
	if(buffer_max_size - write_index < recv_len)
	{
		memmove(buffer,buffer+read_index,write_index - read_index);
	
		write_index = write_index - read_index;
		read_index = 0;
	}
	
	memcpy(buffer+write_index,UDP_buf,recv_len);
	write_index += recv_len;
	
	pthread_mutex_unlock(&m_mutex);

	return true;
}

bool Switch_Stream::GetBuff(char* send_buffer,int *ilen)
{
	int iGetlen = 7*188;
	pthread_mutex_lock(&m_mutex);
	if(write_index - read_index <= 0)
	{
		pthread_mutex_unlock(&m_mutex);
		//usleep(20*1000);	
	//	continue;
		return false;
	}
	
	if(write_index - read_index >= iGetlen)
	{
		memcpy(send_buffer,buffer+read_index,iGetlen);
		*ilen = iGetlen;
		read_index += iGetlen;
	}
	else
	{
		memcpy(send_buffer,buffer+read_index,write_index - read_index);
		*ilen= write_index - read_index;
		read_index = write_index;
	}
	pthread_mutex_unlock(&m_mutex);

	return true;
}


void *Switch_Stream::Parse_recv_thread(void *arg)
{
	Switch_Stream *this0 = (Switch_Stream*)arg;
	Switch_Stream *param = (Switch_Stream*)arg;
	int bindSock = this0->m_BindSocket;

	fprintf(stderr,"create recv thread success \n");
	fflush(stderr);
	uint8_t UDP_buf[4096];

	struct sockaddr_in c_addr;

	socklen_t addr_len;
	addr_len = sizeof(struct sockaddr);

	printf("---%s %d  dst %s %d \n",this0->m_cSwitchInfo.strSRCIP,this0->m_cSwitchInfo.iSRCPort,
				this0->m_cSwitchInfo.strDstIP,this0->m_cSwitchInfo.iDstPort);
	//��ȡ��ǰ�߳�

	fflush(stdout);
//	struct sockaddr_in send_addr;
//	send_addr.sin_family=AF_INET;
//	send_addr.sin_port = htons(this0->m_cSwitchInfo.iDstPort);
//	send_addr.sin_addr.s_addr = inet_addr(this0->m_cSwitchInfo.strDstIP);
	int len = 0;
	int iflag = 0;

		struct timeval tv1,tv2;
		long long time1,time2;
		long long nsendbytes = 0;
		long long totaltime = 0;
		long now_bit_rate = 0;

		gettimeofday(&tv1, NULL);
		time1 = tv1.tv_sec*1000 + tv1.tv_usec / 1000;

	int iSendTime = 0;
		
	while(1)
	{
				//�ж��Ƿ�����Ҫ���յ�����
		if(!this0->m_bSwitchFlag)
			break;
		len = recvfrom(bindSock, UDP_buf, sizeof(UDP_buf)-1, 0, (struct sockaddr*)&c_addr, &addr_len);
		if (len <= 0) {
			usleep(50);
			continue;
		}
		if(iflag == 0)
		{
			printf("-----recv data len=%d \n",len);
			int iport = ntohs(c_addr.sin_port);
			char srcIPAddr[128]={0};

			strcpy(srcIPAddr,inet_ntoa(c_addr.sin_addr));
			printf("--recv ip %s ,port=%d \n",srcIPAddr,iport);

			fflush(stdout);
		}
		iflag = 1;


		pthread_mutex_lock(&this0->m_mutexlocker);
		VectSendAddr::iterator itfid = this0->m_vectSendAddr.begin();
		while(itfid != this0->m_vectSendAddr.end())
		{
			struct sockaddr_in *send_addr = *itfid;
			int iret = 0;
			int iTotal = 0;
			do
			{
				int flags = 0;
				//flags = MSG_DONTWAIT;
				iret = sendto(bindSock,UDP_buf+iTotal,len-iTotal,0,(struct sockaddr*)send_addr,sizeof(struct sockaddr));
				if(iret < 0)
				{
					perror("sendto");
				}

#if 0
				if(iret < 0)
				{
					perror("sendto");
					if(errno == EAGAIN || errno == EWOULDBLOCK)
					{//exit(errno);
						printf("----error no eagain continue\n");
						usleep(1000);
						iret = sendto(bindSock,UDP_buf+iTotal,len-iTotal,0,(struct sockaddr*)send_addr,sizeof(struct sockaddr));
					}
					else
					{
						printf("----error end \n");
						fflush(stdout);
						pthread_mutex_unlock(&this0->m_mutexlocker);
						close(bindSock);
						exit(errno);

					}
					break;
				}
#endif				
				if(iret > 0)
					iTotal += iret;
				
				if(++iSendTime > 5)
				{
					usleep(100);
					iSendTime=0;
				}
					
			}while(iret < len);
			
			++itfid;
		}
		pthread_mutex_unlock(&this0->m_mutexlocker);

		/*

		if(!this0->PutInBuff((char*)UDP_buf,len))
		{
			printf("----error input buff \n");
		}


		nsendbytes += len;
		
		gettimeofday(&tv2, NULL);
		time2 = tv2.tv_sec*1000 + tv2.tv_usec / 1000;

		if(time2 - time1 > 1000)
		{
			now_bit_rate = nsendbytes*8*1000/(time2 - time1);

			nsendbytes = 0;
			if(this0->m_iSendRate != now_bit_rate)
			{
				//this0->m_iSendRate = now_bit_rate;
				printf("----rcv rate = %d \n",now_bit_rate);
			}
		}
			


		pthread_mutex_lock(&this0->m_mutexlocker);
		VectSendAddr::iterator itfid = this0->m_vectSendAddr.begin();
		while(itfid != this0->m_vectSendAddr.end())
		{
			struct sockaddr_in *send_addr = *itfid;
			int iret = sendto(bindSock,UDP_buf,len,0,(struct sockaddr*)send_addr,sizeof(struct sockaddr));
			if(iret < 0)
			{
				perror("sendto");
				//exit(errno);
				pthread_mutex_unlock(&this0->m_mutexlocker);
				close(bindSock);
				return NULL;
			}
			
			++itfid;
		}
		pthread_mutex_unlock(&this0->m_mutexlocker);
	
*/
		

	}
	close(bindSock);	
}



void * Switch_Stream::ts_send_thread(void *arg)
{
	Switch_Stream *this0 = (Switch_Stream*)arg;
	Switch_Stream *param = (Switch_Stream*)arg;

	int bindSock = this0->m_BindSocket;
	fprintf(stderr,"create send thread success \n");
	fflush(stderr);
	uint8_t UDP_buf[4096];

	struct sockaddr_in c_addr;

	socklen_t addr_len;
	addr_len = sizeof(struct sockaddr);


	struct timeval tv1,tv2;
	long long time1,time2;
	long long nsendbytes = 0;
	long long totaltime = 0;
	long now_bit_rate = 0;

	gettimeofday(&tv1, NULL);
	time1 = tv1.tv_sec*1000 + tv1.tv_usec / 1000;

	//long bit_rate = this0->m_iSendRate;
	while(1)
	{

		gettimeofday(&tv2, NULL);
		time2 = tv2.tv_sec*1000 + tv2.tv_usec / 1000;
		if(time2 - time1 > 0)
			now_bit_rate = nsendbytes*8*1000/(time2 - time1);
		else
			now_bit_rate = this0->m_iSendRate;;

		if(now_bit_rate >= this0->m_iSendRate)
		{
			usleep(5*100);	
			//printf("now_bit_rate is too large %d %d\n",now_bit_rate,this0->m_iSendRate);
			continue;
		}
		
		if(time2 - time1 >= 300)
		{
			printf("-----bitrate =%d \n",now_bit_rate);
			gettimeofday(&tv1, NULL);
			time1 = tv1.tv_sec*1000 + tv1.tv_usec / 1000;
			nsendbytes = 0;
		}
		
		//ȡ��ת������
		int len = 0;
		this0->GetBuff((char*)UDP_buf,&len);
		if(len <=0)
		{
			usleep(20*100);
			continue;
		}

		int send_len = 0;
		pthread_mutex_lock(&this0->m_mutexlocker);
		VectSendAddr::iterator itfid = this0->m_vectSendAddr.begin();
		while(itfid != this0->m_vectSendAddr.end())
		{
			struct sockaddr_in *send_addr = *itfid;



			int iret = sendto(bindSock,UDP_buf,len,0,(struct sockaddr*)send_addr,sizeof(struct sockaddr));
			if(iret < 0)
			{
				perror("sendto");
				//exit(errno);
				pthread_mutex_unlock(&this0->m_mutexlocker);
				close(bindSock);
				return NULL;
			}
			send_len = iret;
			
			++itfid;
		}
		pthread_mutex_unlock(&this0->m_mutexlocker);

		nsendbytes +=  send_len;
		
	}
	
}


bool Switch_Stream::EndStream(SwitchInfo *cSwitchInfo)
{
	printf("-----begin lock \n");
	fflush(stdout);
	pthread_mutex_lock(&m_mutexlocker);

	VectSendAddr::iterator itfind = m_vectSendAddr.begin();
	while(itfind != m_vectSendAddr.end())
	{
		printf("-----find one switch \n");
		struct sockaddr_in *send_addr = *itfind;
		int iDst = ntohs(send_addr->sin_port);
		char strDstIP[128]={0};
		strcpy(strDstIP,inet_ntoa(send_addr->sin_addr));

		printf("-----end dst port =%d \n",iDst);
		fflush(stdout);
		if(iDst == cSwitchInfo->iDstPort && strcmp(strDstIP,cSwitchInfo->strDstIP)==0)
		{
			//�Ƴ�����
			m_vectSendAddr.erase(itfind);
			
			printf("=====pop back in send addr size=%d \n",m_vectSendAddr.size());
			fflush(stdout);
			break;
		}
		++itfind;
	}
	pthread_mutex_unlock(&m_mutexlocker);
	
	printf("---leave lock\n");
	fflush(stdout);
	return true;
}


bool Switch_Stream::IsCanDelete()
{
	printf("----delete begin \n");
	fflush(stdout);
	if(m_vectSendAddr.size() ==0)
	{
	//	��
	//	pthread_join(m_iRcvThreadID,NULL);
		m_bSwitchFlag = false;
		close(m_BindSocket);
		usleep(1000*20);
	//	pthread_join(m_iRcvThreadID,NULL);
		return true;

	}

	return false;
}	


