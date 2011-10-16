//=============================================================================
/**
 *  file: EffLineFirst.h
 *
 *  Brief:���ö����࣬����ĳ������ѡ��һ��ֱ���ϵ�һ�����ö���
 *
 *  Authtor:wangqiao
 *	
 *	Datae:2007-8-31
 */
//=============================================================================

#ifndef __EFFECT_LINE_FIRST_H__
#define __EFFECT_LINE_FIRST_H__

#pragma once
#include "effectobjects.h"

class CEffLineFirst :
	public CEffectObjects
{
public:
	CEffLineFirst(stModuParam *pParam,CCondiGroup* pCondiGroup);
	virtual ~CEffLineFirst(void);
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


class CEffEventByParam :
	public CEffectObjects
{
public:
	CEffEventByParam(stModuParam *pParam,CCondiGroup* pCondiGroup);
	virtual ~CEffEventByParam(void);
public:	
	//�õ����õ����ö���
	unsigned int GetEffectObjs(list<CMoveShape*>& listObjs);
};


#endif
