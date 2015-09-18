#include "DBInterface.h"

DBInterfacer * DBInterfacer::theInstance = NULL;
#define USEMUTEX

DBInterfacer::DBInterfacer()
{
	conn_ptr = NULL;
	res_ptr = NULL;	
	sqlrow = NULL;	
	fd = NULL;
	printf("---begin connect to db\n");
	fflush(stdout);
#ifdef USEMUTEX	
	pthread_mutex_init(&locker, NULL);
#endif
	Connect2DB(dbuser,dbpass,dbname,dbip);
}

DBInterfacer::~DBInterfacer()
{
	if(conn_ptr)
		mysql_close(conn_ptr);
	conn_ptr = NULL;
#ifdef USEMUTEX	
	pthread_mutex_destroy(&locker);
#endif

}

bool DBInterfacer::Connect2DB(char* strUser,char* strpasswd,
					char* strDataBase,char* strServer)
{
	if(conn_ptr)
		mysql_close(conn_ptr);
	conn_ptr = NULL;
	conn_ptr = mysql_init(NULL);
	unsigned int ulport = 0;
	if(!conn_ptr)	
	{		
		printf("mysql_inti faild\n");
		fflush(stdout);
		return false;	
	}
	printf("init mysql \n");
	fflush(stdout);
	conn_ptr = mysql_real_connect(conn_ptr,strServer,strUser,strpasswd,strDataBase,3306,NULL,0);
	//conn_ptr = = mysql_real_connect(conn_ptr,NULL,"root","123","db1",0,NULL,0);
	if(!conn_ptr)
	{
		printf("mysql connect error \n");
		fflush(stdout);
		return false;
	}
	printf("----connect mysql success\n");
	fflush(stdout);
	m_bHasConnect = true;
	return true;
}

bool DBInterfacer::LoadALLIPQAMInfo(MapIPQAMInfo &mapIpqaminfo)
{
		int res;
		char strSQL[1024] ={0};
		sprintf(strSQL,"select * from ucs_ipqam_info");
		res = mysql_query(conn_ptr,strSQL);
		if(!res)
		{
			res_ptr = mysql_store_result(conn_ptr);
			if(res_ptr)
			{			
				printf("%lu Rows\n",(unsigned long)mysql_num_rows(res_ptr));
				fflush(stdout);
				//j = mysql_num_fields(res_ptr);	 //��		
				while((sqlrow = mysql_fetch_row(res_ptr)))	
				{				
		/*			for(i=0;i<j;i++)
					{
						printf("%s\t",sqlrow[i]);
						
					}
		*/			
					IPQAMInfo *pTmpStream = new IPQAMInfo;
	
						int istreamID;
	/*	
	int iIPQAMNum;
	char strIpqamName[128];
	char strIpqamIP[128];
	int iIpqamManagerPort;
	char strIpqamType[128];
	char strIpqamModel[128];
	char strIpqamManufacturers[128];
	char strIpqamComment[256];
	int iIsSupportR6;

		
	*/	
					if(sqlrow[0] != NULL)
						pTmpStream->iIPQAMNum = atoi(sqlrow[0]);
					if(sqlrow[1] != NULL)
						memcpy(pTmpStream->strIpqamName,sqlrow[1],strlen(sqlrow[1])+1);
					if(sqlrow[2] != NULL)
						memcpy(pTmpStream->strIpqamIP,sqlrow[2],strlen(sqlrow[2])+1);
					if(sqlrow[3] != NULL)
						pTmpStream->iIpqamManagerPort = atoi(sqlrow[3]);
					if(sqlrow[4] != NULL)
						memcpy(pTmpStream->strIpqamType,sqlrow[4],strlen(sqlrow[4])+1);
					if(sqlrow[5] != NULL)
						memcpy(pTmpStream->strIpqamModel,sqlrow[5],strlen(sqlrow[5])+1);
					if(sqlrow[6] != NULL)
						memcpy(pTmpStream->strIpqamManufacturers,sqlrow[6],strlen(sqlrow[6])+1);
					if(sqlrow[7] != NULL)
						memcpy(pTmpStream->strIpqamComment,sqlrow[7],strlen(sqlrow[7])+1);
					if(sqlrow[8] != NULL)
						pTmpStream->iIsSupportR6 = atoi(sqlrow[8]);
			
					//printf("\n"); 		
					mapIpqaminfo.insert(MapIPQAMInfo::value_type(pTmpStream->iIPQAMNum, pTmpStream));
				}		
			}
	
		}
		else
		{
			printf("SELECT error:");
			fflush(stdout);
			return false;
		}
		
	
		return true;	


}

