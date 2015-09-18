#!/bin/bash

#SM��������
SM_Dir_Path=/home/x00/Penglei/SM/SM_1017
#SM�Ľ�����
SM_Proc_Name=SM_main 
#�����ű���
SM_Start_sh=smstart.sh
#ֹͣ�ű���
SM_Stop_sh=smstop.sh

#redis ��Ҫ������
#redis
Redis_Dir_Path=/home/bbcv/redis
#redis�����ű�
Redis_Start_sh=start.sh
#������
Redis_Proc_Name=redis-server
#���������ļ�
Redis_Conf_File1=ucms.redis.conf
Redis_Conf_File2=msc.redis.conf
Redis_Conf_File3=crsm.redis.conf
Redis_Conf_File4=swm.redis.conf

#������ ����
Switch_Manager_Dir_Path=/home/x00/Penglei/Switch_Manger
#���������ȵĽ�����
Switch_Manager_Proc_Name=SWM
#�����ű���
Switch_Manager_Start_sh=swm_start.sh
#ֹͣ�ű���
Switch_Manager_Stop_sh=swm_stop.sh

#������
Switch_Dir_Path=/home/x00/Penglei/Switch
#�������Ľ�����
Switch_Proc_Name=Switch_tool
#�����ű���
Switch_Start_sh=swt_start.sh
#ֹͣ�ű���
Switch_Stop_sh=swt_stop.sh

#�������
GuangGao_Dir_Path=/home/x00/Penglei/guangGao
#������Ľ�����
GuangGao_Proc_Name=Mul_main
#�����ű���
GuangGao_Start_sh=mustart.sh
#ֹͣ�ű���
GuangGao_Stop_sh=mustop.sh 

#����ΪJava�ĳ���

#MAG 
MAG_Dir_Path=/home/penglei/MAG/bin
#�����ű���
MAG_Start_sh=mag-start.sh
#������

#ֹͣ�ű���

#NS
NS_Dir_Path=/home/penglei/NS/apache-tomcat-6.0.24/bin/
#�����ű���
NS_Start_sh=startup.sh
#������

#ֹͣ�ű���

#UCMS
UCMS_Dir_Path=/home/penglei/UCMS/bin
#�����ű���
UCMS_Start_sh=ucms-start.sh
#������

#ֹͣ�ű���

#VGW
VGW_Dir_Path=/home/penglei/VGW/bin
#�����ű�
VGW_Start_sh=vgw-start.sh
#������

#ֹͣ�ű���

#ADMIN
ADMIN_Dir_Path=/home/penglei/ADMIN/apache-tomcat-6.0.24/bin
#�����ű�
ADMIN_Start_sh=startup.sh
#������

#ֹͣ�ű���

#����CRSM
CRSM_Dir_Path=/home/penglei/CRSM/bin
#�����ű�
ADMIN_Start_sh=start.sh
#������

#ֹͣ�ű���

#����Ƿ���ڽ���
function check_last_command(){
	result=$?
	good_value=0
	command=''
	config=''
	declare -i onCount
	pidlist=`ps -ef |egrep "$1"|sed /.*grep/d|awk '{print $8}'`

	onCount=0
    for pid in $pidlist; do
		onCount=$onCount+1
        echo  pid $pid	$onCount	
    done
	
	if [ $onCount -lt 1 ]
	then
		echo "open process $1 error, please make sure $1 is ok"
		exit 
	fi
	
	if [ $# -eq 1 ]
	then
		echo "check $1 result good go to next "
		return 0
	fi
	
	conflist=`ps -ef |egrep "$1"|egrep "$2"|sed /.*grep/d|awk '{print $9}'`	
	onCount=0
	for conf in $conflist; do
		onCount=$onCount+1
        echo  config $conf $onCount	
    done
	
	if [ $onCount -lt 1 ]
	then
		echo "open process $1 $2 error, please make sure $1 $2 is ok"
		exit 
	fi

	echo "check $1  $2 result good go to next "

}

#������Java����ģ��
#cd $Redis_Dir_Path
#sh $Redis_Start_sh
#����Ƿ���ڽ���
check_last_command $Redis_Proc_Name $Redis_Conf_File1
check_last_command $Redis_Proc_Name $Redis_Conf_File2
check_last_command $Redis_Proc_Name $Redis_Conf_File3
check_last_command $Redis_Proc_Name $Redis_Conf_File4



sleep 1
echo "----------over success -------------"



