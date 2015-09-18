#ifndef _SM_MANAGER_H_
#define _SM_MANAGER_H_


#include <stdio.h>
#include <stdlib.h>
#include "DBInterface.h"

#include "CommonData.h"
//#include "VGW_VodStream.h"
//#include "MSI_SMStream.h"
//#include "Advertisement.h"
//#include "Navigation.h"


class Advertisement_Stream;
class Advmul_stream;
class MSI_SM_Stream ;
class Navigation_Stream ;
class VGW_Vod_Stream;





class SM_Manager
{
public:

	SM_Manager();
	~SM_Manager();
	
	bool CleanAllStream();
	bool CleanAllTask();
	bool CleanStream(int iStreamID,ModuleType emodelType=Other);
	bool Cleanonenav(char *strSeesionID);
	bool Cleanonevod(char *strSeesionID);
	bool ClearoneStream(int iStreamID,int type);

	//�·�����·���������·��
	bool InAdvanceStream(int iNavNum,int iAdverNum);

	bool CheckStreamStatus(ModuleType emodelType);
	bool ResouceRecovery(ModuleType emodelType);
/*	bool CheckVodStream();
	bool CheckNavgationStream();
	bool VODResouceRecovery();
	bool NavResouceRecovery();
*/
	//װ����ͨ������
	bool LoadStreamResource();
	bool LoadStreamStatus();
	bool LoadIPQAMInfo();
	bool LoadNetWorkGroup();
	
	int  AddOneStream(ModuleType emodelType,int iOldStreamID,int *bAddNewStream=NULL);

	bool InitStream();
	bool Tellcums(char *username);
//	bool Tellcu(char *cip,char *port);

	int Bind_OneStream(int iStreamid,char *strUserID,char* strToken,int *iChannelInfo);
	
	int AddOneVodStream(int iStreamID,char* strUrl,int iFd,char *username,char *vodname,char *posterurl,char* strSerilno);
	int AddOneNavStream(int iStreamID);
	int AddOneAdvStream(int iStreamID);
	int AfterBindAddStream(int iStreamID,bool bnewstream);
	
	bool Userbehav(int iStreamID,char *iUsertype,char *strUserName,char *Commit,int result,char *date);
	bool VodStreamOver(int iStreamID,char *username);
	bool PauseVOD(int iStreamID,char *iUsername,char *vodname,char *posterurl,int temp=0);//������ͣ
	bool RecoverVodPlay(int iStreamID,char *username,char *vodname,char *posterurl);//�ָ�VOD�㲥
	
	bool GetTaskStatus();

	bool AddStream2GroupInfo(int iStreamID,int iType);// 1Ϊ���ӣ�0Ϊɾ��

	bool BindOverTime(int iStreamID);

	bool CleanOneAdvStream(int iStreamID);

	bool Bindagain(char *iUsername,int iiStreamid);

	bool Findcsaddr(char *csip,char *csport,int iStreamID);

	bool Checksession();

	bool Unbind(char *usernam,char *streamid);

	bool Navstream(char *strSeesionId,char *strSID,char* strReSID,char *strUrl,char* strISIP,char* strSPort,\
					char* strAreaID,char *strSerialno,char *strMsg);
	//bool Navstream();
	
	//bool ChangeStream_Status(int iStreamID,char *strType,char *bind_user)

	//���󶨳�ʱ�߳�
	static void *Check_BindOverTime_thread(void *arg);

		//Advertisement_Stream *m_Advertisement;
	Navigation_Stream 	 *m_Navigation;
	Advertisement_Stream *m_Advertiser;
	VGW_Vod_Stream 	*m_VGWVodPlayer;
	MSI_SM_Stream	*m_Msi_SMInter;
	DBInterfacer * m_cSM_useract;

	Advmul_stream *m_Advmulstream;

	MapStreamResource m_mapStreamResource; //����Ϣ��Դ��
	MapIPQAMInfo	  m_mapIpqamInfo;  //iPQAM��Ϣ��
	MapNetWorkGroup   m_mapNetworkGroup; //��������

	MapStreamStatus   m_mapStreamStatus;  //װ�ص���������ʱΪ��������
	MapStreamGroup    m_mapStreamGroup; //��ͨ��������Ϣ��

	MapBindOverTime  m_mapBindStatus; //��״̬

	VectorNewNav      m_vecNewNav;  //��������

	SetPauseVod 	  m_SetPauseVod;  //��ͣ�㲥��ʱ

	MapRegionID  	  m_mapRegionID; //vod ��ӳ��˿�

	MapCsaddr		 m_mapCsaddr;
		
	int  m_idex;  //��ǰʹ�õ�id  
	SetUsedRegionID	  m_SetUsedRegionID; //����ʹ�õ�RegionID streamid regionport
	int hdisfull;
	int sdisfull;
	pthread_mutex_t m_lockerRegion;

	pthread_mutex_t m_BindStatuslocker;
	pthread_mutex_t m_VecNewNavlocker;
private:
	//����˿�ip
	char m_strdstIP[128];
	int m_iport;
	char m_NavUrl[512];
	
	//���
	char m_strSerialNo[128];
	//��ֵ�˿�
	int m_keyPort;
	int m_s2qPort;

	int m_icurrentidex;

	ListStreamResource m_ispause;
};

#endif