bool DBInterfacer::LoadNetWorkGroup(MapNetWorkGroup &mapNetWorkGroup)
{
		int res;
		char strSQL[1024] ={0};
		sprintf(strSQL,"select * from ucs_network_area");
		printf("%s \n",strSQL);
		fflush(stdout);
		res = mysql_query(conn_ptr,strSQL);
		if(!res)
		{
			res_ptr = mysql_store_result(conn_ptr);
			printf("load network group success\n");
			fflush(stdout);
			if(res_ptr)
			{			
				printf("%lu Rows\n",(unsigned long)mysql_num_rows(res_ptr));
				fflush(stdout);
				int j = mysql_num_fields(res_ptr);	 //��		
				while((sqlrow = mysql_fetch_row(res_ptr)))	
				{			
					for(int i=0;i<j;i++)
					{
						printf("%s\t",sqlrow[i]);
						fflush(stdout);
					}
					printf("\n");
					fflush(stdout);
					NetworkGroup *pTmpStream = new NetworkGroup;
					memset(pTmpStream,0,sizeof(NetworkGroup));
	/*

		char strNetRegionNum[128];
	char strNetRegionName[128];
	int hdiNavgationStreamNum;
	int sdiNavgationStreamNum;
	int hdiAdvertisementStreamNum;
	int sdiAdvertisementStreamNum;
	char strNetworkComment[256];
	*/				//printf("---%s\n",sqlrow[0]);	
					if(sqlrow[0] != NULL)
						memcpy(pTmpStream->strNetRegionNum,sqlrow[0],strlen(sqlrow[0])+1);
					if(sqlrow[1] != NULL)
						memcpy(pTmpStream->strNetRegionName,sqlrow[1],strlen(sqlrow[1])+1);
					if(sqlrow[2] != NULL)
						pTmpStream->hdiNavgationStreamNum = atoi(sqlrow[2]);
					//printf("---%s\n",sqlrow[3]);
					if(sqlrow[3] != NULL)
						pTmpStream->sdiNavgationStreamNum = atoi(sqlrow[3]);
					if(sqlrow[4] != NULL)
						pTmpStream->hdiAdvertisementStreamNum = atoi(sqlrow[4]);
					if(sqlrow[5] != NULL)
						pTmpStream->sdiAdvertisementStreamNum = atoi(sqlrow[5]);
					if(sqlrow[6] != NULL)
						memcpy(pTmpStream->strNetworkComment,sqlrow[6],strlen(sqlrow[6])+1);
			
				//	printf("insert one data group\n"); 		
					//mapNetWorkGroup.insert(<MapNetWorkGroup::value_type(pTmpStream->strNetRegionNum, pTmpStream));
					mapNetWorkGroup.insert(MapNetWorkGroup::value_type(pTmpStream->strNetRegionNum, pTmpStream));
					
				}		
			}
	
		}
		else
		{
			printf("SELECT error:");
			fflush(stdout);
			return false;
		}
		
	
		return true;		

}



bool DBInterfacer::LoadAllStreamStatus(MapStreamStatus &mapStreamStatustmp)
{

	printf("------begin query db\n");
	fflush(stdout);
	int res;
	char strSQL[1024] ={0};
	sprintf(strSQL,"select * from stream_session");
	res = mysql_query(conn_ptr,strSQL);
	if(!res)
	{
		res_ptr = mysql_store_result(conn_ptr);
		if(res_ptr)
		{			
			printf("%lu Rows\n",(unsigned long)mysql_num_rows(res_ptr));	
			fflush(stdout);
			//j = mysql_num_fields(res_ptr);	 //��	
			//printf("select ret = %d  \n", mysql_num_fields(res_ptr));
			while((sqlrow = mysql_fetch_row(res_ptr)))	
			{				
	/*			for(i=0;i<j;i++)
				{
					printf("%s\t",sqlrow[i]);
					
				}
	*/			
				StreamStatus *pTmpStream = new StreamStatus;
				memset(pTmpStream,0,sizeof(StreamStatus));

					int istreamID;
/*	
int istreamID;
char strStreamType[32];
char strStatus_date[128];
char strBind_userID[128];
char strBind_date[128];
char strSwitch_task_id[128];
char strSessionID[512];
*/	
				printf("----\n");
				fflush(stdout);

				//printf("%s, \t %s, \t %s, \t %s, \t %s,",sqlrow[0],
			    //sqlrow[3],sqlrow[4],sqlrow[5]);

				if(sqlrow[0] != NULL)
					pTmpStream->istreamID = atoi(sqlrow[0]);
				if(sqlrow[1] != NULL)
					memcpy(pTmpStream->strStreamType,sqlrow[1],strlen(sqlrow[1])+1);
				if(sqlrow[2] != NULL)
					memcpy(pTmpStream->strStatus_date,sqlrow[2],strlen(sqlrow[2])+1);
				if(sqlrow[3] != NULL)
					memcpy(pTmpStream->strBind_userID,sqlrow[3],strlen(sqlrow[3])+1);
				if(sqlrow[4] != NULL)
					memcpy(pTmpStream->strBind_date,sqlrow[4],strlen(sqlrow[4])+1);
				if(sqlrow[5] != NULL)
					memcpy(pTmpStream->strSwitch_task_id,sqlrow[5],strlen(sqlrow[5])+1);
				//if(sqlrow[6] == NULL)
				//memcpy(pTmpStream->strSessionID,sqlrow[6],strlen(sqlrow[6]));
				//memcpy(pTmpStream->strSerialNo,sqlrow[7],strlen(sqlrow[7]));
				//printf("\n");			
				mapStreamStatustmp.insert(MapStreamStatus::value_type(pTmpStream->istreamID, pTmpStream));
			}		
		}

	}
	else
	{
		printf("SELECT error:");
		fflush(stdout);
		return false;
	}

	printf("-----end db\n");
	fflush(stdout);
	return true;	


}


