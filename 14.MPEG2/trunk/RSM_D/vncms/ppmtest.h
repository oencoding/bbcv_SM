#ifndef		_PPM_H_
#define		_PPM_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <net/if.h>
#include <netinet/in.h>
#include <linux/sockios.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "rfbclient.h"
#define UDP_PORT_PLUS 40000

typedef enum {
	NblTermKeyDevType_IrrControl     	= 1001,//ң�����豸
	NblTermKeyDevType_WifiControl		= 1002,//WIFI�豸
	NblTermKeyDevType_Console  		= 1003,//��Ϸ�ֱ��豸
	NblTermKeyDevType_Keyboard  		= 1004,//�����豸
	NblTermKeyDevType_Mouse   			= 1005,//����豸
} NblStbTermKeyDevType;

typedef enum {
    BlcMousePropertyStat_BUTTONLEFT     = 1,
    BlcMousePropertyStat_BUTTONMIDDLE   = 2,
    BlcMousePropertyStat_BUTTONRIGHT    = 3,
} BlcMousePropertyKey;

typedef enum {
    BlcMousePropertyStat_MOUSEDEFAULT   = 1,
    BlcMousePropertyStat_MOUSEDOWN      = 2,
    BlcMousePropertyStat_MOUSEUP        = 3,
    BlcMousePropertyStat_MOUSEWHEEL     = 4, //����ת��   
    BlcMousePropertyStat_MOUSELEFTDRAG  = 2, //����϶�������5  ��������down ���
    BlcMousePropertyStat_MOUSERIGHTDRAG = 2, //�Ҽ��϶�������6  ��������down ���
}BlcMousePropertyKeyStat;



/***********************���͸�Ӧ����Ϣ�ṹ�嶨��*************************************************/
typedef struct {
unsigned int dev_type; // ��ֵ�豸���� BlcYunKeyDevType
} BlcYunKeyMsgHead;

typedef struct {//����
unsigned int sequence_num; // ���кţ����ն��ƶ�
unsigned int key_value; // ��ֵ
unsigned int key_status; // ��ֵ����

unsigned int term_type; //BlcStbTermType
unsigned char term_id[32];
} BlcYunKeyIrrMsgBody;

typedef struct {
    BlcYunKeyMsgHead head; // ��Ϣͷ
BlcYunKeyIrrMsgBody body; // ��Ϣ��
} BlcYunKeyIrrMsg;

typedef struct {//����
unsigned int sequence_num; // ���кţ����ն��ƶ�
unsigned int key_value; // ��ֵ
unsigned int key_status; // ��ֵ����
} BlcYunKeyKeyboardMsgBody;

typedef struct {
    BlcYunKeyMsgHead head; // ��Ϣͷ
BlcYunKeyKeyboardMsgBody body; // ��Ϣ��
} BlcYunKeyKeyboardMsg;

typedef struct {//��Ϸ�ֱ�
unsigned int sequence_num; // ���кţ����ն��ƶ�
unsigned int key_value; // ��ֵ
unsigned int key_status; // ��ֵ����
short x; //ϵͳ����x
    short y; //ϵͳ����y
    short cursor_x; //��Ļ���x
    short cursor_y; //��Ļ���y
} BlcYunKeyConsoleMsgBody;

typedef struct {
    BlcYunKeyMsgHead head; // ��Ϣͷ
BlcYunKeyConsoleMsgBody body; // ��Ϣ��
} BlcYunKeyConsoleMsg;

typedef struct {//���
unsigned int sequence_num; // ���кţ����ն��ƶ�
unsigned int key_value; // ��ֵ
unsigned int key_status; // ��ֵ����
short x; //ϵͳ����x
    short y; //ϵͳ����y
    short cursor_x; //��Ļ���x
    short cursor_y; //��Ļ���y
} BlcYunKeyMouseMsgBody;

typedef struct {
    BlcYunKeyMsgHead head; // ��Ϣͷ
BlcYunKeyMouseMsgBody body; // ��Ϣ��
} BlcYunKeyMouseMsg;

/***********************���͸�Ӧ����Ϣ�ṹ�嶨��*************************************************/


static int g_timeout;       //按键超时，长时间未响应按键时上报
static int g_keyfilt;       //键值过滤，0.5秒之内的重复按键过滤

#endif

