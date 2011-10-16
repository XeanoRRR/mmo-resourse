//=============================================================================
/**
 *  file: Procedure.h
 *
 *  Brief:��װ��һ�����п�ʼ���¼�����ʱ����������Ĵ������
 *
 *  Authtor:wangqiao
 *	
 *	Datae:2007-8-31
 */
//=============================================================================

#ifndef __PROCEDURE_H__
#define __PROCEDURE_H__
#pragma once

#include "BuffSkillDef.h"

class CStep;
class CEffectObjects;
class CMoveShape;
struct tagEvenRegVar;

class CProcedure
{
public:
	CProcedure(stModuParam *pParam);
	CProcedure(void* pParam);
	~CProcedure(void);

private:
	//����Ŀ��
	stModuParam	*m_pParam;

	//��ʼִ�в���
	CStep	*m_pBegin;
	//�¼�ִ�в���
	CStep *m_pEvent;
	//��ʱִ�в���
	CStep *m_pTimeOut;
	//����ִ�в���
	CStep *m_pEnd;
	///////////////////////////////////////////////////////////////////////////////////
	//˽�нӿ�
private:
	///////////////////////////////////////////////////////////////////////////////////
public:

	void InitBaseData(string strStepName,CStep *pStep);
	void SetParam(stModuParam *pParam)	{ m_pParam = pParam;}

	//��ʼ
	eModuleRet Begin(stModuParam *pParam);
	//�¼�����ӿ�
	eModuleRet Proc(DWORD type,const tagProcVar *pVar,stModuParam *pParam);
	//��ʱ����ӿ�
	eModuleRet OnTimeOut(DWORD timerid,DWORD curtime,const void* var,stModuParam *pParam);
	//�������ýӿ�
	eModuleRet End(stModuParam *pParam);	
};


#endif