bool DBInterfacer::LoadAllStreamResource(MapStreamResource &mapStreamResourcetmp)
{
	int res;
	char strSQL[1024] ={0};
	sprintf(strSQL,"select * from ucs_stream_resource");
	res = mysql_query(conn_ptr,strSQL);
	if(!res)
	{
		res_ptr = mysql_store_result(conn_ptr);
		if(res_ptr)
		{			
			printf("%lu Rows\n",(unsigned long)mysql_num_rows(res_ptr));
			fflush(stdout);
			//j = mysql_num_fields(res_ptr);	 //��		
			while((sqlrow = mysql_fetch_row(res_ptr)))	
			{				
	/*			for(i=0;i<j;i++)
				{
					printf("%s\t",sqlrow[i]);
					
				}
	*/			
				StreamResource *pTmpStream = new StreamResource;
				if(sqlrow[0] != NULL)
					pTmpStream->iStreamID = atoi(sqlrow[0]);
				if(sqlrow[1] != NULL)
					pTmpStream->iOutPutPort = atoi(sqlrow[1]);
				if(sqlrow[2] != NULL)
					pTmpStream->iPidMaping = atoi(sqlrow[2]);

				if(sqlrow[3] != NULL)
					pTmpStream->iProgramNum = atoi(sqlrow[3]);
				if(sqlrow[4] != NULL)
					pTmpStream->iBitRate = atoi(sqlrow[4]);
				if(sqlrow[5] != NULL)
					pTmpStream->iIPQAMNum = atoi(sqlrow[5]);
				if(sqlrow[6] != NULL)
					memcpy(pTmpStream->strNetRegionNum,sqlrow[6],strlen(sqlrow[6])+1);
				if(sqlrow[7] != NULL)
					pTmpStream->iFreqPoint = atoi(sqlrow[7]);
				if(sqlrow[8] != NULL)
					memcpy(pTmpStream->strComment,sqlrow[8],strlen(sqlrow[8])+1);
				if(sqlrow[9] != NULL)
					memcpy(pTmpStream->strNav_url,sqlrow[9],strlen(sqlrow[9])+1);
				if(sqlrow[10] != NULL)
					pTmpStream->iIs_Need_Key = atoi(sqlrow[10]);
				if(sqlrow[11] != NULL)
					memcpy(pTmpStream->strMobile_url,sqlrow[11],strlen(sqlrow[11])+1);
				if(sqlrow[15] != NULL)
					pTmpStream->iWherether_HD = atoi(sqlrow[15]);
				//memcpy(pTmpStream->strSessionID,sqlrow[12],strlen(sqlrow[12]));
				//printf("\n");			
				mapStreamResourcetmp.insert(MapStreamResource::value_type(pTmpStream->iStreamID, pTmpStream));
			}		
		}

	}
	else
	{
		printf("SELECT error:");
		fflush(stdout);
		return false;
	}
	

	return true;
}



bool DBInterfacer::LoadOneGroupStreamResource(char *strNetworkNum,ListStreamResource &tmplistStreamResourcetmp)
{
#ifdef USEMUTEX
	pthread_mutex_lock(&locker);
#endif
	int res;
	char strSQL[1024] ={0};
	sprintf(strSQL,"select * from ucs_stream_resource where strNetRegionNum=%s",strNetworkNum);
	printf("---%s \n",strSQL);
	fflush(stdout);
	res = mysql_query(conn_ptr,strSQL);
	if(!res)
	{
		res_ptr = mysql_store_result(conn_ptr);
		if(res_ptr)
		{			
			printf("%lu Rows\n",(unsigned long)mysql_num_rows(res_ptr));
			fflush(stdout);
			//j = mysql_num_fields(res_ptr);	 //��		
			while((sqlrow = mysql_fetch_row(res_ptr)))	
			{				
	/*			for(i=0;i<j;i++)
				{
					printf("%s\t",sqlrow[i]);
					
				}
	*/			
			//	StreamResource *pTmpStream = new StreamResource;
			//	pTmpStream->iStreamID = atoi(sqlrow[0]);
/*				pTmpStream->iOutPutPort = atoi(sqlrow[1]);
				pTmpStream->iPidMaping = atoi(sqlrow[2]);
				pTmpStream->iProgramNum = atoi(sqlrow[3]);
				pTmpStream->iBitRate = atoi(sqlrow[4]);
				pTmpStream->iIPQAMNum = atoi(sqlrow[5]);
				memcpy(pTmpStream->strNetRegionNum,sqlrow[6],strlen(sqlrow[6]));
				pTmpStream->iFreqPoint = atoi(sqlrow[7]);
				memcpy(pTmpStream->strComment,sqlrow[8],strlen(sqlrow[8]));
				memcpy(pTmpStream->strNav_url,sqlrow[9],strlen(sqlrow[9]));
				pTmpStream->iIs_Need_Key = atoi(sqlrow[10]);
				memcpy(pTmpStream->strMobile_url,sqlrow[11],strlen(sqlrow[11]));
*/				//memcpy(pTmpStream->strSessionID,sqlrow[12],strlen(sqlrow[12]));
				//printf("\n");
				int iStreamID = 0;
				if(sqlrow[0] != NULL)
				{
					iStreamID = atoi(sqlrow[0]);
					printf("---group stream id =%d \n",iStreamID);
					fflush(stdout);
					tmplistStreamResourcetmp.push_back( iStreamID);
				}
			}		
		}

	}
	else
	{
		printf("SELECT error:");
		fflush(stdout);
#ifdef USEMUTEX		
		pthread_mutex_unlock(&locker);
#endif
		return false;
	}
#ifdef USEMUTEX	
	pthread_mutex_unlock(&locker);
#endif
	return true;

}


