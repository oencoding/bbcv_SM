#ifndef _COMMONFUN_H_
#define _COMMONFUN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
char * replace(char *strbuf, const char *src_str, const char *desc_str);

int applyforUDPPort(char *ip,int *port);


int FindDataFromString(const char* strinput,char* strData1,char* strData2);


#endif