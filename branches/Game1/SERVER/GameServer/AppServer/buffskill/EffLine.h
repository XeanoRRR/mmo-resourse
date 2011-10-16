//=============================================================================
/**
 *  file: EffLine.h
 *
 *  Brief:���ö����࣬����ĳ������ѡ��һ��ֱ���ϵ����ö��󼯺�
 *
 *  Authtor:wangqiao
 *	
 *	Datae:2007-8-31
 */
//=============================================================================

#ifndef __EFFECT_LINE_H__
#define __EFFECT_LINE_H__


#pragma once
#include "effectobjects.h"

class CEffLine :
	public CEffectObjects
{
public:
	CEffLine(stModuParam *pParam,CCondiGroup* pCondiGroup);
	virtual ~CEffLine(void);
private:
	//���õ�����
	ePosition m_ePos;
	//���þ���
	int m_nDistance;
public:
	void InitBaseData(string strPosition,int distance)
	{
		m_ePos = GetPosition(strPosition);
		m_nDistance = distance;
	}
	//�õ����õ����ö���
	unsigned int GetEffectObjs(list<CMoveShape*>& listObjs);
};

#endif