bool DBInterfacer::insert_table(int table_num,void *table) //����˵��:���ţ�������ֵ
{
	printf("----insert into 1\n");
	fflush(stdout);
#ifdef USEMUTEX
	pthread_mutex_lock(&locker);
#endif
	int res;
	StreamStatus *tab1;
	UserBehaviour *tab2;
	NetworkGroup *tab3;
	StreamResource *tab4;
	IPQAMInfo *tab5;
	char buf[2048];
	memset(buf,0,sizeof(buf));
	printf("----insert into 2\n");
	fflush(stdout);
	switch(table_num)
	{
		case 1:          // 1:����stream_session values ��
		{
			tab1 = (StreamStatus *)table;

			sprintf(buf,"insert into stream_session values(%d,\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',NULL,NULL)",\
				tab1->istreamID,tab1->strStreamType,tab1->strStatus_date,tab1->strBind_userID,tab1->strBind_date,tab1->strSwitch_task_id); //,tab1->strSessionID,tab1->strSerialNo
			printf("%s \n",buf);
			fflush(stdout);
			res =  mysql_query(conn_ptr, buf);
			break;
		}	
		case 2:         // 2:����ucs_user_action ��
		{
			tab2 = (UserBehaviour *)table;
			sprintf(buf,"insert into ucs_user_action(iStreamID,strUserName,strActionType,strActionDate,strNetworkComment,result)values(%d,\"%s\",\"%s\",\"%s\",\"%s\",%d)",\
				tab2->iStreamID,tab2->strUserName,tab2->strActionType,tab2->strActionDate,tab2->strNetworkComment,tab2->result);
			printf("buf = %s\n",buf);
			fflush(stdout);
			res = mysql_query(conn_ptr,buf);
			printf("res = %d,this is tab2\n",res);
			fflush(stdout);
			break;
		}	
		case 3:        // 3:����ucs_network_area ��
		{
		  /*	
			char strNetRegionNum[128];
			char strNetRegionName[128];
			int hdiNavgationStreamNum;
			int sdiNavgationStreamNum;
			int hdiAdvertisementStreamNum;
			int sdiAdvertisementStreamNum;
			char strNetworkComment[256];
		*/
			 tab3= (NetworkGroup *)table;
			 sprintf(buf,"insert into ucs_network_area values(%s,%s,%d,%d,%d,%d,%s)",\
			 	tab3->strNetRegionNum,tab3->strNetRegionName,tab3->hdiNavgationStreamNum,tab3->sdiNavgationStreamNum,tab3->hdiAdvertisementStreamNum,\
			 	tab3->sdiAdvertisementStreamNum,tab3->sdiAdvertisementStreamNum);
			 res =  mysql_query(conn_ptr,buf);
			break;
		}	
		case 4:       // 4:����ucs_stream_resource ��
		{
			tab4= (StreamResource *)table;
			sprintf(buf,"insert into ucs_stream_resource values(%d,%d,%d,%d,%d,%d,%s,%d,%s,%s,%d,%s)",\
					tab4->iStreamID,tab4->iOutPutPort,tab4->iPidMaping,tab4->iProgramNum,tab4->iBitRate,tab4->iIPQAMNum,\
					tab4->strNetRegionNum,tab4->iFreqPoint,tab4->strComment,tab4->strNav_url,tab4->iIs_Need_Key,tab4->strMobile_url);
			res =  mysql_query(conn_ptr,buf);
			break;
		}	
		case 5:     // 5:����ucs_ipqam_info values ��
		{
			tab5 = (IPQAMInfo *)table;
			sprintf(buf,"insert into ucs_ipqam_info values(%d,%s,%s,%d,%s,%s,%s,%s,%d)",\
					tab5->iIPQAMNum,tab5->strIpqamName,tab5->strIpqamIP,tab5->iIpqamManagerPort,tab5->strIpqamType,\
					tab5->strIpqamModel,tab5->strIpqamManufacturers,tab5->strIpqamComment,tab5->iIsSupportR6);
			res =  mysql_query(conn_ptr,buf);
			break;
		}	
		default:
		{
			printf("wrong table number\n");
			fflush(stdout);
			res = 1;
			break;
		}	
	}
	if(!res)
	{
		printf("Inserted %lu rows\n",(unsigned long)mysql_affected_rows(conn_ptr));
		fflush(stdout);
	}	
	else
	{
		printf("insert error:");
		fflush(stdout);
#ifdef USEMUTEX		
		pthread_mutex_unlock(&locker);
#endif
		return false;
	}
	printf("-----success insert stream_session\n");
	fflush(stdout);
#ifdef USEMUTEX	
	pthread_mutex_unlock(&locker);
#endif
	return true;
}

