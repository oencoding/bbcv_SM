#ifndef	VNC_WORKER_RSM_H_
#define	VNC_WORKER_RSM_H_

#include "Socket_Ractor.h"
#include "Record.h"
#include "Log.h"
#include "vnc_config.h"
#include "vnc_worker_Xvnc.h"
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include "ppmtest.h"


using namespace std;
/*
	��Ϊʾ��,�˴��������ؾ����ö�����,�Ҳ����ַ�����ʽ��
	1����socket��ȡ����
	2���������ݣ��ж�������ȷ��
	3�����������ͽ��д���
	   3.1 ����ע�᣺XXBBCOH_REGISTER_REQ|GW_TYPE|GW_IP|GW_IP|GW_PORTXXEE �ظ���XXBBCOH_REGISTER_RSP|RETURN_CODEXXEE
	   3.2 �û���¼��XXBBCOC_TERM_LOGIN_REQ|TERM_TYPE|TERM_ID|REGION_ID|CALLBACK_IP|CALLBACK_PORTXXEE �ظ���XXBBCOH_REGISTER_RSP|RETURN_CODEXXEE
	   3.2 �û��˳���XXBBCOC_TERM_LOGOUT_REQ|TERM_TYPE|TERM_ID|REGION_IDXXEE �ظ���XXBBCOH_REGISTER_RSP|RETURN_CODEXXEE
	4�����ؿ�ܣ���Ϊ�Ƕ����ӣ��򷵻�-1��Ҫ���ܹر�socket
	
	����cscs�ı��ĸ�ʽ��ʱΪ��
	XXBBCOC_TERM_LOGIN_REQ|TERM_ID|TERM_TYPE|REGION_IDXXEE
	XXBBCOC_TERM_LOGOUT_REQ|TERM_ID|TERM_TYPE|REGION_IDXXEE
*/

class Prase_Packet
{
public:
	string cmd;
	string session_id;
	int time_out;
	string url;
	int size;

	bool init_para(char *str,const char * flag);
	bool Set_Para(int i,char * data);

};

class Rsm_AiM_Handler : public Socket_Svc_Handler
{
public:
	 FILE 	*fp;
	 int    handle_process();	 //�߼�������	
 
protected:
	 bool   createVnc(const Prase_Packet &p_Element,string &ret_string);
	 bool   freeVnc(const Prase_Packet &p_Element,string &ret_string);
	 
};


typedef Socket_Ractor<Socket_Selector,Rsm_AiM_Handler>  Rsm_AiM_Worker;

typedef Singleton<Rsm_AiM_Worker>  Vnc_sngton;

#endif 



