//=============================================================================
/**
 *  file: Step.h
 *
 *  Brief:���������࣬һ����������࣬ʵ��һ�������������жϺͲ���
 *
 *  Authtor:wangqiao
 *	
 *	Datae:2007-8-31
 */
//=============================================================================

#ifndef __BASE__STEP_H__
#define __BASE__STEP_H__
#pragma once

#include "BuffSkillDef.h"

class CDo;
class CMoveShape;

class CStep
{
public:
	CStep(void);
	virtual ~CStep(void);
private:
	typedef vector<CDo*>::iterator itDo;
	vector<CDo*>	m_DoSome;

public:
	//��ʼ����������
	void InitBaseData(CDo *pDo);
	//������������
	void Reset(CMoveShape* pUser);
	//ִ������Do
	eModuleRet DoSome(stModuParam *pParam);
};

#endif
