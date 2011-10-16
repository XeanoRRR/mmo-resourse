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
	struct tagTmpParam
	{
		stModuParam *pParam;
		CCondiGroup* pCondiGroup;
	};
	CEffCirc(void *pParam);
	virtual ~CEffCirc(void);

private: 
    //�Ƿ��Ƿ�Χ�ڶ���
    bool      m_IsWithinObj;
	//���õ�����
	ePosition m_eCenter;
	//���þ���
	float     m_nRadius;
	//���ö������
	int       m_nNum;
    //���ö������
    string    m_strNum;
   
public:
	void InitBaseData(string strPosition,float distance,int num,bool Within,string strNum)
	{
        m_IsWithinObj = Within;
        m_strNum = strNum;
		m_eCenter = GetPosition(strPosition);	
		m_nRadius = distance;
		m_nNum = num;
		
	}
	//�õ����õ����ö���
	unsigned int GetEffectObjs(list<CMoveShape*>& listObjs);
};

#endif
