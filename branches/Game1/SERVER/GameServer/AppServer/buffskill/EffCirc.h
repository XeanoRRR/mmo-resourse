//=============================================================================
/**
 *  file: EffectCirc.h
 *
 *  Brief:���ö����࣬����ĳ������ѡ��һ��Բ�ε����ö��󼯺�
 *
 *  Authtor:wangqiao
 *	
 *	Datae:2007-8-31
 */
//=============================================================================

#ifndef __EFFECT_CIRC_H__
#define __EFFECT_CIRC_H__


#pragma once
#include "effectobjects.h"

class CEffCirc :
	public CEffectObjects
{
public:
	CEffCirc(stModuParam *pParam,CCondiGroup* pCondiGroup);
	virtual ~CEffCirc(void);

private:
	//���õ�����
	ePosition m_eCenter;
	//���þ���
	int m_nRadius;
	//���ö������
	int m_nNum;
public:
	void InitBaseData(string strPosition,int distance,int num)
	{
		m_eCenter = GetPosition(strPosition);	
		m_nRadius = distance;
		m_nNum = num;
		
	}
	//�õ����õ����ö���
	unsigned int GetEffectObjs(list<CMoveShape*>& listObjs);
};

#endif
