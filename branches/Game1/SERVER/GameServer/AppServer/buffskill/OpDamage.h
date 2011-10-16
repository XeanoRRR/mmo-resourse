//=============================================================================
/**
 *  file: OpDamage.h
 *
 *  Brief:��װ���˺�ֵ�Ĳ���
 *
 *  Authtor:wangqiao
 *	
 *	Datae:2007-9-7
 */
//=============================================================================

#ifndef __OPERATOR_DAMAGE_H__
#define __OPERATOR_DAMAGE_H__

#pragma once
#include "operator.h"
#include "..\public\SkillRelated.h"
#include "..\..\..\..\public\MsgType.h"

class CMoveShape;

class COpDamage :
	public COperator
{
public:
	COpDamage(stModuParam *pParam);
	virtual ~COpDamage(void);
	//��������
	enum eDamageOper
	{
		//�Ӳ���
		DAMAOP_Add,
		//������
		DAMAOP_Sub,
		//���ò���
		DAMAOP_Set,
		//���
		DAMAOP_Clear,
		//�����ڶ���
		DAMAOP_Do,
		//��Ѫ
		MODOP_Inc,
	};

private:
	//����
	eModuleOper m_eOp;
	//�˺�����
	tagDamageType m_lType;
	//ֵ
	long m_lValue;
	//������
	string m_strVariName;
	//��������
	typedef void (COpDamage::*OPFUN)(CMoveShape* pShape);
	OPFUN m_pFun;

	void AddDamage(CMoveShape* pShape);
	void SubDamge(CMoveShape* pShape);
	void SetDamge(CMoveShape* pShape);
	void ClearDamge(CMoveShape* pShape);
	void DoDamage(CMoveShape* pShape);
	void IncHP(CMoveShape* pShape);


public:
	OPFUN GetFun(string Oper)
	{
		OPFUN op;

		if (Oper=="Add")
		{
			op = &COpDamage::AddDamage;//OPFUN *
		} 
		else if(Oper=="Sub")
		{
			op = &COpDamage::SubDamge;
		}
		else if (Oper=="Set")
		{
			op = &COpDamage::SetDamge;
		}
		else if (Oper=="Clear")
		{
			op = &COpDamage::ClearDamge;
		}	
		else if (Oper=="Inc")
		{
			op = &COpDamage::IncHP;
		}
		else
		{
			op = &COpDamage::AddDamage;
		}
		return op;
	}

	//��ʼ��
	void InitBaseData(eObj obj,string strOper,tagDamageType dType,long value,string strVname)
	{
		m_lType = dType;
		m_lValue = value;
		m_strVariName = strVname;		
		m_pFun = GetFun(strOper);
		m_Obj = obj;
	}
	
	
	//ִ��
    void Execute(CMoveShape* pShape);
};


//ʵ�ֶ�Ŀ����˺�
class COpDoDamage :
	public COperator
{
public:
	COpDoDamage(stModuParam *pParam);
	virtual ~COpDoDamage(void);
private:
	//ֵ
	long m_lDamageReslut;
	//������
	string m_strDamaResVariName;
	//�Ƿ񱩻�
	long m_nBlastAttack;
	//�ű�������
	string m_strBlasAttVariName;

	void DoDestDamage(CMoveShape* pShape);
	void DoSelfDamage(CMoveShape* pShape);
	void DoDestIncHp(CMoveShape* pShape);
	void DoSelfIncHp(CMoveShape* pShape);
	
	//��ͨ��Ѫ��Ϣ
	void SendSelfDamageMsg(CMoveShape* pShape);
	//��Ѫ��Ϣ
	void SendDestDamageMsg(CMoveShape* pShape,hash_set<CGUID,hash_guid_compare> &hsPlayers);
	//����������Ϣ
	void SendDestDodgeMsg(CMoveShape* pShape,hash_set<CGUID,hash_guid_compare> &hsPlayers);
	//��������Ϣ
	void SendDestBlockMsg(CMoveShape* pShape,hash_set<CGUID,hash_guid_compare> &hsPlayers);
	//�м���Ϣ
	void SendDestParryMsg(CMoveShape* pShape,hash_set<CGUID,hash_guid_compare> &hsPlayers);
	//�ֿ���Ϣ
	void SendDestResistMsg(CMoveShape* pShape,hash_set<CGUID,hash_guid_compare> &hsPlayers);
	//������Ϣ
	void SendDestSuckMsg(CMoveShape* pShape,hash_set<CGUID,hash_guid_compare> &hsPlayers);
	//������Ϣ
	void SendDestReboundMsg(CMoveShape* pShape,hash_set<CGUID,hash_guid_compare> &hsPlayers);
	//��Ѫ
	void SendDestIncHpMsg(CMoveShape *pShape,hash_set<CGUID,hash_guid_compare> &hsPlayers);
	//��ʧ��Ϣ
	void SendSelfLoseMsg(hash_set<CGUID,hash_guid_compare> &hsPlayers);
	//������Ϣ
	void SendDiedMsg(CMoveShape* pShape);
public:
	void InitBaseData(eObj Obj,long lRes,string strResVName,long lBlast,string strBlastVName)
	{
		m_Obj = Obj;
		m_lDamageReslut = lRes;
		m_strDamaResVariName = strResVName;
		m_nBlastAttack = lBlast;
		m_strBlasAttVariName = strBlastVName;
	}	
	//ִ��
    void Execute(CMoveShape* pShape);
};
#endif
