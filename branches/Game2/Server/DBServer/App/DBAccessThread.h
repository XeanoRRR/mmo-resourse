#pragma once

#include "..\..\..\dbaccess\myadobase.h"


class DBAdoBase;
//! ---------------------------------------���ݿ��߳���---------------------------------------
//!	�����߳��ɵ���Begin������ʼ������Exit����������
//! һ��������Exit�����ӿڣ������ٽ����µ����񣬵��Ѿ�ѹ����������ִ����ɡ�
//! ����ʱ���Զ����������̣߳����Ƽ��ֶ�����Exit������
//! ���߳�ʹ�õ����ݿ������,���಻���Լ������߳�
//!---------------------------------------------------------------------------------------------------
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
	string				m_strInitialCatalog;
	//!					�߳����б��
	bool				m_Running;
	
};