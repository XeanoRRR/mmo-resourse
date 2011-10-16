//=============================================================================
/**
 *  file: EffPoint.h
 *
 *  Brief:���ö����࣬����ĳ������ѡ��һ���ϵ����ö��󼯺�
 *
 *  Authtor:wangqiao
 *	
 *	Datae:2007-8-31
 */
//=============================================================================

#ifndef __EFFECT_POINT_H__
#define __EFFECT_POINT_H__


#pragma once
#include "effectobjects.h"

class CEffPoint :
	public CEffectObjects
{
public:
	CEffPoint(stModuParam *pParam,CCondiGroup* pCondiGroup);

	struct tagTmpParam
	{
		stModuParam *pParam;
		CCondiGroup* pCondiGroup;
	};
	CEffPoint(void *pParam);
	virtual ~CEffPoint(void);

private:
	//���õ�����
	ePosition m_ePos;
    
public:
	//��ʼ��
	void InitBaseData(string strName)
	{
		m_ePos = GetPosition(strName);
	}
	//�õ����õ����ö���
	unsigned int GetEffectObjs(list<CMoveShape*>& listObjs);
};

#endif
