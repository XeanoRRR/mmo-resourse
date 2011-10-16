// Fox@20081120----------------------------------------------
// File:    Pet.h
// Brief:   CPet�����˳�������Լ�����. ���нӿ���δ���ǿ���������.
//			��Ҫ�漰�����ʼ�����ͷ�, �����¼�������Ӧ.
//
// Author:  Fox (yulefox@gmail.com)
// Date:    Nov.18, 2008
// Fox@20081118----------------------------------------------

#pragma once
#include "../MoveShape.h"
#include "PetCtrl.h"
#include "../../../../Public/Common/BasePetDef.h"

class CPet :
	public CMoveShape
{
public:
	CPet(void);
	~CPet(void);

	friend class CPetCtrl;

	virtual bool CodeToDataBlock(DBWriteSet& setWriteDB,
		bool bExData = true);							// WS==>GS����
	virtual bool DecodeFromDataBlock(DBReadSet& setReadDB,
		bool bExData = true);							// GS==>WS����

private:

	// ���ýṹ��ʽ��ȡ�ĳ�Ա������Ϊ��������, ������������ݴ�ȡ
	// ����ǻ�������, ��ʹ��ʱӦע�����ʹ��.
	tagPetDiff			m_DiffAttrs;					// ��������
	CGUID				m_HostID;						// ����
	SKILL_LIST			m_Skills;						// �����б�
};