bool DBInterfacer::delete_table(int table_num,int pri_key)//����˵��:���ţ�����ֵ
{
#ifdef USEMUTEX
	pthread_mutex_lock(&locker);
#endif
	int res;
	char buf[2048];
	char table_name[64];
	char key_name[10];
	memset(table_name,0,sizeof(table_name));
	memset(key_name,0,sizeof(key_name));
	memset(buf,0,sizeof(buf));
	switch(table_num)
	{
		
		case 1:
		{	
			strcpy(table_name,"stream_session");
			strcpy(key_name,"istreamID");
			break;
		}
		case 2:
		{	
			strcpy(table_name,"ucs_user_action");
			strcpy(key_name,"iID");
			break;
		}
		case 3:
		{	
			strcpy(table_name,"ucs_network_area");
			strcpy(key_name,"strNetRegionNum");
			break;
		}
		case 4:
		{	
			strcpy(table_name,"ucs_stream_resource");
			strcpy(key_name,"iStreamID");
			break;
		}
		case 5:
		{	
			strcpy(table_name,"ucs_ipqam_info");
			strcpy(key_name,"iIPQAMNum");
			break;
		}
		default:
			res = 1;
			break;
	}
	sprintf(buf,"delete from %s where %s = %d",table_name,key_name,pri_key);
	printf("%s\n",buf);
	fflush(stdout);
	res =  mysql_query(conn_ptr,buf);
	if(res)
	{
		printf("delete error\n");
		fflush(stdout);
#ifdef USEMUTEX		
		pthread_mutex_unlock(&locker);
#endif
		return false;
	}
#ifdef USEMUTEX	
	pthread_mutex_unlock(&locker);
#endif
	return true;
}

