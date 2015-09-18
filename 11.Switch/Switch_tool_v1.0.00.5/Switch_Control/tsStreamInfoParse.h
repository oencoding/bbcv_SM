#ifndef __TSSTREAMINFOPARSE_H_
#define __TSSTREAMINFOPARSE_H_


const int TS_PACKET_SIZE = 188;

typedef struct _TS_packet_Header
{
	unsigned sync_byte						:8;
	unsigned transport_error_indicator		:1;   //��������־λ��һ�㴫�����Ļ��Ͳ��ᴦ���������
	unsigned payload_unit_start_indicator	:1;   //��Ч���صĿ�ʼ��־�����ݺ�����Ч���ص����ݲ�ͬ����Ҳ��ͬ
	unsigned transport_prority				:1;   //�������ȼ�λ��1��ʾ�����ȼ�
	unsigned PID							:13;
	unsigned transport_scrambling_control	:2;   //���ܱ�־λ,00��ʾδ����
	unsigned adaption_field_control			:2;   //�����ֶο���,��01������Ч���أ�10���������ֶΣ�11���е����ֶκ���Ч���ء�Ϊ00�Ļ������������д���
	unsigned continuity_counter				:4;    //һ��4bit�ļ���������Χ0-15

}TS_packet_Header;

static int tsGetPESLength(unsigned char* pBuff,int iLen);


#endif
