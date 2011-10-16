//=============================================================================
/**
*  file: EffAppObject.h
*
*  Brief:���ö����࣬����ĳ������ѡ��һ������������ö��󼯺�
*
*  Authtor:Andy_Chan
*	
*	Datae:2008-1-7
*/
//=============================================================================

#ifndef __EFFECT_APPOBJECT_H__
#define __EFFECT_APPOBJECT_H__


#pragma once
#include "effectobjects.h"

class CEffAppObject :
	public CEffectObjects
{
public:
	CEffAppObject(stModuParam *pParam,CCondiGroup* pCondiGroup);
	struct tagTmpParam
	{
		stModuParam *pParam;
		CCondiGroup* pCondiGroup;
	};
	CEffAppObject(void *pParam);
	virtual ~CEffAppObject(void);
	unsigned int GetEffectObjs(list<CMoveShape*>& listObjs);
};

#endif