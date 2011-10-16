//=============================================================================
/**
 *  file: Buff.h
 *
 *  Brief:�����ʵʩ�࣬��װ��һ���������Ϻ�һЩ����������
 *
 *  Authtor:wangqiao
 *	
 *	Datae:2007-8-31
 */
//=============================================================================

#ifndef __DO_H__
#define __DO_H__
#pragma once

#include "BuffSkillDef.h"

class CCondiGroup;
class CEffectObjects;
class COperator;
class CMoveShape;

class CDo
{
public:
	CDo(stModuParam *pParam);
	CDo(void *pParam);
	virtual ~CDo(void);

	struct stRet
	{
		eModuleRet m_etrueRet;
		eModuleRet m_efalseRet;
	};

	//��������
	typedef vector<COperator*>::iterator itOper;
	struct tagGroup
	{
		//������
		CCondiGroup *pCondiGroup;
		//������ȷʱִ�еĲ���
		vector<COperator*> pTrueOperators;
		//��������ʱ���ִ�в���
		vector<COperator*> pFalseOperators;
		//ִ����������
		tagGroup *pTrueGroup;
		//ִ�м�������
		tagGroup *pFalseGroup;

		//���ؽ��
		stRet Ret;
		void Init(eModuleRet tret,eModuleRet fret)
		{
			Ret.m_etrueRet = tret;
			Ret.m_efalseRet = fret;
		}
	};
private:
	//����Ŀ��
	stModuParam *m_pParam;	
	//���ö���
	CEffectObjects *m_pEffObjects;
	//�����鼯��
	typedef vector<tagGroup*>::iterator itGroup;
	vector<tagGroup*> m_Groups;
	eModuleRet m_Ret;
	///////////////////////////////////////////////////////////////////////////////
	//˽�нӿ�
private:
	void DoOperator(list<CMoveShape*>& listObjs,vector<COperator*>& Operators);
	void DoOperator(CMoveShape* pObj,vector<COperator*>& Operators);
	void SendSKillFailMsg(int nValue);
	///////////////////////////////////////////////////////////////////////////////
	//���������ӿ�
public:
	//��ʼ��
	void InitBaseData(CEffectObjects *pEffObjects,vector<tagGroup*> &vtgGroup,eModuleRet eRet)
	{
		m_pEffObjects = pEffObjects;
		m_Groups = vtgGroup;
		m_Ret = eRet;
	}
	//ִ��
	eModuleRet Execute(stModuParam *pParam);
	//����
	void  DoTrueOrFalseOperator(eModuleRet &eRet,tagGroup *ptgGroup,CMoveShape *pMoveShape);
	//�ͷ�
	void  ClearGroup(tagGroup *group);
    //����״̬
    void  AnnexGeniusStates(stModuParam *pParam);
};

#endif
