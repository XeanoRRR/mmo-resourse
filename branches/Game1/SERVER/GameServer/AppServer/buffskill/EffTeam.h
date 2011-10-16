//=============================================================================
/**
 *  file: EffTeam.h
 *
 *  Brief:���ö����࣬����ĳ������ѡ��һ������������ö��󼯺�
 *
 *  Authtor:wangqiao
 *	
 *	Datae:2007-8-31
 */
//=============================================================================

#ifndef __EFFECT_TEAM_H__
#define __EFFECT_TEAM_H__


#pragma once
#include "effectobjects.h"

class CEffTeam :
	public CEffectObjects
{
public:
	CEffTeam(stModuParam *pParam,CCondiGroup* pCondiGroup);
	virtual ~CEffTeam(void);
private:
	//���õ�����
	ePosition m_eCenter;
	//���þ���
	int m_nDistance;
public:
	void InitBaseData(string strPosition,int distance)
	{
		m_eCenter = GetPosition(strPosition);
		m_nDistance = distance;
	}
	//�õ����õ����ö���
	unsigned int GetEffectObjs(list<CMoveShape*>& listObjs);
};

#endif
