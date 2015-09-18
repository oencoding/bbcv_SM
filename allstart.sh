#!/bin/bash

#����ΪJava�ĳ���

#redis ��Ҫ������
#redis
Redis_Dir_Path=/home/x00/Penglei/redis
#redis�����ű�
Redis_Start_sh=start.sh
#������ �������ҵ��ý���
Redis_Proc_Name=redis-server
#�����ű�
Redis_Stop_sh=stop.sh
#���������ļ�
Redis_Conf_File1=mag.redis.conf
Redis_Conf_File2=msi.redis.conf
Redis_Conf_File3=crsm.redis.conf
Redis_Conf_File4=swm.redis.conf

#MAG 
MAG_Dir_Path=/home/x00/Penglei/MAG/bin
#�����ű���
MAG_Start_sh=mag-start.sh
#������
MAG_Proc_Name=MAG
#ֹͣ�ű���
MAG_Stop_sh=mag-stop.sh

#NS
NS_Dir_Path=/home/x00/Penglei/NSCS/apache-tomcat-6.0.24/bin
#�����ű���
NS_Start_sh=startup.sh
#������
NS_Proc_Name=NSCS
#ֹͣ�ű���
NS_Stop_sh=shutdown.sh

#UCMS
UCMS_Dir_Path=/home/x00/Penglei/UCMS/bin
#�����ű���
UCMS_Start_sh=ucms-start.sh
#������
UCMS_Proc_Name=UCMS
#ֹͣ�ű���
UCMS_Stop_sh=ucms-stop.sh

#VGW
VGW_Dir_Path=/home/x00/Penglei/VGW/bin
#�����ű�
VGW_Start_sh=vgw-start.sh
#������
VGW_Proc_Name=VGW
#ֹͣ�ű���
VGW_Stop_sh=vgw-stop.sh

#ADMIN
ADMIN_Dir_Path=/home/x00/Penglei/ADMIN/apache-tomcat-6.0.24/bin
#�����ű�
ADMIN_Start_sh=startup.sh
#������
ADMIN_Proc_Name=ADMIN
#ֹͣ�ű���
ADMIN_Stop_sh=shutdown.sh

#����CRSM
CRSM_Dir_Path=/home/x00/Penglei/CRSM/bin
#�����ű�
CRSM_Start_sh=start.sh
#������
CRSM_Proc_Name=crsm-core
#ֹͣ�ű���
CRSM_Stop_sh=stop.sh

#����ΪC����
#RSM����
RSM_Dir_Path=/home/x00/Penglei/rsm_mpeg2/rsm
#RSM�Ľ�����
RSM_Proc_Name=resourcemag
#RSM�������ű�
RSM_Start_sh=rsm-start.sh
#RSM��ֹͣ�ű�
RSM_Stop_sh=rsm-stop.sh

#SM��������
SM_Dir_Path=/home/x00/Penglei/SM
#SM�Ľ�����
SM_Proc_Name=SM_main 
#�����ű���
SM_Start_sh=smstart.sh
#ֹͣ�ű���
SM_Stop_sh=smstop.sh

#������ ����
SWM_Dir_Path=/home/x00/Penglei/SWManger
#���������ȵĽ�����
SWM_Proc_Name=SWM
#�����ű���
SWM_Start_sh=swm_start.sh
#ֹͣ�ű���
SWM_Stop_sh=swm_stop.sh

#������
SWT_Dir_Path=/home/x00/Penglei/Switch
#�������Ľ�����
SWT_Proc_Name=Switch_tool
#�����ű���
SWT_Start_sh=swt_start.sh
#ֹͣ�ű���
SWT_Stop_sh=swt_stop.sh

#�������
GuangGao_Dir_Path=/home/x00/Penglei/ADV
#������Ľ�����
GuangGao_Proc_Name=Mul_main
#�����ű���
GuangGao_Start_sh=mustart.sh
#ֹͣ�ű���
GuangGao_Stop_sh=mustop.sh 

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
        #echo  proc $pid	$onCount	
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

#ɱ����
function kill_command(){
	result=$?
	good_value=0
	command=''
	config=''
	declare -i onCount
	pidlist=`ps -ef |egrep "$1"|sed /.*grep/d|awk '{print $2}'`

	onCount=0
    for pid in $pidlist; do
		echo kill pid $pid start
		kill -9 $pid
		onCount=$onCount+1
        echo kill pid $pid end
		
    done
	echo "kill process $1 number=$onCount "

}

