#include "CommonFun.h"
#include "Config.h"
#include "cJSON.h"
#include "DBInterface.h"
#include "CommonData.h"


/*!
* \brief  ���ַ����е�һ��������һ���滻,ע��strbuf�������㹻�Ŀռ����洢�滻��Ĵ�,��������ڴ����
*
* \param
*      strbuf : �ַ���(����/���)
*      src_str : Ŀ���ַ��Ӵ�
*      desc_str     : �滻����Ӵ�
* \return
*      �滻����ַ���
*
*/

bool init(const char * ConfigFile)
{

	printf("---begin to loud configfile\n");
	fflush(stdout);
	PropConfig cfg;
	if(cfg.init(ConfigFile)==false) return false;	
	strcpy(strAdverIP,cfg.getValue("strAdverIP").c_str());
	printf("strAdverIP = %s\n",strAdverIP);
	fflush(stdout);
	iAdverPort= atoi(cfg.getValue("iAdverPort").c_str());
	printf("iAdverPort = %d\n",iAdverPort);
	fflush(stdout);
	strcpy(strNavIP,cfg.getValue("strNavIP").c_str());
	printf("strNavIP = %s\n",strNavIP);
	fflush(stdout);
	iNavPort= atoi(cfg.getValue("iNavPort").c_str());
	printf("iNavPort = %d\n",iNavPort);
	fflush(stdout);
	strcpy(strVodIP,cfg.getValue("strVodIP").c_str());
	printf("strVodIP = %s\n",strVodIP);
	fflush(stdout);
	iVodPort= atoi(cfg.getValue("iVodPort").c_str());
	printf("iVodPort = %d\n",iVodPort);
	fflush(stdout);
	strcpy(strMsiIP,cfg.getValue("strMsiIP").c_str());
	printf("strMsiIP = %s\n",strMsiIP);
	fflush(stdout);
	iMsiPort= atoi(cfg.getValue("iMsiPort").c_str());
	printf("iMsiPort = %d\n",iMsiPort);
	fflush(stdout);
	strcpy(strMyServerIP,cfg.getValue("strMyServerIP").c_str());
	printf("strMyServerIP = %s\n",strMyServerIP);
	fflush(stdout);
	iMSIServerPort= atoi(cfg.getValue("iMSIServerPort").c_str());
	printf("iMSIServerPort = %d\n",iMSIServerPort);
	fflush(stdout);
	strcpy(strVOD_KeyIP,cfg.getValue("strVOD_KeyIP").c_str());
	printf("strVOD_KeyIP = %s\n",strVOD_KeyIP);
	fflush(stdout);
	strcpy(dbip,cfg.getValue("dbip").c_str());
	printf("dbip = %s\n",dbip);
	fflush(stdout);
	strcpy(dbname,cfg.getValue("dbname").c_str());
	printf("dbname = %s\n",dbname);
	fflush(stdout);
	strcpy(dbuser,cfg.getValue("dbuser").c_str());
	printf("dbuser = %s\n",dbuser);
	fflush(stdout);
	strcpy(dbpass,cfg.getValue("dbpass").c_str());
	printf("dbpass = %s\n",dbpass);
	fflush(stdout);
	strcpy(pauseurl,cfg.getValue("pauseurl").c_str());
	printf("pauseurl = %s\n",pauseurl);
	fflush(stdout);
	

//�˳�ҳ��
	strcpy(quiturl,cfg.getValue("quiturl").c_str());
	printf("quiturl = %s\n",quiturl);
	fflush(stdout);
	
//�鲥���ߵ�ַ
	strcpy(advip,cfg.getValue("advip").c_str());
	printf("advip = %s\n",advip);
	fflush(stdout);
	
	muladvport= atoi(cfg.getValue("muladvport").c_str());
	printf("iadvport = %d\n",muladvport);
	fflush(stdout);
//goback ҳ��(navgoback)
	strcpy(navgoback,cfg.getValue("navgoback").c_str());
	printf("navgoback = %s\n",navgoback);
	fflush(stdout);
	
//������ص�ַ�˿�
	strcpy(strsdNavIP,cfg.getValue("strsdNavIP").c_str());
	printf("strsdNavIP = %s\n",strsdNavIP);
	fflush(stdout);

	isdNavPort= atoi(cfg.getValue("isdNavPort").c_str());
	printf("isdNavPort = %d\n",isdNavPort);
	fflush(stdout);
//���ø߱��������
	strcpy(hdrate,cfg.getValue("hdrate").c_str());
	printf("hdrate = %s\n",hdrate);
	fflush(stdout);
	

	strcpy(sdrate,cfg.getValue("sdrate").c_str());
	printf("sdrate = %s\n",sdrate);
	fflush(stdout);
//�������������
	strcpy(sdpauseurl,cfg.getValue("sdpauseurl").c_str());
	printf("sdpauseurl = %s\n",sdpauseurl);
	fflush(stdout);

	strcpy(sdquiturl,cfg.getValue("sdquiturl").c_str());
	printf("sdquiturl = %s\n",sdquiturl);
	fflush(stdout);

	strcpy(sdnavgoback,cfg.getValue("sdnavgoback").c_str());
	printf("sdnavgoback = %s\n",sdnavgoback);
	fflush(stdout);
//�����״̬���ʱ��
   //iwaittime
	iwaittime= atoi(cfg.getValue("iwaittime").c_str());
	printf("iwaittime = %d\n",iwaittime);
	fflush(stdout);
	
	printf("--lode is ok\n");
	fflush(stdout);
//VGW�����Ƶ���pid
	frequency = atoi(cfg.getValue("frequency").c_str());
	pid =  atoi(cfg.getValue("pid").c_str());
	printf("frequency = %d,pid = %d\n",frequency,pid);
	fflush(stdout);
//�Ƿ�ʹ�þ���������
    advflag =  atoi(cfg.getValue("advflag").c_str());
	printf("advflag = %d\n",advflag);
	fflush(stdout);
//������������IP��Port
	strcpy(strblanIP,cfg.getValue("strblanIP").c_str());
	printf("strblanIP = %s\n",strblanIP);
	fflush(stdout);
	
	iBlanport =  atoi(cfg.getValue("iBlanport").c_str());
	printf("iBlanport = %d",iBlanport);
	fflush(stdout);
	
//������־����Ĳ���
	char log_file_path[64]= {0};
  	char log_file[32] = {0};
	char SW_version[32] = {"V1.0.00.1"};
	int log_level;
	int log_size;
	int log_num;
	strcpy(log_file_path,cfg.getValue("SM.LOG_FILE_PATH").c_str());
	strcpy(log_file,cfg.getValue("SM.LOG_FILE").c_str());
	log_level=atoi(cfg.getValue("SM.LOG_LEVEL").c_str());
	log_size=atoi(cfg.getValue("SM.LOG_SIZE").c_str());
	log_num=atoi(cfg.getValue("SM.LOG_NUM").c_str());

	printf("load config is  ----------------------\n");
	printf("SWM_Version = %s\n",SW_version);
	printf("log_file_path = %s\n",log_file_path);
	printf("log_file = %s\n",log_file);
	printf("log_level = %d\n",log_level);
	printf("log_size = %d\n",log_size);
	printf("log_num = %d\n",log_num);
	fflush(stdout);

	LogFactory::instance()->init(log_file_path,
						log_file,
						log_level,
						LOGOUT_FILE,
						log_size,
						log_num);
	LogFactory::instance()->start();

	LOG_INFO("INFO  - [SWM]:��ʼ����\n");
	LOG_INFO("INFO  - [SWM]:��ȡSWM�����ļ�\n");
	LOG_INFO_FORMAT("INFO  - [SM]:����汾[SM.version] = %s\n",SW_version);
	LOG_INFO_FORMAT("INFO  - [SM]:����˿�[SM.tcpserver.port] = %s:%d\n",strMyServerIP,iMSIServerPort);
	return true;

}
char * replace(char *strbuf, const char *src_str, const char *desc_str)
{

	char *pos, *pos1;

	if (strbuf == NULL || src_str == NULL || desc_str == NULL) return strbuf;

	if (strlen(src_str) == 0) return strbuf;
	int ilen = strlen(strbuf);
	char *org = new char[ilen + 1];

	strcpy(org, strbuf);
	pos = org;
	strbuf[0] = 0;
	while (1)
	{
		pos1 = strstr(pos, src_str);
		if (pos1 == NULL)
			break;
		*pos1 = '\0';
		strcat(strbuf,  pos);
		strcat(strbuf, desc_str);
		pos = pos1 + strlen(src_str);
	}
	strcat(strbuf,  pos);
	delete org;
	return strbuf;
}