bool DBInterfacer::update_table(int table_num,void *table)//����˵��:���ţ�����ֵ���������ƣ�����ֵ
{
//	pthread_mutex_lock(&locker);
	int pri_key;
	char *name;
	char *key;

	StreamStatus *tab1;
	UserBehaviour *tab2;
	NetworkGroup *tab3;
	StreamResource *tab4;
	IPQAMInfo *tab5;
	char buf[2048];
	memset(buf,0,sizeof(buf));
	int res = 0;
//	pthread_mutex_unlock(&locker);
	printf("----update into 1\n");
	fflush(stdout);

	switch(table_num)
	{
		case 1:          // 1:����stream_session values ��
		{
			//tab1= (NetworkGroup *)table;
			tab1 = (StreamStatus *)table;
			res = delete_table(table_num,tab1->istreamID);
			/*
			sprintf(buf,"update stream_session set strBind_userID = \"%s\" where istreamID = %s","NULL",table);
			printf("buf = %s\n",buf);
			res = mysql_query(conn_ptr,buf);
			if(res)
			{
				printf("error:1111111111\n");
			}
			*/
			break;
		}	
		case 2:         // 2:����ucs_user_action ��
		{
			tab2 = (UserBehaviour *)table;
			res = delete_table(table_num,tab2->iID);
			break;
		}	
		case 3:        // 3:����ucs_network_area ��
		{
#ifdef USEMUTEX			
			pthread_mutex_lock(&locker);
#endif
			 tab3= (NetworkGroup *)table;
			 sprintf(buf,"delete from %s where %s = \"%s\"",tab3,"strNetRegionNum",tab3->strNetRegionNum);
			 res = mysql_query(conn_ptr,buf);
#ifdef USEMUTEX			 
			 pthread_mutex_unlock(&locker);
#endif

			break;
		}	
		case 4:       // 4:����ucs_stream_resource ��
		{
			tab4= (StreamResource *)table;
			res = delete_table(table_num,tab4->iStreamID);
			break;
		}	
		case 5:     // 5:����ucs_ipqam_info values ��
		{
			tab5 = (IPQAMInfo *)table;
			res = delete_table(table_num,tab5->iIPQAMNum);
			break;
		}	
		default:
		{
			printf("wrong table number\n");
			fflush(stdout);
			res = 1;
			break;
		}	
	}	

	res = insert_table(table_num,table);
	if(!res)
	{
		printf("update error\n");
		fflush(stdout);
		return false;
	}
	
	return true;

}
bool DBInterfacer::select_table(int table_num,int pri_key)//:����˵��:���ţ�����ֵ
{
#ifdef USEMUTEX

	pthread_mutex_lock(&locker);
#endif
	int res;
	char table_name[20];
	char key_name[10];
	char buf[2048];
	int j,i;
	switch(table_num)
	{
		case 1:
		{	
			memcpy(table_name,"stream_session",sizeof(table_name));
			memcpy(key_name,"istreamID",sizeof(key_name));
			break;
		}
		case 2:
		{	
			memcpy(table_name,"ucs_user_action",sizeof(table_name));
			memcpy(key_name,"iID",sizeof(key_name));
			break;
		}
		case 3:
		{	
			memcpy(table_name,"ucs_network_area",sizeof(table_name));
			memcpy(key_name,"strNetRegionNum",sizeof(key_name));
			break;
		}
		case 4:
		{	
			memcpy(table_name,"ucs_stream_resource",sizeof(table_name));
			memcpy(key_name,"iStreamID",sizeof(key_name));
			break;
		}
		case 5:
		{	
			memcpy(table_name,"ucs_ipqam_info",sizeof(table_name));
			memcpy(key_name,"iIPQAMNum",sizeof(key_name));
			break;
		}
		default:
			res = 1;
			break;
	}
	sprintf(buf,"select *from %s where %s = %d",table_name,key_name,pri_key);
	res = mysql_query(conn_ptr, buf); //��ѯ���  
    if (res) 
	{         
    	printf("SELECT error:%s\n",mysql_error(conn_ptr));
		fflush(stdout);
    } 
	else
	{        
    	res_ptr = mysql_store_result(conn_ptr);             //ȡ�������  
        if(res_ptr) 
		{               
        	printf("%lu Rows\n",(unsigned long)mysql_num_rows(res_ptr));   
			fflush(stdout);
            j = mysql_num_fields(res_ptr);          
            while((sqlrow = mysql_fetch_row(res_ptr)))  
			{   //����ȡ����¼  
            	for(i = 0; i < j; i++)         
                printf("%s\t", sqlrow[i]);              //���  
                fflush(stdout);
                printf("\n");  
				fflush(stdout);
             }              
             if (mysql_errno(conn_ptr)) 
			 {                      
             	fprintf(stderr,"Retrive error:s\n",mysql_error(conn_ptr));               
             }        
          }               
     }  
#ifdef USEMUTEX

	pthread_mutex_unlock(&locker);
#endif

}