#������ֹͣ�ű�
cd $Redis_Dir_Path
sh $Redis_Stop_sh

cd $MAG_Dir_Path
sh $MAG_Stop_sh
cd $NS_Dir_Path
sh $NS_Stop_sh
cd $UCMS_Dir_Path
sh $UCMS_Stop_sh
cd $VGW_Dir_Path
sh $VGW_Stop_sh
cd $ADMIN_Dir_Path
sh $ADMIN_Stop_sh
cd $CRSM_Dir_Path
sh $CRSM_Stop_sh
echo stop java sh
sleep 3
cd $RSM_Dir_Path
sh $RSM_Stop_sh

cd $SWM_Dir_Path
sh $SWM_Stop_sh

cd $SWT_Dir_Path
sh $SWT_Stop_sh
cd $SM_Dir_Path
sh $SM_Stop_sh
cd $GuangGao_Dir_Path
sh $GuangGao_Stop_sh

echo stop sm sh 

#����ɱ����
kill_command $Redis_Proc_Name
kill_command $MAG_Proc_Name
kill_command $UCMS_Proc_Name
kill_command $VGW_Proc_Name
kill_command $ADMIN_Proc_Name
kill_command $CRSM_Proc_Name

kill_command $RSM_Proc_Name
kill_command $SWM_Proc_Name
kill_command $SWT_Proc_Name
kill_command $GuangGao_Proc_Name
kill_command $SM_Proc_Name


#�����ű�
cd $Redis_Dir_Path
sh $Redis_Start_sh >& /dev/null
sleep 5
check_last_command $Redis_Proc_Name $Redis_Conf_File1
check_last_command $Redis_Proc_Name $Redis_Conf_File2
check_last_command $Redis_Proc_Name $Redis_Conf_File3
check_last_command $Redis_Proc_Name $Redis_Conf_File4

cd $MAG_Dir_Path
sh $MAG_Start_sh >& /dev/null
echo start $MAG_Dir_Path/$MAG_Start_sh
cd $NS_Dir_Path
sh $NS_Start_sh >& /dev/null
echo start $NS_Dir_Path/$NS_Start_sh
cd $UCMS_Dir_Path
sh $UCMS_Start_sh >& /dev/null
echo start $UCMS_Dir_Path/$UCMS_Start_sh
sleep 1
cd $VGW_Dir_Path
sh $VGW_Start_sh >& /dev/null
echo start $VGW_Dir_Path/$VGW_Start_sh
cd $ADMIN_Dir_Path
sh $ADMIN_Start_sh >& /dev/null
echo start $ADMIN_Dir_Path/$ADMIN_Start_sh

sleep 5
cd $CRSM_Dir_Path
sh $CRSM_Start_sh >& /dev/null
echo start $CRSM_Dir_Path/$CRSM_Start_sh

sleep 2
cd $RSM_Dir_Path
sh $RSM_Start_sh >& /dev/null
echo start $RSM_Dir_Path/$RSM_Start_sh
sleep 10
cd $SWT_Dir_Path
sh $SWT_Start_sh >& /dev/null
echo start $SWT_Dir_Path/$SWT_Start_sh
cd $SWM_Dir_Path
sh $SWM_Start_sh >& /dev/null
echo start $SWM_Dir_Path/$SWM_Start_sh
cd $GuangGao_Dir_Path
sh $GuangGao_Start_sh >& /dev/null
echo start $GuangGao_Dir_Path/$GuangGao_Start_sh
sleep 10
cd $SM_Dir_Path
sh $SM_Start_sh >& /dev/null
echo start $SM_Dir_Path/$SM_Start_sh

sleep 2

#����Ӧ�Ľ����Ƿ���

check_last_command $MAG_Proc_Name
check_last_command $NS_Proc_Name
check_last_command $UCMS_Proc_Name
check_last_command $VGW_Proc_Name
check_last_command $ADMIN_Proc_Name
check_last_command $CRSM_Proc_Name

check_last_command $RSM_Proc_Name
check_last_command $SWM_Proc_Name
check_last_command $SWT_Proc_Name
check_last_command $GuangGao_Proc_Name
check_last_command $SM_Proc_Name


echo "----------ALL success -------------"



