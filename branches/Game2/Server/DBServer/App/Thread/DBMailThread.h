/**
*File:  DBMailThread.h
*brief: ������:�ö���ά��һ�����߳�ʹ�õ����ݿ�Cn����
---------------------------------------���ݿ��߳���---------------------------------------
	�����߳��ɵ���Begin������ʼ������Exit����������
 һ��������Exit�����ӿڣ������ٽ����µ����񣬵��Ѿ�ѹ����������ִ����ɡ�
 ����ʱ���Զ����������̣߳����Ƽ��ֶ�����Exit������
 ���߳�ʹ�õ����ݿ������,���಻���Լ������߳�
------------------------------------------------------------------------------------------
*Author:������
*Date:  2009.02.17
*/
#pragma once

#include "ThreadDataDef.h"
#include "DBThread.h"

class DBAdoBase;
class DBMailThread : public DBThread
{
public:
	DBMailThread(std::string strProvider, 
				std::string strDataSource, 
				std::string strInitialCatalog,
				std::string strUserID, 
				std::string strPassword
				);
	~DBMailThread(void);

public:
	//!		
	virtual bool					ProcessData(void);
};