bool DBInterfacer::FindOneStream(int table_num,char* pri_key,char* key_value,void *table)
{
#ifdef USEMUTEX

	pthread_mutex_lock(&locker);
#endif

	int res;
	char table_name[64]={0};
	char key_name[10];
	char buf[2048];
	int j,i;

	printf("----find one stream table=%d where %s \n",table_num,key_value);
	fflush(stdout);
	//strcpy(key_name,pri_key);
	switch(table_num)
	{
		case 1:
		{	
			strcpy(table_name,"stream_session");
			//memcpy(key_name,"istreamID",sizeof(key_name));
			break;
		}
		case 2:
		{	
			strcpy(table_name,"ucs_user_action");
			//memcpy(key_name,"iID",sizeof(key_name));
			break;
		}
		case 3:
		{	
			strcpy(table_name,"ucs_network_area");
			//memcpy(key_name,"strNetRegionNum",sizeof(key_name));
			break;
		}
		case 4:
		{	
			printf("-----find Resource %s \n",pri_key);
			fflush(stdout);
			strcpy(table_name,"ucs_stream_resource");
			printf("---%s \n",table_name);
			fflush(stdout);
			//memcpy(key_name,"iStreamID",sizeof(key_name));

			break;
		}
		case 5:
		{	
			strcpy(table_name,"ucs_ipqam_info");
			//memcpy(key_name,"iIPQAMNum",sizeof(key_name));
			break;
		}
		default:
			res = 1;
			break;
	}
	if(strlen(key_value)>5)
	sprintf(buf,"select *from %s where %s = \"%s\"",table_name,pri_key,key_value);
	else
	sprintf(buf,"select *from %s where %s = %s",table_name,pri_key,key_value);	
	printf("%s\n",buf);
	fflush(stdout);
	res = mysql_query(conn_ptr, buf); //��ѯ���  
    if (res) 
	{         
    	printf("SELECT error:%s\n",mysql_error(conn_ptr));
		fflush(stdout);
    } 
	else
	{        
    	res_ptr = mysql_store_result(conn_ptr);             //ȡ�������  
        if(res_ptr) 
		{               

        	printf("%lu Rows\n",(unsigned long)mysql_num_rows(res_ptr)); 
			fflush(stdout);
			if((unsigned long)mysql_num_rows(res_ptr) <=0)
			{	
				printf("select false\n");
				fflush(stdout);
				pthread_mutex_unlock(&locker);
				return false;
			}
          //  j = mysql_num_fields(res_ptr);          
            while((sqlrow = mysql_fetch_row(res_ptr)))  
			{   //����ȡ����¼  
            //	for(i = 0; i < j; i++)         
            //    printf("%s\t", sqlrow[i]);              //���  
            //    printf("\n");       
			            	
				switch(table_num)
				{
					case 1:          // 1:����stream_session values ��
					{

						StreamStatus *pTmpStream = (StreamStatus *)table;
						memset(pTmpStream,0,sizeof(StreamStatus));

						int istreamID;
			
						printf("----\n");
						fflush(stdout);
						if(sqlrow[0] != NULL)
							pTmpStream->istreamID = atoi(sqlrow[0]);
						if(sqlrow[1] != NULL)
							memcpy(pTmpStream->strStreamType,sqlrow[1],strlen(sqlrow[1])+1);
						if(sqlrow[2] != NULL)
							memcpy(pTmpStream->strStatus_date,sqlrow[2],strlen(sqlrow[2])+1);
						if(sqlrow[3] != NULL)
							memcpy(pTmpStream->strBind_userID,sqlrow[3],strlen(sqlrow[3])+1);
						if(pTmpStream->strBind_userID!=NULL)
						{
							printf("555555555555555 strBind_userID = %s\n",pTmpStream->strBind_userID);
							fflush(stdout);
						}
						if(sqlrow[4] != NULL)
							memcpy(pTmpStream->strBind_date,sqlrow[4],strlen(sqlrow[4])+1);
						if(sqlrow[5] != NULL)
							memcpy(pTmpStream->strSwitch_task_id,sqlrow[5],strlen(sqlrow[5])+1);
						//memcpy(pTmpStream->strSessionID,sqlrow[6],strlen(sqlrow[6])+1);
						//memcpy(pTmpStream->strSerialNo,sqlrow[7],strlen(sqlrow[7])+1);
						
						break;
					}	
					case 2:         // 2:����ucs_user_action ��
					{
						//tab2 = (UserBehaviour *)table;
						
						break;
					}	
					case 3:        // 3:����ucs_network_area ��
					{

						NetworkGroup *pTmpStream = (NetworkGroup *)table;
						if(sqlrow[0] != NULL)
						  memcpy(pTmpStream->strNetRegionNum,sqlrow[0],strlen(sqlrow[0])+1);
						if(sqlrow[1] != NULL)
							memcpy(pTmpStream->strNetRegionName,sqlrow[1],strlen(sqlrow[1])+1);
						if(sqlrow[2] != NULL)
							pTmpStream->hdiNavgationStreamNum = atoi(sqlrow[2]);
						//printf("---%s\n",sqlrow[3]);
						if(sqlrow[3] != NULL)
							pTmpStream->sdiNavgationStreamNum = atoi(sqlrow[3]);
						if(sqlrow[4] != NULL)
							pTmpStream->hdiAdvertisementStreamNum = atoi(sqlrow[4]);
						if(sqlrow[5] != NULL)
							pTmpStream->sdiAdvertisementStreamNum = atoi(sqlrow[5]);
						if(sqlrow[6] != NULL)
							memcpy(pTmpStream->strNetworkComment,sqlrow[6],strlen(sqlrow[6])+1);
						break;
					}	
					case 4:       // 4:����ucs_stream_resource ��
					{
						
						StreamResource *pTmpStream = (StreamResource *)table;
						if(sqlrow[0] != NULL)
							pTmpStream->iStreamID = atoi(sqlrow[0]);
						if(sqlrow[1] != NULL)
							pTmpStream->iOutPutPort = atoi(sqlrow[1]);
						if(sqlrow[2] != NULL)
							pTmpStream->iPidMaping = atoi(sqlrow[2]);
						
						if(sqlrow[3] != NULL)
							pTmpStream->iProgramNum = atoi(sqlrow[3]);
						if(sqlrow[4] != NULL)
							pTmpStream->iBitRate = atoi(sqlrow[4]);
						if(sqlrow[5] != NULL)
							pTmpStream->iIPQAMNum = atoi(sqlrow[5]);
						if(sqlrow[6] != NULL)
							memcpy(pTmpStream->strNetRegionNum,sqlrow[6],strlen(sqlrow[6])+1);
						if(sqlrow[7] != NULL)
							pTmpStream->iFreqPoint = atoi(sqlrow[7]);
						if(sqlrow[8] != NULL)
							memcpy(pTmpStream->strComment,sqlrow[8],strlen(sqlrow[8])+1);
						if(sqlrow[9] != NULL)
							memcpy(pTmpStream->strNav_url,sqlrow[9],strlen(sqlrow[9])+1);
						if(sqlrow[10] != NULL)
							pTmpStream->iIs_Need_Key = atoi(sqlrow[10]);
						if(sqlrow[11] != NULL)
							memcpy(pTmpStream->strMobile_url,sqlrow[11],strlen(sqlrow[11])+1);
					//	if(sqlrow[12] != NULL)
					//		memcpy(pTmpStream->strMobile_url,sqlrow[11],strlen(sqlrow[11])+1);
						//if(sqlrow[12])	vod_pageurl
						//if(sqlrow[13])	bind_status
						if(sqlrow[14] != NULL)
							pTmpStream->iChannel_info = atoi(sqlrow[14]);
						if(sqlrow[15] != NULL)
							pTmpStream->iWherether_HD = atoi(sqlrow[15]);

						break;
					}	
					case 5:     // 5:����ucs_ipqam_info values ��
					{
						IPQAMInfo *pTmpStream = (IPQAMInfo *)table;
	

						if(sqlrow[0] != NULL)
							pTmpStream->iIPQAMNum = atoi(sqlrow[0]);
						if(sqlrow[1] != NULL)
							memcpy(pTmpStream->strIpqamName,sqlrow[1],strlen(sqlrow[1])+1);
						if(sqlrow[2] != NULL)
							memcpy(pTmpStream->strIpqamIP,sqlrow[2],strlen(sqlrow[2])+1);
						if(sqlrow[3] != NULL)
							pTmpStream->iIpqamManagerPort = atoi(sqlrow[3]);
						if(sqlrow[4] != NULL)
							memcpy(pTmpStream->strIpqamType,sqlrow[4],strlen(sqlrow[4])+1);
						if(sqlrow[5] != NULL)
							memcpy(pTmpStream->strIpqamModel,sqlrow[5],strlen(sqlrow[5])+1);
						if(sqlrow[6] != NULL)
							memcpy(pTmpStream->strIpqamManufacturers,sqlrow[6],strlen(sqlrow[6])+1);
						if(sqlrow[7] != NULL)
							memcpy(pTmpStream->strIpqamComment,sqlrow[7],strlen(sqlrow[7])+1);
						if(sqlrow[8] != NULL)
							pTmpStream->iIsSupportR6 = atoi(sqlrow[8]);
				
						break;
					}	
					default:
					{
						printf("wrong table number\n");
						fflush(stdout);
						res = 1;
						break;
					}	
				}	
             }              
             if (mysql_errno(conn_ptr)) 
			 {                      
             	fprintf(stderr,"Retrive error:s\n",mysql_error(conn_ptr));               
#ifdef USEMUTEX

				pthread_mutex_unlock(&locker);
#endif
				return false;
			 }        
          }               
     }  	
#ifdef USEMUTEX

	pthread_mutex_unlock(&locker);
#endif
	return true;
}

