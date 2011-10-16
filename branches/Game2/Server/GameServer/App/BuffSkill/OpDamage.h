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

#pragma once

#include "Operator.h"
class CMoveShape;

const  float  lDamMulti = 100.0;
class COpDamage :
	public COperator
{
public:
	COpDamage(stModuParam *pParam);
	COpDamage(void *pParam);
	virtual ~COpDamage(void);
	//��������
	enum eDamageOper
	{
		//�Ӳ���
		DAMAOP_Add,
		//������
		DAMAOP_Sub,
        //�˷�����
        DAMAOP_Multi,
		//���ò���
		DAMAOP_Set,
		//���
		DAMAOP_Clear,
		//�����ڶ���
		DAMAOP_Do,
		//�ӷ���Ѫ
		MODOP_Inc,
        //�˷���Ѫ
        MODOP_MultiInc,
	};

private:
	//�˺�����
	tagDamageType m_lType;
	//ֵ
	long m_lValue;
	//������
	string m_strVariName;
    //@by:expter �˺�����
    string m_strDamParamname;

	//��������
	typedef void (COpDamage::*OPFUN)(CMoveShape* pShape);
	OPFUN m_pFun;

	void AddDamage(CMoveShape* pShape);
	void SubDamge(CMoveShape* pShape);
    void MultiDamge(CMoveShape* pShape);
	void SetDamge(CMoveShape* pShape);
	void ClearDamge(CMoveShape* pShape);
	void DoDamage(CMoveShape* pShape);
	void IncHP(CMoveShape* pShape);
    void MultiInc(CMoveShape* pShape);


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
        else if (Oper=="Multi")
        {
            op = &COpDamage::MultiDamge;
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
        else if (Oper=="MultiInc")
        {
            op = &COpDamage::MultiInc;
        }
		else
		{
			op = &COpDamage::AddDamage;
		}
		return op;
	}

	//��ʼ��
	void InitBaseData(eObj obj,string strOper,tagDamageType dType,long value,string strVname,string Damagename)
	{
		//m_eOp = Oper;
		m_lType = dType;
		m_lValue = value;
		m_strVariName = strVname;		
		m_pFun = GetFun(strOper);
		m_Obj = obj;
        m_strDamParamname = Damagename;
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
	COpDoDamage(void *pParam);
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
    //@by:expter ���ռ����˺�������
    string m_strDamParamname;       /// �����˺�����
    string m_strBlastParamname;     /// ���ձ����˺�����

	void DoDestDamage(CMoveShape* pShape);
	void DoSelfDamage(CMoveShape* pShape);
	void DoDestIncHp(CMoveShape* pShape);
	void DoSelfIncHp(CMoveShape* pShape);
	//void DoEnergy(CMoveShape *pShape);	//����ֵ����


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
	////��Ч��Ϣ
	//void SendEffectMsg(CMoveShape* pShape);
public:
	void InitBaseData(eObj Obj,long lRes,string strResVName,long lBlast,string strBlastVName,string Damname,string Blastname)
	{
		m_Obj = Obj;
		m_lDamageReslut = lRes;
		m_strDamaResVariName = strResVName;
		m_nBlastAttack = lBlast;
		m_strBlasAttVariName = strBlastVName;
        m_strDamParamname = Damname;
        m_strBlastParamname=Blastname;
	}	
	//ִ��
    void Execute(CMoveShape* pShape);
};
