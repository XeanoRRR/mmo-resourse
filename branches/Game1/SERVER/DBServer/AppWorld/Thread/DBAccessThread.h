//DBAccessThread.h/////////////////////////////////////////////////////////////////////
//������:�ö���ά��һ�����߳�ʹ�õ����ݿ�Cn����
//Author:������
//Create Time:2008.11.03
#pragma once
#include "..\..\dbaccess\myadobase.h"
#include <list>
#include "ThreadDataDef.h"
#include <string>


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
	DBAccessThread(std::string strProvider, 
				std::string strDataSource, 
				std::string strInitialCatalog,
				std::string strUserID, 
				std::string strPassword
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

