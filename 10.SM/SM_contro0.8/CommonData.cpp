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

char strVOD_KeyIP[128];

char dbip[128];
char dbname[128];
char dbuser[128];
char dbpass[128];

int VOD_play_clean = 1000*1000;              //�ͷţ� ����� ���� ������  ����  ���+CDN��
int VodStreamOver_clean = 1000*2000;		//VodStreamOver ��goback������  �������������cdn����
int BindOverTime_clean = 1000*2000;			//BindOverTime �ͷţ�������
int RecoverVodPlay_clean = 1000*1500;		//RecoverVodPlay �ͷţ������� 
int InitStream_clean = 1000*1000;			//InitStream  ��ʼ���ͷ�������
int PauseVOD_clean = 1000*1500;				//PauseVOD �ͷŵ�һ·����� 