#ifndef _COMMONDATA_H_
#define _COMMONDATA_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <map>
#include <vector>
#include <list>
#include <semaphore.h>
#include <set>
#include <string>


//#define USEMAP
using namespace std;
const char Status_Idle[32] ="A";  //����
const char Status_Adver[32] ="B"; //���
const char Status_Nav[32] ="C";		//����
const char Status_Nav_Home[32] ="D"; //������ҳ
const char Status_Vod[32] ="E";	//vod
const char Status_abnormal[32] = "F";//�쳣
typedef enum _ModuleType
{
	Advertisement =0 ,
	Navigation    ,
	VodStream,
	MobileGateservice,
	Other,
	vodplay
}ModuleType;

 const int CIRegionID[3] = {50452,50450,50454};
 const char CstrRegion[3][64]={"0x0601","0x0603","0x0602"};
 const int Baseport = 52100;
 
 extern char strAdverIP[128];
 extern int iAdverPort;

 extern char strNavIP[128];
 extern int iNavPort;

 extern char strVodIP[128];
 extern int iVodPort;

 extern char strMsiIP[128];
 extern int iMsiPort;

 extern char strMyServerIP[128];
 extern int iMSIServerPort;

 extern char strVOD_KeyIP[128];

 extern char dbip[128];
 extern char dbuser[128];
 extern char dbpass[128];
 extern char dbname[128];

 extern char hdadvname[128];
 extern char hdadvip[128];
 extern char hdadvport[64];

 extern char advip[128];
 extern int muladvport;

 extern char quiturl[256];
 extern char pauseurl[256];

 extern char navgoback[256];

 extern char strsdNavIP[128];
 extern int isdNavPort;

 extern char hdrate[64];
 extern char sdrate[64];

 extern char sdadvname[128];
 extern char sdadvip[128];
 extern char sdadvport[64];

extern char sdpauseurl[256];
extern char sdquiturl[256];
extern char sdnavgoback[256];

extern int iwaittime;

extern int VOD_play_clean;
extern int VodStreamOver_clean; 
extern int BindOverTime_clean;
extern int RecoverVodPlay_clean;
extern int InitStream_clean;
extern int PauseVOD_clean; 

extern int frequency;
extern int pid;

extern int Istype;
extern int advflag;

extern char strblanIP[64];
extern int iBlanport;

extern int iBaseport;

extern int Recovery;


const int iMulitNum = 5;
const char strVLCMulitURL[5][128] = {"udp://225.0.0.1:10000","udp://225.0.0.1:12000","udp://225.0.0.1:12000",
					"udp://225.0.0.1:10000","udp://225.0.0.1:12000"};

const int iBindOverTime = 60; //sec

typedef struct _StreamStatus
{
	int istreamID;
	char strStreamType[32];
	char strStatus_date[128];
	char strBind_userID[128];
	char strBind_date[128];
	char strSwitch_task_id[128];
	char strSessionID[512];
	char strSerialNo[512];
	//int iTaskID;

}StreamStatus;

typedef struct _UserBehaviour
{

	int iID;
	int iStreamID;
	char strUserName[128];
	char strActionType[128];
	char strActionDate[128];
	char strNetworkComment[512];
	int result;

}UserBehaviour;

typedef struct _NetworkGroup
{

	char strNetRegionNum[128];
	char strNetRegionName[128];
	int hdiNavgationStreamNum;
	int sdiNavgationStreamNum;
	int hdiAdvertisementStreamNum;
	int sdiAdvertisementStreamNum;
	char strNetworkComment[256];

}NetworkGroup;


typedef struct _StreamResource
{
	int iStreamID;
	int iIPQAMNum;
	char strNetRegionNum[64];
	char strNav_url[512];
	int iChannel_info;
	int iWherether_HD;
}StreamResource;

typedef struct _IPQAMInfo
{
	int iIPQAMNum;
	char strIpqamIP[128];
	int iIpqamManagerPort;
	
}IPQAMInfo;


typedef struct _Csaddr
{
	char csip[128];
	char csport[64];
}Csaddr;

typedef struct _Advinfo
{
	char advip[128];
	char advport[64];
}Advinfo;

typedef struct _Smstream
{
	char sadvip[64];
	char sadvport[64];
}Smstream;

typedef std::map<int,StreamResource*> MapStreamResource;
typedef std::map<int,StreamStatus*> MapStreamStatus;
typedef std::map<int,IPQAMInfo*>  MapIPQAMInfo;
typedef std::map<char*,NetworkGroup*> MapNetWorkGroup;

typedef std::map<int,Csaddr*> MapCsaddr;

typedef std::list<int> ListStreamResource;
typedef std::map< char*, ListStreamResource> MapStreamGroup;
typedef std::set<int> VectorNewNav;  
typedef std::list<int> Sockfd;

typedef std::set<int> SetGroupStream;  //һ���ͨ��

typedef std::map<int,time_t> MapBindOverTime;//��Ű�״̬

typedef std::set<int>  SetPauseVod;

typedef std::map<int,char*> MapRegionID;
typedef std::map<int,int> SetUsedRegionID;

typedef std::map<int,Advinfo*> SetAdvinfo; //װ�ع������Ϣ

typedef std::map<int,int> Navinfo;   //װ�ص�ǰ����Ӧ�ĵ��������

typedef std::list<int> Navnext;    //װ�ؼ����·��ĵ������ı��

typedef std::list<int> Streatype; //������������״̬

typedef std::list<Smstream*> Smstrinfo;//���ڼ�¼��ǰ��������ip��port

typedef std::map<int,Smstream*> Smvodaddr;//���ڼ�¼VOD�����������ĵ�ַ

typedef std::map<int,int> StreamAbnormal;//���ڴ��check������������״̬

typedef std::map<int,int> FailNav;//���ڴ�ͬһ������δ�·��ɹ��ĵ������ĸ���
 
#endif
