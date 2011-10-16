/************************************************************
*  file: AutoControl.h
*
*  Brief:�Զ������࣬Ϊ�ͻ����ṩ�Զ�����һϵ���¼��Ľӿڣ�
*		 ��Ҫ������mapfile�϶�ȥ���ݣ�Ȼ����в���
*
*  Authtor:liuke
*
*  Datae:2009-01-15
*************************************************************/

#include "StdAfx.h"
#include "AutoControl.h"

AutoControl::AutoControl(void)
{
}

AutoControl::~AutoControl(void)
{
}

// ��ʼ��
bool AutoControl::InitAutoCtl()
{
	//����Ŀ¼,���Ҫд�ļ����ͷŵ���Ŀ¼����
	CreateDirectory("AutoControl",NULL);
	m_pMapFile = new CMapFile();
	// ��ʼ��mapfile������ʽ��ӳ���������ӳ���ļ���
	if( !m_pMapFile->Initial(FMT_Server,"AutoControl","mapfile"))
	{
		SAFE_DELETE(m_pMapFile);
		return false;
	}
	return true;
}

// ����
bool AutoControl::RunAutoCtl()
{
	if(m_pMapFile)
	{
		eFileOpType OpType = File_No;
		//����Read����ʱ������ȡ���ݵ�buff
		char pszFileName[MAX_FILENAME_LEN]="";
		char pszContent[MAX_CONTENT_LEN]="";
		OpType = m_pMapFile->Read(pszFileName,pszContent);
		// ��д������ݣ�������������
		if(OpType == File_Write)
		{
		}
		return true;
	}
	return false;
}

// �ͷ�
void AutoControl::ReleaseAutoCtl()
{
	if(m_pMapFile)
	{
		m_pMapFile->DoFile(End,"");
		m_pMapFile->Release();
		SAFE_DELETE(m_pMapFile);
	}
}
