char strAdverIP[128];
int iAdverPort;

char strNavIP[128];
int iNavPort;

char strVodIP[128];
int iVodPort;

char strMsiIP[128];
int iMsiPort;

char strMyServerIP[128];
int iMSIServerPort;
int iMContrlport;

char strVOD_KeyIP[128];

char dbip[128];
char dbname[128];
char dbuser[128];
char dbpass[128];

char hdadvname[128];
char hdadvip[128];
char hdadvport[64];

char advip[128];
int  muladvport;

char pauseurl[256];
char quiturl[256];

char strsdNavIP[128];
int isdNavPort;

char hdrate[64];
char sdrate[64];

char navgoback[256];

char sdpauseurl[256];
char sdquiturl[256];
char sdnavgoback[256];

char sdadvname[128];
char sdadvip[128];
char sdadvport[64];

int iwaittime;
int checkflag;
	
int frequency;
int pid;

int advflag;   //�Ƿ�ʹ�þ���������1:ʹ�� 0:��ʹ��

char strblanIP[64];
int iBlanport;

int iBaseport;

int Istype = 1;                            //VGW������  0:ENRICH  1:SIHUA  2:VLC  
int VOD_play_clean;              //�ͷţ� ����� ���� ������  ����  ���+CDN��
int VodStreamOver_clean;		//VodStreamOver ��goback������  �������������cdn����
int BindOverTime_clean = 1000*1800;			//BindOverTime �ͷţ�������
int RecoverVodPlay_clean = 1000*1000;		//RecoverVodPlay �ͷţ������� 
int InitStream_clean = 1000*1000;			//InitStream  ��ʼ���ͷ�������
int PauseVOD_clean;				//PauseVOD �ͷŵ�һ·����� 
int Recovery;