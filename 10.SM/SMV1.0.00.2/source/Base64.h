/**
* (C) Copyright 2009, asiainfo
* @version 	v1.0
* @author 	chenli
* @brief    base64编码和解码
*
* history:
* <p>chenli 2009-02-17 1.0 build this moudle</p>
*/

#ifndef ___BASE64_H___
#define ___BASE64_H___


#include <string>
using namespace std;

class ZBase64
{
public:
    /*编码
    DataByte
        [in]输入的数据长度,以字节为单位
    */
    string Encode(const unsigned char* Data,int DataByte);
	char * Encode( const unsigned char * bindata,char *base64, int binlength);
    /*解码
    DataByte
        [in]输入的数据长度,以字节为单位
    OutByte
        [out]输出的数据长度,以字节为单位,请不要通过返回值计算
        输出数据的长度
    */
    string Decode(const char* Data,int DataByte,int& OutByte);
};

#if 0
#include <string>

using namespace std;

class CBase64
{
public:
	CBase64();
	~CBase64();

	/*********************************************************
	* 函数说明：将输入数据进行base64编码
	* 参数说明：[in]pIn		需要进行编码的数据
				[in]uInLen  输入参数的字节数
				[out]strOut 输出的进行base64编码之后的字符串
	* 返回值  ：true处理成功,false失败
	* 作  者  ：ChenLi
	* 编写时间：2009-02-17
	**********************************************************/
	bool static Encode(const unsigned char *pIn, unsigned long uInLen, string& strOut);

	/*********************************************************
	* 函数说明：将输入数据进行base64编码
	* 参数说明：[in]pIn			需要进行编码的数据
				[in]uInLen		输入参数的字节数
				[out]pOut		输出的进行base64编码之后的字符串
				[out]uOutLen	输出的进行base64编码之后的字符串长度
	* 返回值  ：true处理成功,false失败
	* 作  者  ：ChenLi
	* 编写时间：2009-02-17
	**********************************************************/
	bool static Encode(const unsigned char *pIn, unsigned long uInLen, unsigned char *pOut, unsigned long *uOutLen);
	
	/*********************************************************
	* 函数说明：将输入数据进行base64解码
	* 参数说明：[in]strIn		需要进行解码的数据
				[out]pOut		输出解码之后的节数数据
				[out]uOutLen	输出的解码之后的字节数长度
	* 返回值  ：true处理成功,false失败
	* 作  者  ：ChenLi
	* 编写时间：2009-02-17
	**********************************************************/
	bool static Decode(const string& strIn, unsigned char *pOut, unsigned long *uOutLen) ;

	/*********************************************************
	* 函数说明：将输入数据进行base64解码
	* 参数说明：[in]strIn		需要进行解码的数据
				[out]pOut		输出解码之后的节数数据
				[out]uOutLen	输出的解码之后的字节数长度
	* 返回值  ：true处理成功,false失败
	* 作  者  ：ChenLi
	* 编写时间：2009-02-17
	**********************************************************/
	bool static Decode(const unsigned char *pIn, unsigned long uInLen, unsigned char *pOut, unsigned long *uOutLen) ;
};
#endif

#endif // ___BASE64_H___

