/**
*File:  DBAccessThread.h
*brief: ������:�ö���ά��һ�����߳�ʹ�õ����ݿ�Cn����
---------------------------------------���ݿ��߳���---------------------------------------
	�����߳��ɵ���Begin������ʼ������Exit����������
 һ��������Exit�����ӿڣ������ٽ����µ����񣬵��Ѿ�ѹ����������ִ����ɡ�
 ����ʱ���Զ����������̣߳����Ƽ��ֶ�����Exit������
 ���߳�ʹ�õ����ݿ������,���಻���Լ������߳�
------------------------------------------------------------------------------------------
*Author:������
*Date:  2008.11.03
*/
#pragma once
#include "ThreadDataDef.h"
#include "../../../../Public/ServerPublic/db/CnPool.h"
class DBAdoBase;

class DBAccessThread
{
public:
	DBAccessThread(string strProvider, 
				string strDataSource, 
				string strInitialCatalog,
				string strUserID, 
				string strPassword
				);
	~DBAccessThread(void);

public:
	//!								ConnctionPtr
	_ConnectionPtr					GetConnPtr(void);
	//!								OpenCn
	bool							OpenCn(void);
	DBAdoBase*						GetAdoBasePtr(void);

private:
	//!
	DBAdoBase*			m_pConnPtr;
	//!					���ݿ�����
	std::string			m_strInitialCatalog;
	//!					�߳����б��
	bool				m_Running;
	
};