bool DBInterfacer::FindSameGroupStream(char *strNetworkNum,int ishd,SetGroupStream& setStreamTmp)
{
#ifdef USEMUTEX
	pthread_mutex_lock(&locker);
#endif
	int res;
	char strSQL[1024] ={0};
	sprintf(strSQL,"select * from ucs_stream_resource where strNetRegionNum=%s and strWhether_HD = %d",strNetworkNum,ishd);
	printf("---%s \n",strSQL);
	fflush(stdout);
	res = mysql_query(conn_ptr,strSQL);
	if(!res)
	{
		res_ptr = mysql_store_result(conn_ptr);
		if(res_ptr)
		{			
			printf("%lu Rows\n",(unsigned long)mysql_num_rows(res_ptr));	
			fflush(stdout);
			//j = mysql_num_fields(res_ptr);	 //��		
			while((sqlrow = mysql_fetch_row(res_ptr)))	
			{				
	
				int iStreamID = 0;
				if(sqlrow[0] != NULL)
				{
					iStreamID = atoi(sqlrow[0]);
					printf("---group stream id =%d \n",iStreamID);
					fflush(stdout);
					setStreamTmp.insert( iStreamID);
				}
			}		
		}

	}
	else
	{
		printf("SELECT error:");
		fflush(stdout);
#ifdef USEMUTEX		
		pthread_mutex_unlock(&locker);
#endif
		return false;
	}
#ifdef USEMUTEX	
	pthread_mutex_unlock(&locker);
#endif
	return true;	
	return true;
}

bool DBInterfacer::update_table(int pri,int status)
{
#ifdef USEMUTEX

	pthread_mutex_lock(&locker);
#endif
	int res;
	char buf[2048];
	memset(buf,0,sizeof(buf));
	sprintf(buf,"update ucs_stream_resource set bind_status = %d where iStreamID = pri",status,pri);
	res =  mysql_query(conn_ptr,buf);
	if(res)
	{
		printf("update status error\n");
		fflush(stdout);
#ifdef USEMUTEX		
		pthread_mutex_unlock(&locker);
#endif
		return false;
	}
	#ifdef USEMUTEX
	pthread_mutex_unlock(&locker);
	#endif
	return true;
}


