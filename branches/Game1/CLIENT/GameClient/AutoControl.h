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

#pragma once
#include "..\public\MapFile.h"


class AutoControl
{
public:
	AutoControl(void);
	virtual ~AutoControl(void);

private:
	//����ʵ��
	CMapFile* m_pMapFile;

public:
	// ��ʼ��
	bool InitAutoCtl();

	// ����
	bool RunAutoCtl();

	// �ͷ�
	void ReleaseAutoCtl();
};
