n=`ps -ef |grep resourcemag|grep -v "grep"`
if [ -n "$n" ];then 
echo "rsm���̴���"
echo "========================================="
else
echo "rsm���̲�����"
echo "�뵽Ŀ¼/home/x00/yyd/yyd/rsmִ�� sh rsm-start.sh����"
echo "========================================="
fi

n=`ps -ef |grep Mul_main|grep -v "grep"`
if [ -n "$n" ];then 
echo "�����̴���"
echo "========================================="
else
echo "�����̲�����"
echo "�뵽Ŀ¼/home/x00/Penglei/ADVִ��sh mustart.sh ����"
echo "========================================="
fi

n=`ps -ef |grep Switch_main|grep -v "grep"`
if [ -n "$n" ];then 
echo "���������̴���"
echo "========================================="
else
echo "���������̲�����"
echo "�뵽Ŀ¼/home/x00/Penglei/Switchִ�� sh switch.sh����"
echo "========================================="
fi


n=`ps -ef |grep SM_main|grep -v "grep"`
if [ -n "$n" ];then 
echo "SM���̴���"
echo "========================================="
else
echo "SM���̲�����"
echo "�뵽Ŀ¼/home/x00/Penglei/SMִ��sh smstart.sh����"
echo "========================================="
fi

n=`ps -ef |grep VGW|grep -v "grep"`
if [ -n "$n" ];then 
echo "VGW���̴���"
echo "========================================="
else
echo "VGW���̲�����"
echo "�뵽Ŀ¼/home/x00/Penglei/VGW/binִ��sh vgw-start.sh����"
echo "========================================="
fi