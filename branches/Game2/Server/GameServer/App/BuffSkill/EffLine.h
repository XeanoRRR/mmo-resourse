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
	struct tagTmpParam
	{
		stModuParam *pParam;
		CCondiGroup* pCondiGroup;
	};
	CEffLine(stModuParam *pParam,CCondiGroup* pCondiGroup);
	CEffLine(void *pParam);
	virtual ~CEffLine(void);

private:

    //�Ƿ��Ƿ�Χ�ڶ���
    bool      m_IsWithinObj;
	//���õ�����
	ePosition m_ePos;
	//���þ���
	int m_nDistance;
    //����
    int m_nNum;
    //���ö������
    string    m_strNum;
    
public:
    void InitBaseData(string strPosition,int distance,int num,bool Within,string strNum)
    {
        m_IsWithinObj = Within;
        m_strNum = strNum;
		m_ePos = GetPosition(strPosition);
		m_nDistance = distance;
        m_nNum  = num;
	}
	//�õ����õ����ö���
	unsigned int GetEffectObjs(list<CMoveShape*>& listObjs);
};

#endif
